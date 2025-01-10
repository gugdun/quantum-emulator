#include <memory.h>
#include <stddef.h>

static u8 m[51968];

void m_write(u8 data, u16 addr)
{
    m[addr % 51968] = data;
}

u8 m_read(u16 addr)
{
    return m[addr % 51968];
}

void *m_get_framebuffer()
{
    return (void *)((size_t)m + 32768);
}
