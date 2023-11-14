#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"

#include "game.h"
#include "constants.h"

typedef enum TextAlign {
    TA_LEFT,
    TA_CENTER,
    TA_RIGHT
} TEXT_ALIGN;

// colours
extern const SDL_Color BASE_COLOURS[];
extern const SDL_Color LIGHT_COLOURS[];
extern const SDL_Color DARK_COLOURS[];
extern const SDL_Color BOX_COLOUR;
extern const SDL_Color CLEAR_COLOUR;
extern const SDL_Color TEXT_COLOUR;
extern const SDL_Color TRUE_BLACK;

void fill_rect(SDL_Renderer *renderer, i32 x, i32 y, i32 width, i32 height, SDL_Color colour);

void outline_rect(SDL_Renderer *renderer, i32 x, i32 y, i32 width, i32 height, SDL_Color colour);

void render_string(SDL_Renderer *renderer, TTF_Font *font, const char *text, i32 x, i32 y, TEXT_ALIGN alignment, SDL_Color colour);

void draw_tetromino_cell(SDL_Renderer *renderer, i32 row, i32 col, u8 type, i32 x_offset, i32 y_offset, bool outline);

void draw_tetromino(SDL_Renderer *renderer, const TETROMINO_STATE *tetromino, i32 x_offset, i32 y_offset, bool outline);

void draw_preview_tetromino(SDL_Renderer *renderer, i32 index);

void draw_matrix(SDL_Renderer *renderer, const u8 *matrix, i32 width, i32 height, i32 x_offset, i32 y_offset, bool game_over);

void render(const GAME_STATE *game_state, SDL_Renderer *renderer, TTF_Font *game_font, TTF_Font *upper_font, TTF_Font *lower_font);

#endif

