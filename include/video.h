#ifndef _VIDEO_H
#define _VIDEO_H

#include <types.h>
#include <SDL2/SDL_opengl.h>

#define VID_SCREEN_WIDTH  160
#define VID_SCREEN_HEIGHT 120
#define VID_PIXEL_COUNT   VID_SCREEN_WIDTH * VID_SCREEN_HEIGHT
#define VID_PIXEL_SIZE    sizeof(u8)
#define VID_PIXEL_FORMAT  GL_UNSIGNED_BYTE_3_3_2
#define VID_RESET_COLOR   0b01100100

#endif // _VIDEO_H
