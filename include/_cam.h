#ifndef _CAM_H
#define _CAM_H

#include "_math.h"

typedef struct s_Cam {
    Vec3 pos;
    float yaw, pitch, roll;
} Cam;

Cam cam_init(Vec3 pos, float yaw, float pitch, float roll);
void cam_update(Cam* cam, Vec3 pos, float yaw, float pitch, float roll);
Vec3 cam_apply(Cam* cam, Vec3 point);

#endif // _CAM_H