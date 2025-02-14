#ifndef _EMULATOR_H
#define _EMULATOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <types.h>

typedef struct
{
    SDL_Window *wnd;
    SDL_GLContext ctx;
    bool help;
    bool version;
    bool running;
    int cycles_per_frame;
    char *kernel_path;
    u16 kernel_base;
} context;

context *get_context();

#endif // _EMULATOR_H
