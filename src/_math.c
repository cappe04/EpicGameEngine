#include "_math.h"

#include <math.h>

#include "_globals.h"

void sortf3(float n[3]){
    float tmp;
    if(n[0] > n[1]){
        tmp = n[0];
        n[0] = n[1];
        n[1] = tmp;
    }
    if(n[1] > n[2]){
        tmp = n[1];
        n[1] = n[2];
        n[2] = tmp;
    }
    if(n[0] > n[1]){
        tmp = n[0];
        n[0] = n[1];
        n[1] = tmp;
    }
}

Vec2i vec3_flatten(Vec3 vec)
{
    int offset_x = DISPLAY_W / 2;
    int offset_y = DISPLAY_H / 2;

    if(vec.z == 0) vec.z = 1;

    return (Vec2i){ vec.x * K1 / (vec.z) + offset_x, vec.y * K1 / (vec.z) + offset_y };
}

Vec3 vec3_rotatex(Vec3 vec, float t){
    float cost = cosf(t);
    float sint = sinf(t);
    return (Vec3){
        vec.x,                          // x
        cost * vec.y - sint * vec.z,    // y
        sint * vec.y + cost * vec.z,    // z
    };
}
Vec3 vec3_rotatey(Vec3 vec, float t){
    float cost = cosf(t);
    float sint = sinf(t);
    return (Vec3){
        cost * vec.x + sint * vec.z,    // x
        vec.y,                          // y
        -sint * vec.x + cost * vec.z,   // z
    };
   
}
Vec3 vec3_rotatez(Vec3 vec, float t){
    float cost = cosf(t);
    float sint = sinf(t);
    return (Vec3){
        cost * vec.x - sint * vec.y,    // x
        sint * vec.x + cost * vec.y,    // y
        vec.z,                          // z
    };
}

Vec3 vec3_add(Vec3 a, Vec3 b){
    return (Vec3){ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3 vec3_sub(Vec3 a, Vec3 b){
    return (Vec3){ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3 vec3_scalmul(Vec3 vec, float scalar){
    return (Vec3){ vec.x * scalar, vec.y * scalar, vec.z * scalar };
}

Vec3 vec3_scaldiv(Vec3 vec, float scalar){
    return (Vec3){ vec.x / scalar, vec.y / scalar, vec.z / scalar };
}

float vec3_dot(Vec3 a, Vec3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
