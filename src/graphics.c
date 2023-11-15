#include "graphics.h"

const SDL_Color BASE_COLOURS[] = {
    { 0x28, 0x28, 0x28, 0xFF },
    { 0x2D, 0x99, 0x99, 0xFF },
    { 0x99, 0x99, 0x2D, 0xFF },
    { 0x99, 0x2D, 0x99, 0xFF },
    { 0x2D, 0x99, 0x51, 0xFF },
    { 0x99, 0x2D, 0x2D, 0xFF },
    { 0x2D, 0x63, 0x99, 0xFF },
    { 0x99, 0x63, 0x2D, 0xFF }
};

const SDL_Color LIGHT_COLOURS[] = {
    { 0x28, 0x28, 0x28, 0xFF },
    { 0x44, 0xE5, 0xE5, 0xFF },
    { 0xE5, 0xE5, 0x44, 0xFF },
    { 0xE5, 0x44, 0xE5, 0xFF },
    { 0x44, 0xE5, 0x7A, 0xFF },
    { 0xE5, 0x44, 0x44, 0xFF },
    { 0x44, 0x95, 0xE5, 0xFF },
    { 0xE5, 0x95, 0x44, 0xFF }
};

const SDL_Color DARK_COLOURS[] = {
    { 0x28, 0x28, 0x28, 0xFF },
    { 0x1E, 0x66, 0x66, 0xFF },
    { 0x66, 0x66, 0x1E, 0xFF },
    { 0x66, 0x1E, 0x66, 0xFF },
    { 0x1E, 0x66, 0x36, 0xFF },
    { 0x66, 0x1E, 0x1E, 0xFF },
    { 0x1E, 0x42, 0x66, 0xFF },
    { 0x66, 0x42, 0x1E, 0xFF }
};

const SDL_Color BOX_COLOUR = {52, 54, 54, 255};
const SDL_Color CLEAR_COLOUR = {245, 244, 235, 255};
const SDL_Color TEXT_COLOUR = {255, 255, 255, 255};
const SDL_Color TRUE_BLACK = {0, 0, 0, 255};

void fill_rect(SDL_Renderer *renderer, i32 x, i32 y, i32 width, i32 height, SDL_Color colour) {
    SDL_Rect rect; rect.x = x; rect.y = y; rect.w = width; rect.h = height;

    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderFillRect(renderer, &rect);
}

void outline_rect(SDL_Renderer *renderer, i32 x, i32 y, i32 width, i32 height, SDL_Color colour) {
    SDL_Rect rect; rect.x = x; rect.y = y; rect.w = width; rect.h = height;

    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderDrawRect(renderer, &rect);
}

