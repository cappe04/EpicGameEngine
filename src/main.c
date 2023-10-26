#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "SDL2/SDL.h"

#include "_globals.h"
#include "_renderer.h"
#include "_math.h"
#include "_cam.h"
#include "_graphics.h"

struct {
    clock_t t1;
    clock_t t2;
    double delta;
} timer;

static void update(Cam* cam, double dt);

int main(int argc, char *argv[]){

    if(SDL_Init(SDL_INIT_VIDEO)){
        ERROR("SDL_Init failed: %s\n", SDL_GetError());
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    static uint32_t pixels[DISPLAY_W * DISPLAY_H];
    static float z_buffer[DISPLAY_W * DISPLAY_H];
    memset(z_buffer, 0, sizeof(z_buffer));

    bool quit = false;

    window = SDL_CreateWindow(
        "3D Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        0                           // Flags
    ); 
    if(!window){
        ERROR("SDL_CreateWindow failed: %s\n", SDL_GetError());
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        ERROR("SDL_CreateRenderer failed: %s\n", SDL_GetError());
    }


    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_W, DISPLAY_H
    );
    if(!texture){
        ERROR("SDL_CreateTexture failed: %s\n", SDL_GetError());
    }

    SDL_Rect dstrect = { 0, 0, DISPLAY_W, DISPLAY_H }; // { 0, 0, WINDOW_W, WINDOW_H };

    Vec3 vertices[8] = {
        vec3_scalmul((Vec3){ -0.5,  0.5, -0.5 }, -1),
        vec3_scalmul((Vec3){ -0.5, -0.5, -0.5 }, -1),
        vec3_scalmul((Vec3){  0.5,  0.5, -0.5 }, -1),
        vec3_scalmul((Vec3){  0.5, -0.5, -0.5 }, -1),
        vec3_scalmul((Vec3){ -0.5,  0.5,  0.5 }, -1),
        vec3_scalmul((Vec3){ -0.5, -0.5,  0.5 }, -1),
        vec3_scalmul((Vec3){  0.5,  0.5,  0.5 }, -1),
        vec3_scalmul((Vec3){  0.5, -0.5,  0.5 }, -1),
    };

    unsigned int edges[24] = {
        0, 1,
        0, 2,
        0, 4,
        3, 1,
        3, 2,
        3, 7,
        5, 4,
        5, 7,
        5, 1,
        6, 4,
        6, 7,
        6, 2,
    };

    Cam cam = cam_init((Vec3){ 0, 0, 0 }, PI, 0, 0);

    timer.t1 = clock();

    while(!quit){

        timer.t2 = timer.t1;
        timer.t1 = clock();
        timer.delta = (double)(timer.t1 - timer.t2) / CLOCKS_PER_SEC;

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            
            default:
                break;
            }
        }

        // Update here
        update(&cam, timer.delta);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        memset(pixels, rgba_uint32(80, 80, 80, 255), sizeof(pixels));
        memset(z_buffer, 0, sizeof(z_buffer));

        // Cube
        for(int i = 0; i<24; i+=2){
            Vec3 p1 = cam_apply(&cam, vertices[edges[i  ]]);
            Vec3 p2 = cam_apply(&cam, vertices[edges[i+1]]);
            render_line(pixels, new_color(255, 255, 0, 255), p1, p2);
        }


        { // X-axis (red)
            Vec3 p1 = cam_apply(&cam, (Vec3){ -50, 0, 0 });
            Vec3 p2 = cam_apply(&cam, (Vec3){  50, 0, 0 });

            render_line(pixels, new_color(255, 0, 0, 255), p1, p2);
        }
        { // Y-axis (green)
            Vec3 p1 = cam_apply(&cam, (Vec3){ 0, -50, 0 });
            Vec3 p2 = cam_apply(&cam, (Vec3){ 0,  50, 0 });

            render_line(pixels, new_color(0, 255, 0, 255), p1, p2);
        }
        { // Z-axis (blue)
            Vec3 p1 = cam_apply(&cam, (Vec3){ 0, 0, -50 });
            Vec3 p2 = cam_apply(&cam, (Vec3){ 0, 0,  50 });

            render_line(pixels, new_color(0, 0, 255, 255), p1, p2);
        }
        // Crosshair
        set_pixel(pixels, new_color(255, 255, 255, 255), DISPLAY_W / 2, DISPLAY_H / 2);

        { // Tetrahedron
            Vec3 p1 = cam_apply(&cam, vec3_scalmul((Vec3){  5, 0,  5 }, -1));
            Vec3 p2 = cam_apply(&cam, vec3_scalmul((Vec3){ 10, 0,  5 }, -1));
            Vec3 p3 = cam_apply(&cam, vec3_scalmul((Vec3){  5, 0, 10 }, -1));
            Vec3 p4 = cam_apply(&cam, vec3_scalmul((Vec3){  5, 5,  5 }, -1));
            // Bottom
            render_triangle(pixels, z_buffer, new_color(255, 0, 0, 255), p1, p2, p3);
            // Sides
            render_triangle(pixels, z_buffer, new_color(0, 255, 0, 255), p1, p2, p4);
            render_triangle(pixels, z_buffer, new_color(0, 0, 255, 255), p1, p3, p4);
            // Top
            render_triangle(pixels, z_buffer, new_color(255, 255, 0, 255), p2, p3, p4);
        }

        { // Depth test
            Vec3 p1 = cam_apply(&cam, vec3_scalmul((Vec3){  -5, 0,  5 }, -1));
            Vec3 p2 = cam_apply(&cam, vec3_scalmul((Vec3){ -10, 0,  5 }, -1));
            Vec3 p3 = cam_apply(&cam, vec3_scalmul((Vec3){  -5, 5,  10 }, -1));
            render_triangle(pixels, z_buffer, new_color(0, 255, 255, 255), p1, p2, p3);
        }

        SDL_UpdateTexture(texture, NULL, pixels, DISPLAY_W * 4);
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);

    }

    return 0;
}

