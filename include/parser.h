#ifndef _PARSER_H
#define _PARSER_H

#include <emulator.h>

enum parser_state
{
    BLANK,
    CYCLES_PER_FRAME,
    KERNEL_PATH,
    KERNEL_BASE
};

int parse_args(context *c, int argc, char *argv[]);

#endif // _PARSER_H
