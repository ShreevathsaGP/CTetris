#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "constants.h"

// helper functions
i32 min(i32 x, i32 y);
i32 max(i32 x, i32 y);
i32 randrange(i32 min, i32 max);

// priotity queue (DATA STRUCTURE - 2)
typedef struct PQNode { u8 data; u8 priority; } PQ_NODE;
typedef struct PriorityQueue { PQ_NODE *array; i16 size; i16 capacity; } PQ;

// priority queue functions
PQ *newPQ(i16 capacity);
bool is_empty(PQ *p);
void enqueue(PQ *p, u8 data, u8 priority);
u8 peek(PQ *p);
u8 dequeue(PQ *p);
void printPQ(PQ *p);

// Fisher-Yates shuffle algorithm (generating random order)
void shuffle(u8 *array, int size);

// tetromino types
extern const u8 I[];
extern const u8 J[];
extern const u8 L[];
extern const u8 O[];
extern const u8 S[];
extern const u8 T[];
extern const u8 Z[];

// colurs
extern const SDL_Color boxColour;

// level wait times
extern const u16 WAIT_PER_DROP[];

// input state
typedef struct InputState {
    u8 a;
    u8 up;
    u8 down;
    u8 left;
    u8 right;

    i8 difference_a;
    i8 difference_up;
    i8 difference_down;
    i8 difference_left;
    i8 difference_right;
} INPUT_STATE;

// tetromino 
typedef struct Tetromino { const u8 *matrix; const i32 side; } TETROMINO;
typedef struct TetrominoState {
    u8 index; // index in TETROMINOES array
    i32 row; // offset row
    i32 col; // offset col
    i32 rotation; // rotation [0: 0| 1: 90 | 2: 180 | 3: 270] (deg clockwise)
} TETROMINO_STATE;
const TETROMINO TETROMINOES[7];

// tetromino functions
u8 get_tetromino(const TETROMINO *tetromino, i32 row, i32 col, i32 rotation);
bool is_tetromino_valid(const TETROMINO_STATE *tet, const u8 *matrix, i32 width, i32 height);


// game state
typedef enum GamePhase { GP_START, GP_PLAY, GP_LINE_CLEARANCE, GP_GAMEOVER } GAME_PHASE;
typedef struct GameState {
    u8 lines[HEIGHT];
    i32 remaining_lines;
    u8 matrix[WIDTH * HEIGHT];

    GAME_PHASE current_phase;
    TETROMINO_STATE current_tetromino;

    i32 no_lines;
    i32 no_points;
    i32 current_level;
    i32 starting_level;

    f32 drop_time; // time for next drop
    f32 current_time;
    f32 highlight_end; // time for ending highlight

    PQ *priority_queue;
} GAME_STATE;

// games state functions
u32 next_drop_time(i32 level);
void generate_tetromino(GAME_STATE *game_state);
bool soft_drop(GAME_STATE *game_state);
i32 lines_needed(i32 start_level, i32 current_level); // lines needed for next level
                                                      //
void start_state_update(GAME_STATE *game_state, const INPUT_STATE *input_state);
void gameover_state_update(GAME_STATE *game_state, const INPUT_STATE *input_state);
void clearance_state_update(GAME_STATE *game_state);
void play_state_update(GAME_STATE *game_state, const INPUT_STATE *input_state);
void update(GAME_STATE *game_state, const INPUT_STATE *input_state);

// game matrix functions (DATA STRUCTURE - 1)
u8 get_matrix(const u8 *matrix, i32 width, i32 row, i32 col);
void set_matrix(u8 *matrix, i32 width, i32 row, i32 col, u8 value);
bool is_row_filled(const u8 *matrix, i32 width, i32 row);
bool is_row_empty(const u8 *matrix, i32 width, i32 row);
i32 get_lines(const u8 *matrix, i32 width, i32 height, u8 *lines_state);
void clear_lines(u8 *matrix, i32 width, i32 height, const u8 *lines);

#endif