static void update(Cam* cam, double dt){
    const uint8_t* keystate = SDL_GetKeyboardState(NULL);
    float movspeed = 7.5 * dt;
    float rotspeed = 2.5 * dt;

    Vec3 pos =      cam->pos;
    float pitch =   cam->pitch;
    float yaw =     cam->yaw;
    float roll =    cam->roll;

    float cos_yaw = cosf(yaw);
    float sin_yaw = sinf(yaw);

    if(keystate[SDL_SCANCODE_W]){
        pos.z -= cos_yaw * movspeed;
        pos.x += sin_yaw * movspeed;
    }
    if(keystate[SDL_SCANCODE_S]){
        pos.z += cos_yaw * movspeed;
        pos.x -= sin_yaw * movspeed;
    }
    if(keystate[SDL_SCANCODE_D]){
        pos.x -= cos_yaw * movspeed;
        pos.z -= sin_yaw * movspeed;
    }
    if(keystate[SDL_SCANCODE_A]){
        pos.x += cos_yaw * movspeed;
        pos.z += sin_yaw * movspeed;
    }
    if(keystate[SDL_SCANCODE_SPACE]){
        pos.y += movspeed;
    }
    if(keystate[SDL_SCANCODE_LCTRL]){
        pos.y -= movspeed;
    }

    if(keystate[SDL_SCANCODE_UP]){
        pitch -= rotspeed;
    }
    if(keystate[SDL_SCANCODE_DOWN]){
        pitch += rotspeed;
    }
    if(keystate[SDL_SCANCODE_RIGHT]){
        yaw -= rotspeed;
    }
    if(keystate[SDL_SCANCODE_LEFT]){
        yaw += rotspeed;
    }
    if(keystate[SDL_SCANCODE_Q]){
        roll += rotspeed;
    }
    if(keystate[SDL_SCANCODE_E]){
        roll -= rotspeed;
    }

    cam_update(cam, pos, yaw, pitch, roll);
}