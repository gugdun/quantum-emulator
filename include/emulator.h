#ifndef _EMULATOR_H
#define _EMULATOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

typedef struct
{
    SDL_Window *wnd;
    SDL_GLContext ctx;
    int cycles_per_frame;
} context;

context *get_context();

#endif // _EMULATOR_H