void render_string(SDL_Renderer *renderer, TTF_Font *font, const char *text, i32 x, i32 y, TEXT_ALIGN alignment, SDL_Color colour) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, colour);
    if (!surface) { printf("SURFACE NOT CREATED\n"); }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) { printf("TEXTURE NOT CREATED\n"); }

    SDL_Rect rect; rect.y = y; rect.w = surface -> w; rect.h = surface -> h;

    switch (alignment) {
        case TA_LEFT:
            rect.x = x;
            break;
        case TA_CENTER:
            rect.x = x - surface->w / 2;
            break;
        case TA_RIGHT:
            rect.x = x - surface->w;
            break;
    }

    SDL_RenderCopy(renderer, texture, 0, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_tetromino_cell(SDL_Renderer *renderer, i32 row, i32 col, u8 type, i32 x_offset, i32 y_offset, bool outline) {
    SDL_Color base_color = BASE_COLOURS[type];
    SDL_Color light_color = LIGHT_COLOURS[type];
    SDL_Color dark_color = DARK_COLOURS[type];

    i32 edge = CELL_SIZE / 8;
    i32 x = col * CELL_SIZE + x_offset;
    i32 y = row * CELL_SIZE + y_offset;

    if (outline) { outline_rect(renderer, x, y, CELL_SIZE, CELL_SIZE, base_color); return; }

    fill_rect(renderer, x, y, CELL_SIZE, CELL_SIZE, dark_color);
    fill_rect(renderer, x + edge, y, CELL_SIZE - edge, CELL_SIZE - edge, light_color);
    fill_rect(renderer, x + edge, y + edge, CELL_SIZE - edge * 2, CELL_SIZE - edge * 2, base_color);
}

void draw_tetromino(SDL_Renderer *renderer, const TETROMINO_STATE *tet_state, i32 x_offset, i32 y_offset, bool outline) {
    const TETROMINO *tetromino = TETROMINOES + tet_state -> index;

    for (i32 row = 0; row < tetromino ->side; ++row) { 

        for (i32 col = 0; col < tetromino ->side; ++col) {
            u8 type = get_tetromino(tetromino, row, col, tet_state->rotation);

            if (type > 0) {
                draw_tetromino_cell(renderer, row + tet_state -> row, col + tet_state -> col, type, x_offset, y_offset, outline);
            }
        }
    }
}
void draw_preview_tetromino(SDL_Renderer *renderer, i32 index) {
    SDL_Color base_color = BASE_COLOURS[index + 1];
    SDL_Color light_color = LIGHT_COLOURS[index + 1];
    SDL_Color dark_color = DARK_COLOURS[index + 1];

    index = 0;

    // default [side = 3]
    i32 cell_size = 25;
    i32 edge = cell_size / 8;

    i32 x, y;

    i32 x_start = (int)(WINDOW_WIDTH) / 2 + (5.7 * PADDING_X);
    i32 y_start = (FRACTION * UPPER_BUFFER) + ( ((1 - FRACTION) * UPPER_BUFFER)  / 2) - (1.8 * PADDING_Y);

    switch (index) {
        // I [side = 4]
        case 0:
            cell_size = 25;
            edge = cell_size / 8;
            x = x_start; y = y_start;

            for (int i = 0; i < 4; i++) {
                fill_rect(renderer, x, y, cell_size, cell_size, dark_color);
                fill_rect(renderer, x + edge, y, cell_size - edge, cell_size - edge, light_color);
                fill_rect(renderer, x + edge, y + edge, cell_size - edge * 2, cell_size - edge * 2, base_color);

                x += cell_size;
            }

            break;

        // O [side = 2]
        case 3:
            x_start += cell_size;
            y_start -= (cell_size / 2);

            x = x_start; y = y_start;

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    fill_rect(renderer, x, y, cell_size, cell_size, dark_color);
                    fill_rect(renderer, x + edge, y, cell_size - edge, cell_size - edge, light_color);
                    fill_rect(renderer, x + edge, y + edge, cell_size - edge * 2, cell_size - edge * 2, base_color);
                    x += cell_size;
                }

                x = x_start;
                y += cell_size;
            }

            break;

        // default [side = 3] (considering upper matrix 2x3)
        default:
            x_start += (cell_size / 2);
            y_start -= (cell_size / 2);

            x = x_start; y = y_start;

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    if (TETROMINOES[index].matrix[(3 * i) + j]) {
                        fill_rect(renderer, x, y, cell_size, cell_size, dark_color);
                        fill_rect(renderer, x + edge, y, cell_size - edge, cell_size - edge, light_color);
                        fill_rect(renderer, x + edge, y + edge, cell_size - edge * 2, cell_size - edge * 2, base_color);
                    }

                    x += cell_size;
                }
                x = x_start;
                y += cell_size;
            }
    }
}

void draw_matrix(SDL_Renderer *renderer, const u8 *matrix, i32 width, i32 height, i32 x_offset, i32 y_offset, bool game_over) {
    fill_rect(renderer, x_offset, y_offset, width * CELL_SIZE, height * CELL_SIZE, TRUE_BLACK);

    for (i32 row = 0; row < height; ++row) {
        for (i32 col = 0; col < width; ++col) {
            u8 type = get_matrix(matrix, width, row, col);
            if (type) { draw_tetromino_cell(renderer, row, col, type, x_offset, y_offset, false); }
        }
    }
}

