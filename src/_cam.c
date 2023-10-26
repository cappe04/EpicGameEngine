#include "_cam.h"

#include <math.h>

#include "_math.h"

Cam cam_init(Vec3 pos, float yaw, float pitch, float roll){
    Cam cam;
    cam_update(&cam, pos, yaw, pitch, roll);
    return cam;
}

void cam_update(Cam* cam, Vec3 pos, float yaw, float pitch, float roll){
    // Update values
    cam->pos =      pos;
    cam->yaw =      yaw;
    cam->pitch =    pitch;
    cam->roll =     roll;
}

Vec3 cam_apply(Cam* cam, Vec3 point){
    // Position
    point = vec3_add(point, cam->pos);
    // Rotation
    // OBS! Order ZYX important.
    point = vec3_rotatez(point, cam->roll);
    point = vec3_rotatey(point, cam->yaw);
    point = vec3_rotatex(point, cam->pitch);
    return point;
}