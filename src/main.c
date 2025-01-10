#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emulator.h>
#include <parser.h>
#include <loader.h>
#include <cpu.h>
#include <memory.h>

int setup_window(context *c)
{
    int error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0)
    {
        printf(
            "SDL_Init failed: %s\n",
            SDL_GetError()
        );
        return 1;
    }

    c->wnd = SDL_CreateWindow(
        "quark",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );
    if (c->wnd == NULL)
    {
        printf(
            "SDL_CreateWindow failed: %s\n",
            SDL_GetError()
        );
        SDL_Quit();
        return 1;
    }

    c->ctx = SDL_GL_CreateContext(c->wnd);
    if (c->ctx == NULL)
    {
        printf(
            "SDL_GL_CreateContext failed: %s\n",
            SDL_GetError()
        );
        SDL_DestroyWindow(c->wnd);
        SDL_Quit();
        return 1;
    }

    return 0;
}

void cleanup(context *c)
{
    SDL_GL_DeleteContext(c->ctx);
    SDL_DestroyWindow(c->wnd);
    SDL_Quit();
}

void poll_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            CPU_set_state(CPU_HALT);
    }
}

int main(int argc, char *argv[])
{
    context *c = get_context();
#ifdef DEBUG
    c->cycles_per_frame = 1;
#else
	c->cycles_per_frame = 1000000;
#endif
    c->kernel_path = "kernel.bin";
    c->kernel_base = 0x1000;

    if (parse_args(c, argc, argv)) return 1;
    if (setup_window(c)) return 1;
    
    load_kernel(c->kernel_path, c->kernel_base);
    CPU_reset(c->kernel_base);
    memset(m_get_framebuffer(), VID_RESET_COLOR, VID_PIXEL_COUNT);

    while (CPU_get_state() != CPU_HALT)
    {
        poll_events();

        for (int i = 0; i < c->cycles_per_frame; i++)
            CPU_cycle();

        glRasterPos2f(-1, 1);
        glPixelZoom(4, -4);
        glDrawPixels(
            VID_SCREEN_WIDTH,
            VID_SCREEN_HEIGHT,
            GL_RGB,
            VID_PIXEL_FORMAT,
            m_get_framebuffer()
        );
        
        SDL_GL_SwapWindow(c->wnd);
    }

    cleanup(c);
    return 0;
}
