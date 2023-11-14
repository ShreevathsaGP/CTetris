#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>

#define CLEAR_STRUCT(x) memset(&(x), 0, sizeof(x))

// all types used
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef float f32;
typedef double f64;

// game dims
#define WIDTH 10
#define HEIGHT 22

// graphics
#define FPS 30
#define FRAME_DELAY (1e3/FPS)

#define PADDING_Y 5
#define PADDING_X 5

#define WINDOW_HEIGHT 835
#define UPPER_BUFFER (0.25 * WINDOW_HEIGHT) - PADDING_Y
#define FRACTION 0.55
#define CELL_SIZE ((WINDOW_HEIGHT - UPPER_BUFFER) / HEIGHT)
#define WINDOW_WIDTH (CELL_SIZE * WIDTH) + (PADDING_X * 4)

// gamplay
#define TETROMINO_MOD 1000 // ms

// priority queue
#define MAX_PQ_SIZE 7

#endif
