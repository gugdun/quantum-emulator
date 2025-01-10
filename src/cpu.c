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
#ifdef DEBUG
            printf("JMP 0x%04X\n", a1);
#endif
            c.regs[PC] = a1;
            c.latency  = LATENCY;
            break;

        case OP_JL:
#ifdef DEBUG
            printf(
                "JL R%d, R%d, 0x%04X\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1,
                a2
            );
#endif
            if (r1 < r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;
        
        case OP_JLE:
#ifdef DEBUG
            printf(
                "JLE R%d, R%d, 0x%04X\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1,
                a2
            );
#endif
            if (r1 <= r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;

        case OP_JE:
#ifdef DEBUG
            printf(
                "JE R%d, R%d, 0x%04X\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1,
                a2
            );
#endif
            if (r1 == r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;
    
        case OP_JNE:
#ifdef DEBUG
            printf(
                "JNE R%d, R%d, 0x%04X\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1,
                a2
            );
#endif
            if (r1 != r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;

        case OP_JGE:
#ifdef DEBUG
            printf(
                "JGE R%d, R%d, 0x%04X\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1,
                a2
            );
#endif
            if (r1 >= r2)
            {
                c.regs[PC] = a2;
                c.latency  = LATENCY;
            }
            break;

        case OP_JG:
#ifdef DEBUG
            printf(
                "JG R%d, R%d, 0x%04X\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1,
                a2
            );
#endif
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
#ifdef DEBUG
            printf(
                "ADD R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
            c.regs[c.args[0] >> 4] += r2;
            if ((u32)r1 + (u32)r2 > 0xFFFF)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case OP_ADDS:
#ifdef DEBUG
            printf(
                "ADDS R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
            c.regs[c.args[0] >> 4] = (u16)((s16)r1 + (s16)r2);
            if ((s32)r1 + (s32)r2 > 0x7FFF || (s32)r1 + (s32)r2 < -0x8000)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_SUB:
#ifdef DEBUG
            printf(
                "SUB R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
            c.regs[c.args[0] >> 4] -= r2;
            if (r2 > r1) c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case OP_SUBS:
#ifdef DEBUG
            printf(
                "SUBS R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
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
            else if (--a.temp2 == 0)
            {
#ifdef DEBUG
                printf(
                    "MUL R%d, R%d\n",
                    (c.args[0] >> 4) + 1,
                    (c.args[0] & 0x0F) + 1
                );
#endif
                c.state = READ_CMD;
            }
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
            else if (--a.temp2 == 0)
            {
#ifdef DEBUG
                printf(
                    "MULS R%d, R%d\n",
                    (c.args[0] >> 4) + 1,
                    (c.args[0] & 0x0F) + 1
                );
#endif
                c.state = READ_CMD;
            }
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
#ifdef DEBUG
                printf(
                    "DIV R%d, R%d\n",
                    (c.args[0] >> 4) + 1,
                    (c.args[0] & 0x0F) + 1
                );
#endif
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
#ifdef DEBUG
                printf(
                    "DIVS R%d, R%d\n",
                    (c.args[0] >> 4) + 1,
                    (c.args[0] & 0x0F) + 1
                );
#endif
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
            else if (r2 > r1)
            {
#ifdef DEBUG
                printf(
                    "MOD R%d, R%d\n",
                    (c.args[0] >> 4) + 1,
                    (c.args[0] & 0x0F) + 1
                );
#endif
                c.state = READ_CMD;
            }
            else
            {
                c.regs[c.args[0] >> 4] -= r2;
                if (c.latency > 0) c.latency--;
            }
            break;
        
        case OP_INV:
#ifdef DEBUG
            printf("INV R%d\n", (c.args[0] >> 4) + 1);
#endif
            c.regs[c.args[0] >> 4] = (u16)(-(s16)r1);
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_SHL:
#ifdef DEBUG
            printf(
                "SHL R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
            c.regs[c.args[0] >> 4] <<= r2;
            if (((u32)r1 << r2) > 0xFFFF)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_SHR:
#ifdef DEBUG
            printf(
                "SHR R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
            c.regs[c.args[0] >> 4] >>= r2;
            if (r1 > 0 && (r1 >> r2) == 0)
                c.regs[CR] |= CARRY;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_NOT:
#ifdef DEBUG
            printf("NOT R%d\n", (c.args[0] >> 4) + 1);
#endif
            c.regs[c.args[0] >> 4] = !r1;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_AND:
#ifdef DEBUG
            printf(
                "AND R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
            c.regs[c.args[0] >> 4] &= r2;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;
        
        case OP_OR:
#ifdef DEBUG
            printf(
                "OR R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
            c.regs[c.args[0] >> 4] |= r2;
            c.latency = LATENCY - 1;
            c.state   = READ_CMD;
            break;

        case OP_XOR:
#ifdef DEBUG
            printf(
                "XOR R%d, R%d\n",
                (c.args[0] >> 4) + 1,
                (c.args[0] & 0x0F) + 1
            );
#endif
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
    if ((c.cmd & CMD_MASK) == CMD_MATH) CPU_ALU();
    else if ((c.cmd & CMD_MASK) == CMD_JUMP) CPU_CU();
    else if (c.cmd == CMD_LDB)
    {
#ifdef DEBUG
        printf(
            "LDB R%d, R%d (%04X)\n",
            (c.args[0] >> 4) + 1,
            (c.args[0] & 0x0F) + 1,
            c.regs[c.args[0] & 0x0F]
        );
#endif
        c.cmd     = (c.cmd & 0xF0) + (c.args[0] >> 4);
        c.args[1] = c.regs[c.args[0] & 0x0F] >> 8;
        c.args[0] = c.regs[c.args[0] & 0x0F] & 0xFF;
        c.argc    = 1;
        c.counter = 0;
        c.latency = LATENCY;
        c.state   = LOAD_DATA;
    }
    else if (c.cmd == CMD_LDW)
    {
#ifdef DEBUG
        printf(
            "LDW R%d, R%d (%04X)\n",
            (c.args[0] >> 4) + 1,
            (c.args[0] & 0x0F) + 1,
            c.regs[c.args[0] & 0x0F]
        );
#endif
        c.cmd     = (c.cmd & 0xF0) + (c.args[0] >> 4);
        c.args[1] = c.regs[c.args[0] & 0x0F] >> 8;
        c.args[0] = c.regs[c.args[0] & 0x0F] & 0xFF;
        c.argc    = 2;
        c.counter = 0;
        c.latency = LATENCY;
        c.state   = LOAD_DATA;
    }
    else if (c.cmd == CMD_STB)
    {
#ifdef DEBUG
        printf(
            "STB R%d, R%d\n",
            (c.args[0] >> 4) + 1,
            (c.args[0] & 0x0F) + 1
        );
#endif
        c.cmd     = (c.cmd & 0xF0) + (c.args[0] >> 4);
        c.args[1] = c.regs[c.args[0] & 0x0F] >> 8;
        c.args[0] = c.regs[c.args[0] & 0x0F] & 0xFF;
        c.argc    = 1;
        c.counter = 0;
        c.latency = LATENCY;
        c.state   = STORE_DATA;
    }
    else if (c.cmd == CMD_STW)
    {
#ifdef DEBUG
        printf(
            "STW R%d, R%d\n",
            (c.args[0] >> 4) + 1,
            (c.args[0] & 0x0F) + 1
        );
#endif
        c.cmd     = (c.cmd & 0xF0) + (c.args[0] >> 4);
        c.args[1] = c.regs[c.args[0] & 0x0F] >> 8;
        c.args[0] = c.regs[c.args[0] & 0x0F] & 0xFF;
        c.argc    = 2;
        c.counter = 0;
        c.latency = LATENCY;
        c.state   = STORE_DATA;
    }
    else if (c.cmd == CMD_MOV)
    {
#ifdef DEBUG
        printf(
            "MOV R%d, R%d\n",
            (c.args[0] >> 4) + 1,
            (c.args[0] & 0x0F) + 1
        );
#endif
        c.regs[c.args[0] >> 4] = c.regs[c.args[0] & 0x0F];
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
    else if (c.cmd == CMD_INT)
    {
#ifdef DEBUG
        printf("Interrupts are not implemented!\n");
#endif
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
    else if (c.cmd == CMD_CALLI)
    {
#ifdef DEBUG
        printf("CALL 0x%02X%02X\n", c.args[1], c.args[0]);
#endif
        c.args[2]   = c.args[0];
        c.args[3]   = c.args[1];
        c.args[0]   = c.regs[SP] & 0xFF;
        c.args[1]   = c.regs[SP] >> 8;
        c.cmd       = (c.cmd & CMD_MASK) + PC;
        c.counter   = 0;
        c.argc      = 2;
        c.latency   = LATENCY;
        c.state     = STORE_DATA;
        c.regs[SP] += 2;
    }
    else if ((c.cmd & CMD_MASK) == CMD_CALL)
    {
#ifdef DEBUG
        printf("CALL R%d\n", c.cmd & 0x0F);
#endif
        c.args[2]   = c.cmd & 0x0F;
        c.args[0]   = c.regs[SP] & 0xFF;
        c.args[1]   = c.regs[SP] >> 8;
        c.cmd       = (c.cmd & CMD_MASK) + PC;
        c.counter   = 0;
        c.argc      = 2;
        c.latency   = LATENCY;
        c.state     = STORE_DATA;
        c.regs[SP] += 2;
    }
    else if ((c.cmd & CMD_MASK) == CMD_LDBA)
    {
#ifdef DEBUG
        printf(
            "LDB R%d, [0x%02X%02X]\n",
            (c.cmd & 0x0F) + 1,
            c.args[1],
            c.args[0]
        );
#endif
        c.argc    = 1;
        c.counter = 0;
        c.latency = LATENCY;
        c.state   = LOAD_DATA;
    }
    else if ((c.cmd & CMD_MASK) == CMD_LDWA)
    {
#ifdef DEBUG
        printf(
            "LDW R%d, [0x%02X%02X]\n",
            (c.cmd & 0x0F) + 1,
            c.args[1],
            c.args[0]
        );
#endif
        c.argc    = 2;
        c.counter = 0;
        c.latency = LATENCY;
        c.state   = LOAD_DATA;
    }
    else if ((c.cmd & CMD_MASK) == CMD_LDBI)
    {
#ifdef DEBUG
        printf(
            "LDB R%d, 0x%02X\n",
            (c.cmd & 0x0F) + 1,
            c.args[0]
        );
#endif
        c.regs[c.cmd & 0x0F] = c.args[0];
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
    else if ((c.cmd & CMD_MASK) == CMD_LDWI)
    {
#ifdef DEBUG
        printf(
            "LDW R%d, 0x%02X%02X\n",
            (c.cmd & 0x0F) + 1,
            c.args[1],
            c.args[0]
        );
#endif
        c.regs[c.cmd & 0x0F] = ((u16)c.args[1] << 8) + c.args[0];
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
    else if ((c.cmd & CMD_MASK) == CMD_STBA)
    {
#ifdef DEBUG
        printf(
            "STB R%d, [0x%02X%02X]\n",
            (c.cmd & 0x0F) + 1,
            c.args[1],
            c.args[0]
        );
#endif
        c.argc    = 1;
        c.counter = 0;
        c.latency = LATENCY;
        c.state   = STORE_DATA;
    }
    else if ((c.cmd & CMD_MASK) == CMD_STWA)
    {
#ifdef DEBUG
        printf(
            "STW R%d, [0x%02X%02X]\n",
            (c.cmd & 0x0F) + 1,
            c.args[1],
            c.args[0]
        );
#endif
        c.argc    = 2;
        c.counter = 0;
        c.latency = LATENCY;
        c.state   = STORE_DATA;
    }
    else if ((c.cmd & CMD_MASK) == CMD_PUSH)
    {
#ifdef DEBUG
        printf("PUSH R%d\n", (c.cmd & 0x0F) + 1);
#endif
        c.args[0]   = c.regs[SP] & 0xFF;
        c.args[1]   = c.regs[SP] >> 8;
        c.regs[SP] += 2;
        c.counter   = 0;
        c.argc      = 2;
        c.latency   = LATENCY;
        c.state     = STORE_DATA;
    }
    else if ((c.cmd & CMD_MASK) == CMD_POP)
    {
#ifdef DEBUG
        printf("POP R%d\n", (c.cmd & 0x0F) + 1);
#endif
        c.regs[SP] -= 2;
        c.args[0]   = c.regs[SP] & 0xFF;
        c.args[1]   = c.regs[SP] >> 8;
        c.counter   = 0;
        c.argc      = 2;
        c.latency   = LATENCY;
        c.state     = LOAD_DATA;
    }
}

static void CPU_parse()
{
    if (c.cmd == CMD_RET)
    {
#ifdef DEBUG
        printf("RET\n");
#endif
        c.regs[SP] -= 2;
        c.cmd     = PC;
        c.args[0] = c.regs[SP] & 0xFF;
        c.args[1] = c.regs[SP] >> 8;
        c.latency = LATENCY;
        c.counter = 0;
        c.argc    = 2;
        c.state   = LOAD_DATA;
    }
    else if (c.cmd == CMD_HLT)
    {
#ifdef DEBUG
        printf("HLT\n");
#endif
        c.state = CPU_HALT;
    }
    else if ((c.cmd & CMD_MASK) == CMD_INC)
    {
#ifdef DEBUG
        printf("INC R%d\n", (c.cmd & 0x0F) + 1);
#endif
        c.regs[c.cmd & 0x0F]++;
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
    else if ((c.cmd & CMD_MASK) == CMD_DEC)
    {
#ifdef DEBUG
        printf("DEC R%d\n", (c.cmd & 0x0F) + 1);
#endif
        c.regs[c.cmd & 0x0F]--;
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
    else if (((c.cmd & CMD_MASK) == CMD_MATH) ||
             ((c.cmd & CMD_MASK) == CMD_LDBI) ||
              (c.cmd == CMD_LDB) ||
              (c.cmd == CMD_LDW) ||
              (c.cmd == CMD_STB) ||
              (c.cmd == CMD_STW) ||
              (c.cmd == CMD_MOV) ||
              (c.cmd == CMD_INT))
    {
        a.started = false;
        c.latency = LATENCY - 1;
        c.counter = 0;
        c.argc    = 1;
        c.state   = READ_ARGS;
    }
    else if ((c.cmd & CMD_MASK) == CMD_JUMP)
    {
        c.latency = LATENCY - 1;
        c.counter = 0;
        c.argc    = (c.cmd == CMD_JUMP) ? 2 : 3;
        c.state   = READ_ARGS;
    }
    else if (((c.cmd & CMD_MASK) == CMD_LDBA) ||
             ((c.cmd & CMD_MASK) == CMD_LDWA) ||
             ((c.cmd & CMD_MASK) == CMD_LDWI) ||
             ((c.cmd & CMD_MASK) == CMD_STBA) ||
             ((c.cmd & CMD_MASK) == CMD_STWA) ||
              (c.cmd == CMD_CALLI))
    {
        c.latency = LATENCY - 1;
        c.counter = 0;
        c.argc    = 2;
        c.state   = READ_ARGS;
    }
    else if (((c.cmd & CMD_MASK) == CMD_PUSH) ||
             ((c.cmd & CMD_MASK) == CMD_CALL) ||
             ((c.cmd & CMD_MASK) == CMD_POP))
    {
        c.state = EXEC_CMD;
    }
    else
    {
#ifdef DEBUG
        printf("NOP\n");
#endif
        c.latency = LATENCY - 1;
        c.state   = READ_CMD;
    }
}

void CPU_reset(u16 vector)
{
    c.regs[PC] = vector;
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
                m_read((((u16)c.args[1]) << 8) + c.args[0]) :
                (((u16)m_read((((u16)c.args[1]) << 8) + c.args[0]) << 8) + c.regs[c.cmd & 0x0F]);

            if (c.args[0] == 0xFF)
            {
                c.args[0] = 0x00;
                if (c.args[1] == 0xFF)
                     c.args[1] = 0x00;
                else c.args[1]++;
            } else c.args[0]++;
            
            if (++c.counter == c.argc)
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
            
            if (++c.counter == c.argc)
            {
                if (c.cmd == CMD_CALLI)
                    c.regs[PC] = ((u16)c.args[3] << 8) + c.args[2];
                else if ((c.cmd & CMD_MASK) == CMD_CALL)
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
