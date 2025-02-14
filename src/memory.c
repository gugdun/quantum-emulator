#include <memory.h>
#include <stddef.h>
#include <stdio.h>

static uint8_t m[TOTAL_MEMORY_SIZE];

void m_write(uint8_t data, uint16_t addr)
{
    m[addr % TOTAL_MEMORY_SIZE] = data;
#ifdef DEBUG
    // printf("m[%04X] <= %02X\n", addr % TOTAL_MEMORY_SIZE, data);
#endif
}

uint8_t m_read(uint16_t addr)
{
#ifdef DEBUG
    // printf("m[%04X] => %02X\n", addr % TOTAL_MEMORY_SIZE, m[addr % TOTAL_MEMORY_SIZE]);
#endif
    return m[addr % TOTAL_MEMORY_SIZE];
}

uint8_t *m_get_ptr()
{
    return m;
}

void *m_get_framebuffer()
{
    return (void *)((size_t)m + RAM_SIZE);
}
