#ifndef _COMMANDS_H
#define _COMMANDS_H

#define CMD_MATH  0x10
#define CMD_JUMP  0x20
#define CMD_LDB   0x31
#define CMD_LDW   0x32
#define CMD_STB   0x33
#define CMD_STW   0x34
#define CMD_RET   0x35
#define CMD_MOV   0x36
#define CMD_INT   0x37
#define CMD_CALLI 0x3F
#define CMD_LDBA  0x40
#define CMD_LDWA  0x50
#define CMD_LDBI  0x60
#define CMD_LDWI  0x70
#define CMD_STBA  0x80
#define CMD_STWA  0x90
#define CMD_PUSH  0xA0
#define CMD_POP   0xB0
#define CMD_CALL  0xC0
#define CMD_INC   0xD0
#define CMD_DEC   0xE0
#define CMD_MATHF 0xF0
#define CMD_HLT   0xFF

#define CMD_MASK  0xF0

#endif // _COMMANDS_H