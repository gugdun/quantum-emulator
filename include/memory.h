#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <video.h>

#define RAM_SIZE 0x8000
#define TOTAL_MEMORY_SIZE (RAM_SIZE + VID_SCREEN_WIDTH * VID_SCREEN_HEIGHT * VID_PIXEL_SIZE)

void m_write(uint8_t data, uint16_t addr);
uint8_t m_read(uint16_t addr);
uint8_t *m_get_ptr();
void *m_get_framebuffer();

#endif // _MEMORY_H
