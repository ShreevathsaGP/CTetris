#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "game.h"
#include "SDL2/SDL.h"
#include "graphics.h"
#include "constants.h"

int main() {
    //--TESTING--------------------------------------------------------------------------------------------------------------------

    //--TESTING--------------------------------------------------------------------------------------------------------------------

    //--INITIALISE-----------------------------------------------------------------------------------------------------------------
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { return 1; }
    if (TTF_Init() < 0) { return 2; }
    SDL_Window *window = SDL_CreateWindow(
            "Tetris Classic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            WINDOW_WIDTH, WINDOW_HEIGHT + (2 * PADDING_Y), SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    srand(time(NULL));

    const char *font_path = "build/novem___.ttf";
    TTF_Font *game_font = TTF_OpenFont(font_path, 28);
    TTF_Font *upper_font = TTF_OpenFont(font_path, 32);
    TTF_Font *lower_font = TTF_OpenFont(font_path, 26);

    if (!game_font || !upper_font || !lower_font) { printf("font aint there brev!\n"); }

    /* GAME_STATE game_state; */
    /* INPUT_STATE input_state; */

    /* CLEAR_STRUCT(game_state); */
    /* CLEAR_STRUCT(input_state); */

    GAME_STATE *game_state = calloc(1, sizeof(GAME_STATE));
    INPUT_STATE *input_state = calloc(1, sizeof(INPUT_STATE));

    // initial order of tetrominoes (will change)
    u8 priority_order[7];
    for (int i = 0; i < 7; i++) { priority_order[i] = i; }
    shuffle(priority_order, 7);

    game_state -> priority_queue = newPQ(7);
    for (int i = 0; i < 7; i++) {
        enqueue(game_state -> priority_queue, priority_order[i], i);
    }

    generate_tetromino(game_state);

    //--INITIALISE-----------------------------------------------------------------------------------------------------------------

    bool running = true;
    u32 previous_time; // ms
    u32 elapsed_time; // ms

    printf("frame_delay = %f\n", FRAME_DELAY);
    previous_time = SDL_GetTicks();

    //--BOXES---------------------------------------------------------------------------------------------------------------------

    u16 box_width = WINDOW_WIDTH - (2 * PADDING_X);
    SDL_Rect board_rect, upper_rect, lower_rect_left, lower_rect_right;

    board_rect.x = PADDING_X; board_rect.y = UPPER_BUFFER; 
    board_rect.w = box_width; board_rect.h = (CELL_SIZE * HEIGHT) + (2 * PADDING_Y) + 2;

    upper_rect.x = PADDING_X; upper_rect.y = PADDING_Y; 
    upper_rect.w = box_width; upper_rect.h = FRACTION * UPPER_BUFFER;

    lower_rect_left.x = PADDING_X; lower_rect_left.y = upper_rect.h + (2 * PADDING_Y);
    lower_rect_left.w = (box_width / 2) - PADDING_X;
    lower_rect_left.h = ((1 - FRACTION) * UPPER_BUFFER) - (2 * PADDING_Y);

    lower_rect_right.x = lower_rect_left.x + lower_rect_left.w + PADDING_X;
    lower_rect_right.y = upper_rect.h + (2 * PADDING_Y);
    lower_rect_right.w = box_width / 2; 
    lower_rect_right.h = ((1 - FRACTION) * UPPER_BUFFER) - (2 * PADDING_Y);


    //--BOXES---------------------------------------------------------------------------------------------------------------------

    //--GAME-LOOP-----------------------------------------------------------------------------------------------------------------
    while (running) {
        game_state -> current_time = SDL_GetTicks();

        //--EVENT-HANDLING--------------------------------------------------------------------------------------------------------
        // poll events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                    }
            }
        }

        i32 keyboard_count;
        const u8 *keyboard_state = SDL_GetKeyboardState(&keyboard_count);

        // prevents holding (mimics the old consoles)
        INPUT_STATE previous_input = *input_state;

        input_state -> left = keyboard_state[SDL_SCANCODE_LEFT];
        input_state -> right = keyboard_state[SDL_SCANCODE_RIGHT];
        input_state -> up = keyboard_state[SDL_SCANCODE_UP];
        input_state -> down = keyboard_state[SDL_SCANCODE_DOWN];
        input_state -> a = keyboard_state[SDL_SCANCODE_SPACE];

        input_state -> difference_left = (i8)input_state -> left - (i8)previous_input.left;
        input_state -> difference_right = (i8)input_state -> right - (i8)previous_input.right;
        input_state -> difference_up = (i8)input_state -> up - (i8)previous_input.up;
        input_state -> difference_down = (i8)input_state -> down - (i8)previous_input.down;
        input_state -> difference_a = (i8)input_state -> a - (i8)previous_input.a;
        //--EVENT-HANDLING----------------------------------------------------------------------------------------------------------

        // clear the window to black
        SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(renderer);

        //--UPDATE------------------------------------------------------------------------------------------------------------------
        update(game_state, input_state);
        //--UPDATE------------------------------------------------------------------------------------------------------------------

        //--RENDER------------------------------------------------------------------------------------------------------------------

        // outer boxes
        SDL_SetRenderDrawColor(renderer, BOX_COLOUR.r, BOX_COLOUR.g, BOX_COLOUR.b, BOX_COLOUR.a);
        SDL_RenderFillRect(renderer, &board_rect);
        SDL_RenderFillRect(renderer, &upper_rect);
        SDL_RenderFillRect(renderer, &lower_rect_left);
        SDL_RenderFillRect(renderer, &lower_rect_right);

        // inner box
        fill_rect(renderer, 2 * PADDING_X, upper_rect.h + (3 * PADDING_Y), 
                lower_rect_left.w - (2 * PADDING_X), lower_rect_left.h - (2 * PADDING_X),
                TRUE_BLACK);

        fill_rect(renderer, lower_rect_right.x + PADDING_X, upper_rect.h + (3 * PADDING_Y), 
                lower_rect_right.w - (2 * PADDING_X), lower_rect_right.h - (2 * PADDING_X),
                TRUE_BLACK);

        render(game_state, renderer, game_font, upper_font, lower_font);

        SDL_RenderPresent(renderer);

        elapsed_time = SDL_GetTicks() - previous_time;
        if (elapsed_time < FRAME_DELAY) { SDL_Delay(FRAME_DELAY - elapsed_time); }
        printf("fps = %f\n", 1e3 / (SDL_GetTicks() - previous_time));

        previous_time = SDL_GetTicks();
    }
    //--RENDER----------------------------------------------------------------------------------------------------------------------

    //--GAME-LOOP-------------------------------------------------------------------------------------------------------------------

    //--CLEANUP---------------------------------------------------------------------------------------------------------------------
    free(game_state); free(input_state);
    TTF_CloseFont(game_font);
    TTF_CloseFont(upper_font);
    TTF_CloseFont(lower_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    //--CLEANUP---------------------------------------------------------------------------------------------------------------------
}
