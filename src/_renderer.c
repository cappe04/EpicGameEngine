#include "_renderer.h"

#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include "_globals.h"
#include "_math.h"
#include "_graphics.h"

static void set_depth(float* z_buffer, int x, int y, float depth){
    if(x >= 0 && x < DISPLAY_W-1 && y >= 0 && y < DISPLAY_H-1){
        z_buffer[(y * DISPLAY_W) + x] = depth;
    }
}

static float get_depth(float* z_buffer, int x, int y){
    if(x >= 0 && x < DISPLAY_W-1 && y >= 0 && y < DISPLAY_H-1){
        return z_buffer[(y * DISPLAY_W) + x];
    }
    return 0;
}

void render_point(uint32_t *pixels, Color color, Vec3 point){
    if(point.z < Z_CLIP) return;

    Vec2i pixel = vec3_flatten(point);
    set_pixel(pixels, color, pixel.x, pixel.y);
}

void render_line(uint32_t *pixels, Color color, Vec3 p1, Vec3 p2){
    
    if (p1.z < Z_CLIP && p2.z < Z_CLIP) return;

    if (p1.z < Z_CLIP){
        Vec3 len = vec3_sub(p2, p1);
        float t = (Z_CLIP - p1.z) / len.z;
        p1.x += t * len.x;
        p1.y += t * len.y;
        p1.z = Z_CLIP;
    }

    if (p2.z < Z_CLIP){
        Vec3 len = vec3_sub(p2, p1);
        float t = (Z_CLIP - p2.z) / len.z;
        p2.x += t * len.x;
        p2.y += t * len.y;
        p2.z = Z_CLIP;
    }

    Vec2i pixel1 = vec3_flatten(p1);
    Vec2i pixel2 = vec3_flatten(p2);

    draw_line(pixels, color, pixel1.x, pixel1.y, pixel2.x, pixel2.y);
}

static void fill_scanline(uint32_t *pixels, float *z_buffer, Color color,
                          int y, int x1, int x2, float z1, float z2,
                          float factor, float depth){ // LAST ROW TEMP

    for(int x = x1; x<x2; x++){
        float k = (float)(z2 - z1) / (float)(x2 - x1);
        float m = z1 - k * x1;
        float z = 1 / (k * x + m);
        if(get_depth(z_buffer, x, y) < z){
            set_pixel(pixels, color, x, y);
            set_depth(z_buffer, x, y, z);
        }

        // color.r = factor * (z - depth);
        // color.g = factor * (z - depth);
        // color.b = factor * (z - depth);
        // set_pixel(pixels, color, x, y);
    }
}

void render_triangle(uint32_t *pixels, float *z_buffer, Color color, Vec3 p1, Vec3 p2, Vec3 p3){
    // Z clipping
    if (p1.z < Z_CLIP || p2.z < Z_CLIP || p3.z < Z_CLIP){
        return;
    }

    // Flatten points to screen space
    Vec2i pf1 = vec3_flatten(p1);
    Vec2i pf2 = vec3_flatten(p2);
    Vec2i pf3 = vec3_flatten(p3);



    // Sort by Y-value (ascending)
    struct pixel { int x; int y; float z; };

    struct pixel p[3] = { 
        pf1.x, pf1.y, p1.z,
        pf2.x, pf2.y, p2.z, 
        pf3.x, pf3.y, p3.z
    };

    {
        struct pixel tmp;
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

    // Degug depth values, color = (z - depth[0]) * depth_factor
    float depth[3] = { p[0].z, p[1].z, p[2].z };
    sortf3(depth);
    float depth_factor = 255.0 / (depth[2] - depth[0]);

    // Linear interpolation 
    struct { float x_slope; int x_start; float z_slope; float z_start; } line01, line02, line12;

    line01.x_slope = (float)(p[1].x - p[0].x) / (float)(p[1].y - p[0].y);
    line01.x_start = p[0].x - line01.x_slope * p[0].y;
    line01.z_slope = (float)(p[1].z - p[0].z) / (float)(p[1].y - p[0].y);
    line01.z_start = p[0].z - line01.z_slope * p[0].y;

    line02.x_slope = (float)(p[2].x - p[0].x) / (float)(p[2].y - p[0].y);
    line02.x_start = p[0].x - line02.x_slope * p[0].y;
    line02.z_slope = (p[2].z - p[0].z) / (float)(p[2].y - p[0].y);
    line02.z_start = p[0].z - line02.z_slope * p[0].y;

    line12.x_slope = (float)(p[2].x - p[1].x) / (float)(p[2].y - p[1].y);
    line12.x_start = p[1].x - line12.x_slope * p[1].y;
    line12.z_slope = (float)(p[2].z - p[1].z) / (float)(p[2].y - p[1].y);
    line12.z_start = p[1].z - line12.z_slope * p[1].y;

    // Limit why values to screen
    int y_start = MIN(MAX(p[0].y + 1, 0), DISPLAY_H);
    int y_mid = MIN(MAX(p[1].y, 0), DISPLAY_H);
    int y_end = MIN(MAX(p[2].y, 0), DISPLAY_H);

    // Scanline fill
    for(int y = y_start; y<y_mid; y++){
        int x01 = line01.x_slope * y + line01.x_start;
        int x02 = line02.x_slope * y + line02.x_start;

        float z01 = line01.z_slope * y + line01.z_start;
        float z02 = line02.z_slope * y + line02.z_start;

        if(x01 < x02){
            fill_scanline(pixels, z_buffer, color, y, x01, x02, z01, z02, depth_factor, depth[0]);
        }
        else {
            fill_scanline(pixels, z_buffer, color, y, x02, x01, z01, z02, depth_factor, depth[0]);
        }
    }

    for(int y = y_mid; y<y_end; y++){
        int x12 = line12.x_slope * y + line12.x_start;
        int x02 = line02.x_slope * y + line02.x_start;

        float z12 = line12.z_slope * y + line12.z_start;
        float z02 = line02.z_slope * y + line02.z_start;

        if(x12 < x02){
            fill_scanline(pixels, z_buffer, color, y, x12, x02, z12, z02, depth_factor, depth[0]);
        }
        else {
            fill_scanline(pixels, z_buffer, color, y, x02, x12, z12, z02, depth_factor, depth[0]);
        }
    }
}