void render(const GAME_STATE *game_state, SDL_Renderer *renderer, TTF_Font *game_font, TTF_Font *upper_font, TTF_Font *lower_font) {
    printf("RENDERING 1\n");
    char buffer[4096];

    draw_matrix(renderer, game_state->matrix, WIDTH, HEIGHT, 2 * PADDING_X, UPPER_BUFFER + PADDING_Y, game_state -> current_phase == GP_GAMEOVER);

    if (game_state -> current_phase == GP_PLAY) {
        draw_tetromino(renderer, &game_state->current_tetromino, 2 * PADDING_X, UPPER_BUFFER + PADDING_Y, false);

        // show shadow for the tetromino
        TETROMINO_STATE tetromino = game_state->current_tetromino;
        while (is_tetromino_valid(&tetromino, game_state->matrix, WIDTH, HEIGHT)) { tetromino.row++; }
        --tetromino.row;

        if (tetromino.row >= 0) {
            draw_tetromino(renderer, &tetromino, 2 * PADDING_X, UPPER_BUFFER + PADDING_Y, true);
        }

        // preview the next tetromino
        if (!is_empty(game_state -> priority_queue)) { 
            draw_preview_tetromino(renderer, peek(game_state -> priority_queue));
        }

    }

    if (game_state-> current_phase == GP_LINE_CLEARANCE) {
        for (i32 row = 0; row < HEIGHT; ++row) {
            if (game_state->lines[row]) {
                i32 x = 0; i32 y = row * CELL_SIZE + UPPER_BUFFER + PADDING_Y;
                fill_rect(renderer, 2 * PADDING_X, y, WIDTH * CELL_SIZE, CELL_SIZE, CLEAR_COLOUR);
            }
        }

    } else if (game_state->current_phase == GP_GAMEOVER) {
        i32 x = (int)(WINDOW_WIDTH) / 2;
        i32 y = UPPER_BUFFER + ( (CELL_SIZE * HEIGHT) / 2 );
        render_string(renderer, upper_font, "GAME OVER", x, y, TA_CENTER, TEXT_COLOUR);

    } else if (game_state->current_phase == GP_START) {
        i32 x = (int)(WINDOW_WIDTH) / 2;
        i32 y = UPPER_BUFFER + ( (CELL_SIZE * HEIGHT) / 2 );
        render_string(renderer, game_font, "PRESS SPACE", x, y - 17, TA_CENTER, TEXT_COLOUR);
        snprintf(buffer, sizeof(buffer), "STARTING LEVEL: %02d", game_state->starting_level);
        render_string(renderer, game_font, buffer, x, y + 15, TA_CENTER, TEXT_COLOUR);
    }

    fill_rect(renderer, 2 * PADDING_X, 2 * PADDING_Y, WIDTH * CELL_SIZE, (FRACTION * UPPER_BUFFER) - (2 * PADDING_Y), TRUE_BLACK);

    snprintf(buffer, sizeof(buffer), "SCORE: %06d", game_state->no_points);
    render_string(renderer, upper_font, buffer, 4 * PADDING_X, 4.5 * PADDING_Y, TA_LEFT, TEXT_COLOUR);

    snprintf(buffer, sizeof(buffer), "LINES: %03d", game_state->no_lines);
    render_string(renderer, upper_font, buffer, WINDOW_WIDTH - (4 * PADDING_X), (FRACTION * UPPER_BUFFER) / 1.55, TA_RIGHT, TEXT_COLOUR);

    snprintf(buffer, sizeof(buffer), "LEVEL: %02d", game_state->current_level);
    render_string(renderer, lower_font, buffer, 4.8 * PADDING_X, 
            (FRACTION * UPPER_BUFFER) + (((1 - FRACTION) * UPPER_BUFFER) / 2) - (1.8 * PADDING_Y), 
            TA_LEFT, TEXT_COLOUR);
}
