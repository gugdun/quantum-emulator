#ifndef _CPU_H
#define _CPU_H

#include <state.h>
#include <stdint.h>

#define LATENCY 1

typedef struct {
    enum CPU_state state;
    uint16_t regs[16];
    uint8_t cmd;
    uint8_t args[5];
    uint8_t latency;
    uint8_t argc;
    uint8_t counter;
} CPU;

void CPU_reset(uint16_t vector);
void CPU_cycle();
void CPU_set_state(enum CPU_state new_state);
enum CPU_state CPU_get_state();

#endif // _CPU_H
