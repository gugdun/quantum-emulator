#ifndef _MEMORY_H
#define _MEMORY_H

#include <types.h>
#include <video.h>

#define RAM_SIZE 0x8000
#define TOTAL_MEMORY_SIZE (RAM_SIZE + VID_SCREEN_WIDTH * VID_SCREEN_HEIGHT * VID_PIXEL_SIZE)

void m_write(u8 data, u16 addr);
u8 m_read(u16 addr);
u8 *m_get_ptr();
void *m_get_framebuffer();

#endif // _MEMORY_H
