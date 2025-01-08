#ifndef _MEMORY_H
#define _MEMORY_H

#include <types.h>

void m_write(u8 data, u16 addr);
u8 m_read(u16 addr);
void *m_get_framebuffer();

#endif // _MEMORY_H
