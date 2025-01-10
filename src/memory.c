#include <memory.h>
#include <stddef.h>
#include <stdio.h>

static u8 m[TOTAL_MEMORY_SIZE];

void m_write(u8 data, u16 addr)
{
    m[addr % TOTAL_MEMORY_SIZE] = data;
}

u8 m_read(u16 addr)
{
    return m[addr % TOTAL_MEMORY_SIZE];
}

u8 *m_get_ptr()
{
    return m;
}

void *m_get_framebuffer()
{
    return (void *)((size_t)m + RAM_SIZE);
}
