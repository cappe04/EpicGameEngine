#ifndef _MATH_H
#define _MATH_H

#define PI 3.14159265359
#define PI_2 1.57079632679

#define MIN(a, b) (a <= b ? a: b)
#define MAX(a, b) (a >= b ? a: b)

typedef struct s_Vec3{
    float x, y, z;
} Vec3;

typedef struct s_Vec2i{
    int x, y, z;
} Vec2i;

void sortf3(float n[3]);

Vec2i vec3_flatten(Vec3 vec);

Vec3 vec3_rotatex(Vec3 vec, float t);
Vec3 vec3_rotatey(Vec3 vec, float t);
Vec3 vec3_rotatez(Vec3 vec, float t);

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);

Vec3 vec3_scalmul(Vec3 vec, float scalar);
Vec3 vec3_scaldiv(Vec3 vec, float scalar);
float vec3_dot(Vec3 a, Vec3 b);

#endif // _MATH_H