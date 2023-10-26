#include "_graphics.h"

#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include "_globals.h"
#include "_math.h"

uint32_t rgba_uint32(int r, int g, int b, int a){
    // x & 255 limit to 255
    return ((a & 255) << 24) + ((b & 255) << 16) + ((g & 255) << 8) + (r & 255);
}

uint32_t color_uint32(Color color){
    return rgba_uint32(color.r, color.g, color.b, color.a);
}

Color new_color(int r, int g, int b, int a){
    return (Color){ r, g, b, a };
}


void set_pixel(uint32_t *pixels, Color color, int x, int y){
    if(x >= 0 && x < DISPLAY_W-1 && y >= 0 && y < DISPLAY_H-1){
        pixels[(y * DISPLAY_W) + x] = color_uint32(color);
    }
}

void draw_line(uint32_t *pixels, Color color, int x1, int y1, int x2, int y2){

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = x1 < x2 ? 1: -1;
    int sy = y1 < y2 ? 1: -1;
    int err = dx - dy;
    
    int e2;

    while (1) {
        set_pixel(pixels, color, x1, y1);

        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_triangle(uint32_t *pixels, Color color, int x1, int y1, int x2, int y2, int x3, int y3){
    Vec2i p[3] = {
        { x1, y1 },
        { x2, y2 },
        { x3, y3 }, 
    };

    // Sort by Y-value (ascending)
    {
        Vec2i tmp;
        if(p[0].y > p[1].y){
            tmp = p[0];
            p[0] = p[1];
            p[1] = tmp;
        }
        if(p[1].y > p[2].y){
            tmp = p[1];
            p[1] = p[2];
            p[2] = tmp;
        }
        if(p[0].y > p[1].y){
            tmp = p[0];
            p[0] = p[1];
            p[1] = tmp;
        }
    }

    // X-values is given by "x(y) = ky + m" where 
    // k = (x_end - x_start) / (y_end - y_start)
    // m = x_start
    float k01 = (float)(p[1].x - p[0].x) / (float)(p[1].y - p[0].y);
    int m01 = p[0].x - k01 * p[0].y;
    float k02 = (float)(p[2].x - p[0].x) / (float)(p[2].y - p[0].y);
    int m02 = p[0].x - k02 * p[0].y;
    float k12 = (float)(p[2].x - p[1].x) / (float)(p[2].y - p[1].y);
    int m12 = p[1].x - k12 * p[1].y;


    int y_start = MIN(MAX(p[0].y + 1, 0), DISPLAY_H);
    int y_mid = MIN(MAX(p[1].y, 0), DISPLAY_H);
    int y_end = MIN(MAX(p[2].y, 0), DISPLAY_H);

    // Scanline fill
    for(int y = y_start; y<y_mid; y++){
        int x01 = k01 * y + m01;
        int x02 = k02 * y + m02;

        if(x01 < x02){
            for(int x = x01; x<x02; x++){
                set_pixel(pixels, color, x, y);
            }
        }
        else {
            for(int x = x02; x<x01; x++){
                set_pixel(pixels, color, x, y);
            }
        }
    }

    for(int y = y_mid; y<y_end; y++){
        int x12 = k12 * y + m12;
        int x02 = k02 * y + m02;

        if(x12 < x02){
            for(int x = x12; x<x02; x++){
                set_pixel(pixels, color, x, y);
            }
        }
        else {
            for(int x = x02; x<x12; x++){
                set_pixel(pixels, color, x, y);
            }
        }
    }
}
