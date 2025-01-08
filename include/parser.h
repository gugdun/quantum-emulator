#ifndef _PARSER_H
#define _PARSER_H

#include <emulator.h>

enum parser_state
{
    BLANK,
    CYCLES
};

int parse_args(context *c, int argc, char *argv[]);

#endif // _PARSER_H
