#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <stdint.h>

#include "_math.h"

typedef struct s_Color {
    int r, g, b, a;
} Color;

uint32_t rgba_uint32(int r, int g, int b, int a);
uint32_t color_uint32(Color color);

Color new_color(int r, int g, int b, int a);

void set_pixel(uint32_t* pixels, Color color, int x, int y);
void draw_line(uint32_t* pixels, Color color, int x1, int y1, int x2, int y2);
void draw_aaline(uint32_t *pixels, Color color, int x1, int y1, int x2, int y2);
void draw_triangle(uint32_t *pixels, Color color, int x1, int y1, int x2, int y2, int x3, int y3);

#endif // _GRAPHICS_H