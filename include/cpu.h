#ifndef _CPU_H
#define _CPU_H

#include <state.h>
#include <types.h>

typedef struct {
    enum cpu_state state;
    u16 regs[16];
    u8  temp[5];
} cpu;

#endif // _CPU_H
