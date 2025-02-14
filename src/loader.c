#include <stdio.h>
#include <memory.h>
#include <loader.h>

void load_kernel(char *path, uint16_t base)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        printf("Kernel file not found: %s\n", path);
        return;
    }

    size_t count = 0;
	uint8_t *mem = m_get_ptr();
    while (1)
    {
        uint8_t data = 0;
        size_t c = fread(&data, sizeof(uint8_t), 1, file);
        if (!c || feof(file)) break;
		*(mem + base + count) = data;
        count++;
    }

    printf("Loaded kernel file: %s\n", path);
    printf("Kernel base: 0x%04X\n", base);
    printf("Kernel size: 0x%04X\n", (uint32_t)count);
    fclose(file);
}
