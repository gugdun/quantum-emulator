#include <cpu.h>
#include <regs.h>

static CPU c;

void CPU_reset()
{
    c.regs[PC] = 0x1000;
    c.state = READ_CMD;
}

void CPU_cycle()
{
    switch (c.state)
    {
        case READ_CMD:
            break;
        case PARSE_CMD:
            break;
        case READ_ARGS:
            break;
        case EXEC_CMD:
            break;
        case LOAD_DATA:
            break;
        case STORE_DATA:
            break;
        default:
            break;
    }
}

void CPU_set_state(enum CPU_state new_state)
{
    c.state = new_state;
}

enum CPU_state CPU_get_state()
{
    return c.state;
}
