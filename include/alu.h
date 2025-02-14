#ifndef _ALU_H
#define _ALU_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t temp1;
    uint16_t temp2;
    bool started;
} ALU;

#endif // _ALU_H
