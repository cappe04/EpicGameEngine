#ifndef _GLOBALS_H
#define _GLOBALS_H

#define WINDOW_W 1280
#define WINDOW_H 720

#define DISPLAY_W 384 * 2 // 384
#define DISPLAY_H 216 * 2 //216

#define K1 120 // FOV?
#define Z_CLIP 0.1
#define MAX_DEPTH 1000

#define ERROR(...) { fprintf(stderr, __VA_ARGS__); exit(1); }

#endif // _GLOBALS_H