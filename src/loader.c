#include <stdio.h>
#include <memory.h>
#include <loader.h>

void load_kernel(char *path, u16 base)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        printf("Kernel file not found: %s\n", path);
        return;
    }

    size_t count = 0;
    while (1)
    {
        u8 data = 0;
        fread(&data, sizeof(u8), 1, file);
        if (feof(file)) break;
        m_write(data, base + count);
        count++;
    }

    printf("Loaded kernel file: %s\n", path);
    printf("Kernel base: 0x%04X\n", base);
    printf("Kernel size: 0x%04X\n", (u32)count);
    fclose(file);
}
