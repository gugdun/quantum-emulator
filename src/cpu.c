#include <stdio.h>
#include <cpu.h>
#include <alu.h>
#include <regs.h>
#include <flags.h>
#include <commands.h>
#include <operations.h>
#include <memory.h>

static CPU c;
static ALU a;

static void CPU_CU()
{
    c.latency = LATENCY - 1;
    u16 r1 = c.regs[c.args[0] >> 4];
    u16 r2 = c.regs[c.args[0] & 0x0F];
    u16 a1 = ((u16)c.args[1] << 8) + c.args[0];
    u16 a2 = ((u16)c.args[2] << 8) + c.args[1];
    switch (c.cmd & 0x0F)
    {
        case OP_JMP:
            c.regs[PC] = a1;
            c.latency  = LATENCY;
            break;

        case OP_JL:
            if (r1 < r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;
        
        case OP_JLE:
            if (r1 <= r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;

        case OP_JE:
            if (r1 == r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;

        case OP_JGE:
            if (r1 >= r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;

        case OP_JG:
            if (r1 > r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;
        
        default:
            break;
    }
    c.state = READ_CMD;
}

static void CPU_ALU()
{
    u16 r1 = c.regs[c.args[0] >> 4];
    u16 r2 = c.regs[c.args[0] & 0x0F];
    switch (c.cmd & 0x0F)
    {
        case OP_ADD:
            c.regs[c.args[0] >> 4] += r2;
            if ((u32)r1 + (u32)r2 > 0xFFFF)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case OP_ADDS:
            c.regs[c.args[0] >> 4] = (u16)((s16)r1 + (s16)r2);
            if ((s32)r1 + (s32)r2 > 0x7FFF || (s32)r1 + (s32)r2 < -0x8000)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_SUB:
            c.regs[c.args[0] >> 4] -= r2;
            if (r2 > r1) c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case OP_SUBS:
            c.regs[c.args[0] >> 4] = (u16)((s16)r1 - (s16)r2);
            if ((s32)r1 - (s32)r2 > 0x7FFF || (s32)r1 - (s32)r2 < -0x8000)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case OP_MUL:
            if (!a.started)
            {
                a.temp1   = r1;
                a.temp2   = r2;
                a.started = true;
                c.latency = LATENCY - 1;
            }
            else if (--a.temp2 == 0) c.state = READ_CMD;
            else
            {
                if ((u32)r1 + (u32)a.temp1 > 0xFFFF)
                    c.regs[CR] |= CARRY;
                c.regs[c.args[0] >> 4] += a.temp1;
                if (c.latency > 0) c.latency--;
            }
            break;

        case OP_MULS:
            if (!a.started)
            {
                a.temp1   = (s16)r2 < 0 ? -r1 : r1;
                a.temp2   = (s16)r2 < 0 ? -r2 : r2;
                a.started = true;
                c.latency = LATENCY - 1;
            }
            else if (--a.temp2 == 0) c.state = READ_CMD;
            else
            {
                if ((s32)r1 + (s32)a.temp1 > 0x7FFF || (s32)r1 + (s32)a.temp1 < -0x8000)
                    c.regs[CR] |= CARRY;
                c.regs[c.args[0] >> 4] += a.temp1;
                if (c.latency > 0) c.latency--;
            }
            break;

        case OP_DIV:
            if (!a.started)
            {
                a.temp1   = 0;
                a.started = true;
                c.latency = LATENCY - 1;
            }
            else if (r2 > r1)
            {
                r1 = a.temp1;
                c.state = READ_CMD;
            }
            else
            {
                a.temp1++;
                c.regs[c.args[0] >> 4] -= r2;
                if (c.latency > 0) c.latency--;
            }
            break;
        
        case OP_DIVS:
            if (!a.started)
            {
                a.temp1   = 0;
                a.temp2   = ((s16)r1 < 0 ? 1 : 0) ^ ((s16)r2 < 0 ? 1 : 0);
                a.started = true;
                c.latency = LATENCY - 1;
                if (a.temp2) c.regs[c.args[0] >> 4] = -c.regs[c.args[0] >> 4];
            }
            else if (r2 > r1)
            {
                r1 = a.temp2 ? -a.temp1 : a.temp1;
                c.state = READ_CMD;
            }
            else
            {
                a.temp1++;
                c.regs[c.args[0] >> 4] -= r2;
                if (c.latency > 0) c.latency--;
            }
            break;
        
        case OP_MOD:
            if (!a.started)
            {
                a.started = true;
                c.latency = LATENCY - 1;
            }
            else if (r2 > r1) c.state = READ_CMD;
            else
            {
                c.regs[c.args[0] >> 4] -= r2;
                if (c.latency > 0) c.latency--;
            }
            break;
        
        case OP_INV:
            c.regs[c.args[0] >> 4] = (u16)(-(s16)r1);
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_SHL:
            c.regs[c.args[0] >> 4] <<= r2;
            if (((u32)r1 << r2) > 0xFFFF)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_SHR:
            c.regs[c.args[0] >> 4] >>= r2;
            if (r1 > 0 && (r1 >> r2) == 0)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_NOT:
            c.regs[c.args[0] >> 4] = !r1;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_AND:
            c.regs[c.args[0] >> 4] &= r2;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case OP_OR:
            c.regs[c.args[0] >> 4] |= r2;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_XOR:
            c.regs[c.args[0] >> 4] ^= r2;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        default:
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
    }
}

static void CPU_exec()
{
    switch (c.cmd & 0xF0)
    {
        case CMD_MATH:
            CPU_ALU();
            break;
        
        case CMD_JUMP:
            CPU_CU();
            break;
        
        case CMD_LOAD:
            c.counter = 0;
            c.latency = LATENCY;
            c.state   = LOAD_DATA;
            break;
        
        case CMD_LOADI:
            c.regs[c.cmd & 0x0F] = ((u16)c.args[1] << 8) + c.args[0];
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case CMD_STORE:
            c.latency = LATENCY;
            c.state   = STORE_DATA;
            break;
        
        case CMD_PUSH:
            c.args[0]   = c.regs[SP] & 0xFF;
            c.args[1]   = c.regs[SP] >> 8;
            c.regs[SP] += 2;
            c.latency   = LATENCY;
            c.state     = STORE_DATA;
            break;

        case CMD_POP:
            c.regs[SP] -= 2;
            c.args[0]   = c.regs[SP] & 0xFF;
            c.args[1]   = c.regs[SP] >> 8;
            c.latency   = LATENCY;
            c.state     = LOAD_DATA;
            break;
        
        case CMD_CALL:
        case CMD_CALLI:
            if ((c.cmd & 0xF0) == CMD_CALL)
            {
                c.args[2] = c.cmd & 0x0F;
            }
            else
            {
                c.args[2] = c.args[0];
                c.args[3] = c.args[1];
            }
            c.args[0]   = c.regs[SP] & 0xFF;
            c.args[1]   = c.regs[SP] >> 8;
            c.cmd       = (c.cmd & 0xF0) + PC;
            c.latency   = LATENCY;
            c.state     = STORE_DATA;
            c.regs[SP] += 2;
            break;

        case CMD_MOV:
            c.regs[c.args[0] >> 4] = c.regs[c.args[0] & 0x0F];
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case CMD_INT:
            printf("Interrupts are not implemented!\n");
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        default:
            break;
    }
}

static void CPU_parse()
{
    if (c.cmd == CMD_RET)
    {
        c.regs[SP] -= 2;
        c.cmd     = PC;
        c.args[0] = c.regs[SP] & 0xFF;
        c.args[1] = c.regs[SP] >> 8;
        c.latency = LATENCY;
        c.counter = 0;
        c.state   = LOAD_DATA;
    }
    else if (c.cmd == CMD_HLT)
    {
        c.state = CPU_HALT;
    }
    else if ((c.cmd & CMD_INC) == CMD_INC)
    {
        c.regs[c.cmd & 0x0F]++;
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
    else if ((c.cmd & CMD_DEC) == CMD_DEC)
    {
        c.regs[c.cmd & 0x0F]--;
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
    else if (((c.cmd & CMD_MATH) == CMD_MATH) ||
             ((c.cmd & CMD_MOV)  == CMD_MOV)  ||
             (c.cmd == CMD_INT))
    {
        a.started = false;
        c.latency = LATENCY - 1;
        c.counter = 0;
        c.argc    = 1;
        c.state   = READ_ARGS;
    }
    else if ((c.cmd & CMD_JUMP) == CMD_JUMP)
    {
        c.latency = LATENCY - 1;
        c.counter = 0;
        c.argc    = (c.cmd == CMD_JUMP) ? 2 : 3;
        c.state   = READ_ARGS;
    }
    else if (((c.cmd & CMD_LOAD)  == CMD_LOAD)  ||
             ((c.cmd & CMD_LOADI) == CMD_LOADI) ||
             ((c.cmd & CMD_STORE) == CMD_STORE) ||
             ((c.cmd & CMD_CALLI) == CMD_CALLI))
    {
        c.latency = LATENCY - 1;
        c.counter = 0;
        c.argc    = 2;
        c.state   = READ_ARGS;
    }
    else if (((c.cmd & CMD_PUSH) == CMD_PUSH) ||
             ((c.cmd & CMD_CALL) == CMD_CALL) ||
             ((c.cmd & CMD_POP)  == CMD_POP))
    {
        c.state = EXEC_CMD;
    }
    else
    {
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
}

void CPU_reset()
{
    c.regs[PC] = 0x1000;
    c.latency  = LATENCY;
    c.state    = READ_CMD;
}

void CPU_cycle()
{
    switch (c.state)
    {
        case READ_CMD:
            if (c.latency)
            {
                c.latency--;
                break;
            }

            c.cmd = m_read(c.regs[PC]);
            c.regs[PC]++;
            c.state = PARSE_CMD;

            break;

        case PARSE_CMD:
            CPU_parse();
            break;

        case READ_ARGS:
            if (c.latency)
            {
                c.latency--;
                break;
            }

            c.args[c.counter] = m_read(c.regs[PC]);
            c.regs[PC]++;
            if (++c.counter == c.argc)
                 c.state   = EXEC_CMD;
            else c.latency = LATENCY;

            break;

        case EXEC_CMD:
            CPU_exec();
            break;

        case LOAD_DATA:
            if (c.latency)
            {
                c.latency--;
                break;
            }
            
            c.regs[c.cmd & 0x0F] = (c.counter == 0) ?
                ((u16)m_read((((u16)c.args[1]) << 8) + c.args[0]) << 8) :
                (c.regs[c.cmd & 0x0F] + m_read((((u16)c.args[1]) << 8) + c.args[0]));

            if (c.args[0] == 0xFF)
            {
                c.args[0] = 0x00;
                if (c.args[1] == 0xFF)
                     c.args[1] = 0x00;
                else c.args[1]++;
            } else c.args[0]++;
            
            if (++c.counter == 2)
                c.state = READ_CMD;
            c.latency = LATENCY;
            break;

        case STORE_DATA:
            if (c.latency)
            {
                c.latency--;
                break;
            }

            m_write(
                (c.counter == 0) ?
                    (c.regs[c.cmd & 0x0F] & 0xFF) :
                    (c.regs[c.cmd & 0x0F] >> 8),
                (((u16)c.args[1]) << 8) + c.args[0]
            );

            if (c.args[0] == 0xFF)
            {
                c.args[0] = 0x00;
                if (c.args[1] == 0xFF)
                        c.args[1] = 0x00;
                else c.args[1]++;
            } else c.args[0]++;
            
            if (++c.counter == 2)
            {
                if ((c.cmd & 0xF0) == CMD_CALLI)
                    c.regs[PC] = ((u16)c.args[3] << 8) + c.args[2];
                else if ((c.cmd & 0xF0) == CMD_CALL)
                    c.regs[PC] = c.regs[c.args[2] & 0x0F];
                c.state = READ_CMD;
            }
            c.latency = LATENCY;

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
