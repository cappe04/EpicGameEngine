#ifndef _RENDERER_H
#define _RENDERER_H

#include <stdint.h>

#include "_math.h"
#include "_graphics.h"

typedef struct s_Vertex {
    Vec3 real_pos, world_pos;
    Color color;
} Vertex;

typedef struct s_Trig {
    Vertex vertices[3];
    Vec3 normal;
    float depth;
} Trig;

typedef struct s_Mesh {
    Trig* trigs;
    unsigned int count;
} Mesh;

Vertex new_vertex(Vec3 real_pos, Color color);
void update_vertex(Vertex* vertex, Vec3 world_pos);

void render_point(uint32_t *pixels, Color color, Vec3 point);
void render_line(uint32_t *pixels, Color color, Vec3 p1, Vec3 p2);
void render_triangle(uint32_t *pixels, float *z_buffer, Color color, Vec3 p1, Vec3 p2, Vec3 p3);

#endif // _RENDERER_H