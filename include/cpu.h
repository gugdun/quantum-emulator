#ifndef _CPU_H
#define _CPU_H

#include <state.h>
#include <types.h>

#define LATENCY 1

typedef struct {
    enum CPU_state state;
    u16 regs[16];
    u8 cmd;
    u8 args[4];
    u8 latency;
    u8 argc;
    u8 counter;
} CPU;

void CPU_reset();
void CPU_cycle();
void CPU_set_state(enum CPU_state new_state);
enum CPU_state CPU_get_state();

#endif // _CPU_H
