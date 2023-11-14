#include "game.h"

void shuffle(u8 *array, int size) {
    int j; u8 temp;
    for (int i = 0; i < size; i++) {
        // generate random index in range [0-i]
        j = rand() % (i + 1);
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


// priority queue using arrays [DATA STRUCTURE - 1]
PQ *newPQ(i16 capacity) {
    PQ *p = (PQ *)malloc(sizeof(PQ));
    p -> array = (PQ_NODE *)calloc(capacity, sizeof(PQ_NODE));
    p -> capacity = capacity;
    p -> size = 0;

    return p;
}

void enqueue(PQ *p, u8 data, u8 priority) {
    if (p -> size == p -> capacity) { printf("PQ Full!\n"); return; }

    // starting from the right, shift new node left
    // while maintaining ascending order in array
    int index = p -> size - 1;
    while (index >= 0 && p -> array[index].priority > priority) {
        p -> array[index + 1] = p -> array[index];
        index -= 1;
    }

    p -> array[index + 1].data = data;
    p -> array[index + 1].priority = priority;
    p -> size += 1;
}

bool is_empty(PQ *p) { return p -> size == 0; }

u8 peek(PQ *p) {
    if (p -> size == 0) { return 255; }
    return p -> array[0].data;
}


u8 dequeue(PQ *p) {
    if (p -> size == 0) { return 255; }

    PQ_NODE node = p -> array[0];

    // shift entire array one index left
    for (int i = 1; i < p -> size; i++) { p -> array[i - 1] = p -> array[i]; }
    p -> size -= 1;

    return node.data;
}

void printPQ(PQ *p) {
    printf("PQ: [ ");
    for (int i = 0; i < p -> size; i++) {
        printf("(%d, %d) ", p -> array[i].data , p -> array[i].priority);
    }
    printf("]\n");
}

// helper functions
i32 min(i32 x, i32 y) { return (x > y) ? y : x; }
i32 max(i32 x, i32 y) { return (x < y) ? y : x; }
i32 randrange(i32 min, i32 max) { return min + rand() % (max - min); }

u8 get_matrix(const u8 *matrix, i32 width, i32 row, i32 col) {
    i32 index = row * width + col;
    return matrix[index];
}

void set_matrix(u8 *matrix, i32 width, i32 row, i32 col, u8 value) {
    i32 index = row * width + col;
    matrix[index] = value;
}

bool is_row_filled(const u8 *matrix, i32 width, i32 row) {
    for (i32 col = 0; col < width; ++col) {
        if (get_matrix(matrix, width, row, col) == 0) { return false; }
    }
    return true;
}

bool is_row_empty(const u8 *matrix, i32 width, i32 row) {
    for (i32 col = 0; col < width; ++col) {
        if (get_matrix(matrix, width, row, col) > 0) { return false; }
    }
    return true;
}

i32 get_lines(const u8 *matrix, i32 width, i32 height, u8 *lines_state) {
    i32 count = 0;
    for (i32 row = 0; row < height; ++row) {
        u8 filled = is_row_filled(matrix, width, row);
        lines_state[row] = filled;
        count += filled;
    }
    return count;
}

void clear_lines(u8 *matrix, i32 width, i32 height, const u8 *lines) {
    i32 source_row = height - 1;

    // iterate over destination row 
    for (i32 destination_row = height - 1; destination_row >= 0; --destination_row) {
        while (source_row >= 0 && lines[source_row]) { source_row -= 1; }

        if (source_row < 0) { 
            memset(matrix + destination_row * width, 0, width); 
        } else {
            if (source_row != destination_row) {
                memcpy(matrix + destination_row * width, matrix + source_row * width, width);
            }
            source_row -= 1;
        }
    }
}

u8 get_tetromino(const TETROMINO *tetromino, i32 row, i32 col, i32 rotation) {
    i32 side = tetromino -> side;

    switch (rotation) {
        case 0:
            return tetromino -> matrix[row * side + col];
        case 1:
            return tetromino -> matrix[(side - col - 1) * side + row];
        case 2:
            return tetromino -> matrix[(side - row - 1) * side + (side - col - 1)];
        case 3:
            return tetromino -> matrix[col * side + (side - row - 1)];
    }

    return 0;
}

bool is_tetromino_valid(const TETROMINO_STATE *tet, const u8 *matrix, i32 width, i32 height) {
    const TETROMINO *tetromino = &TETROMINOES[tet -> index];

    for (i32 row = 0; row < tetromino -> side; ++row) {

        for (i32 col = 0; col < tetromino -> side; ++col) {
            u8 value = get_tetromino(tetromino, row, col, tet -> rotation);

            if (value > 0) {
                i32 matrix_row = tet -> row + row;
                i32 matrix_col = tet -> col + col;

                if (matrix_row < 0) { return false; }
                if (matrix_row >= height) { return false; }
                if (matrix_col < 0) { return false; }
                if (matrix_col >= width) { return false; }
                if (get_matrix(matrix, width, matrix_row, matrix_col)) { return false; }
            }
        }

    }
    return true;
}

void cement_tetromino(GAME_STATE *game_state) {
    const TETROMINO *tetromino = &TETROMINOES[game_state -> current_tetromino.index];

    for (i32 row = 0; row < tetromino -> side; ++row) {
        for (i32 col = 0; col < tetromino -> side; ++col) {
            u8 value;
            value = get_tetromino(tetromino, row, col, game_state -> current_tetromino.rotation);

            if (value > 0) {
                i32 matrix_row = game_state -> current_tetromino.row + row;
                i32 matrix_col = game_state -> current_tetromino.col + col;
                
                if (!get_matrix(game_state -> matrix, WIDTH, matrix_row, matrix_col)) {
                    set_matrix(game_state -> matrix, WIDTH, matrix_row, matrix_col, value);
                }

            }
        }

    }
}

u32 next_drop_time(i32 level) { return WAIT_PER_DROP[min(level, 29)]; }

void generate_tetromino(GAME_STATE *game_state) {
    game_state -> current_tetromino.index = dequeue(game_state -> priority_queue);
    game_state -> current_tetromino.col = WIDTH / 2;
    game_state -> current_tetromino.row = 0;
    game_state -> current_tetromino.rotation = 0;

    // update the drop time of the next tetromino
    game_state -> drop_time = game_state -> current_time + next_drop_time(game_state -> current_level);

    // ensure that PQ is not empty
    if (is_empty(game_state -> priority_queue)) {
        // get randomly generated order of tetrominoes
        u8 priority_order[7] = { 0, 1, 2, 3, 4, 5, 6 };
        shuffle(priority_order, 7);

        // add this order into the PQ
        for (int i = 0; i < 7; i++) { 
            enqueue(game_state -> priority_queue, priority_order[i], i);
        }
    }
}

bool soft_drop(GAME_STATE *game_state) {
    game_state -> current_tetromino.row += 1;

    if (!is_tetromino_valid(&game_state -> current_tetromino, game_state -> matrix, WIDTH, HEIGHT))
    {
        game_state -> current_tetromino.row -= 1;
        cement_tetromino(game_state);
        generate_tetromino(game_state);
        return false;
    }

    game_state -> drop_time = game_state -> current_time + next_drop_time(game_state -> current_level);

    return true;
}

i32 get_points(i32 current_level, i32 no_lines) {
    switch (no_lines) {
        case 1:
            return 40 * (current_level + 1);
        case 2:
            return 100 * (current_level + 1);
        case 3:
            return 300 * (current_level + 1);
        case 4:
            return 1200 * (current_level + 1);
    }
    return 0;
}

i32 lines_needed(i32 start_level, i32 current_level) {
    // first level up limit
    i32 flu_limit = min( (start_level * 10 + 10), max(100, (start_level * 10 - 50)));
    if (current_level == start_level) { return flu_limit; }
    i32 difference = current_level - start_level;
    return flu_limit + difference * 10;
}

void start_state_update(GAME_STATE *game_state, const INPUT_STATE *input_state) {
    if (input_state -> difference_up > 0 && game_state -> starting_level < 29) { game_state -> starting_level += 1; }
    if (input_state -> difference_down > 0 && game_state -> starting_level > 0) {
        game_state -> starting_level -= 1; 
    }

    if (input_state -> difference_a > 0) {
        memset(game_state -> matrix, 0, WIDTH * HEIGHT);
        game_state -> current_level = game_state -> starting_level;
        game_state -> no_lines = 0;
        game_state -> no_points = 0;
        generate_tetromino(game_state);
        game_state -> current_phase = GP_PLAY;
    }
}

void gameover_state_update(GAME_STATE *game_state, const INPUT_STATE *input_state) {
    if (input_state -> difference_a > 0) { game_state -> current_phase = GP_START; }
}

void clearance_state_update(GAME_STATE *game_state) {
    if (game_state -> current_time >= game_state -> highlight_end) {
        clear_lines(game_state -> matrix, WIDTH, HEIGHT, game_state -> lines);
        game_state -> no_lines += game_state -> remaining_lines;
        game_state -> no_points += get_points(game_state -> current_level, game_state -> remaining_lines);

        i32 next_level_requirement = lines_needed(game_state -> starting_level, game_state -> current_level);
        if (game_state -> no_lines >= next_level_requirement) { game_state -> current_level += 1; }
        game_state -> current_phase = GP_PLAY;
    }
}

void play_state_update(GAME_STATE *game_state, const INPUT_STATE *input_state) {
    i32 death_row = 0;
    if (!is_row_empty(game_state->matrix, WIDTH, death_row)) {
        game_state->current_phase = GP_GAMEOVER;
    }

    TETROMINO_STATE tetromino = game_state -> current_tetromino;
    if (input_state -> difference_left > 0) { --tetromino.col; }
    if (input_state->difference_right> 0) { ++tetromino.col; }
    if (input_state->difference_up > 0) { tetromino.rotation = (tetromino.rotation + 1) % 4; }
    if (is_tetromino_valid(&tetromino, game_state -> matrix, WIDTH, HEIGHT)) { game_state -> current_tetromino = tetromino; }
    if (input_state->difference_down > 0) { soft_drop(game_state); }
    if (input_state->difference_a > 0) { while(soft_drop(game_state)); }

    while (game_state->current_time >= game_state->drop_time) { 
        soft_drop(game_state); 
    }
    game_state->remaining_lines = get_lines(game_state->matrix, WIDTH, HEIGHT, game_state->lines);

    if (game_state->remaining_lines > 0) {
        game_state->current_phase = GP_LINE_CLEARANCE;
        game_state->highlight_end = game_state->current_time + 200;
    }
    
}

void update(GAME_STATE *game_state, const INPUT_STATE *input_state) {

    switch(game_state -> current_phase) {
        case GP_START:
            start_state_update(game_state, input_state);
            break;
        case GP_PLAY:
            play_state_update(game_state, input_state);
            break;
        case GP_LINE_CLEARANCE:
            clearance_state_update(game_state);
            break;
        case GP_GAMEOVER:
            gameover_state_update(game_state, input_state);
            break;
    }
}

// tetrominoes
const u8 I[] = { 
    1, 1, 1, 1, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0 };
const u8 J[] = { 
    2, 2, 2, 
    0, 0, 2,
    0, 0, 0, };
const u8 L[] = { 
    3, 3, 3, 
    3, 0, 0,
    0, 0, 0 };
const u8 O[] = { 
    4, 4, 
    4, 4 };
const u8 S[] = { 
    0, 5, 5, 
    5, 5, 0,
    0, 0, 0, };
const u8 T[] = { 
    0, 6, 0, 
    6, 6, 6, 
    0, 0, 0 };
const u8 Z[] = { 
    7, 7, 0, 
    0, 7, 7,
    0, 0, 0, 
};

const TETROMINO TETROMINOES[7] = {
    {I, 4}, {J, 3}, {L, 3}, {O, 2}, {S, 3}, {T, 3}, {Z, 3}
};

// level wait times
const u16 WAIT_PER_DROP[] = {
    800,
    716,
    633,
    550,
    466,
    383,
    300,
    216,
    133,
    100,
    83,
    83,
    83,
    66,
    66,
    66,
    50,
    50,
    50,
    33,
    33,
    33,
    33,
    33,
    33,
    33,
    33,
    33,
    33,
    16
};
