#ifndef _STATE_H
#define _STATE_H

enum CPU_state
{
    READ_CMD,
    PARSE_CMD,
    READ_ARGS,
    EXEC_CMD,
    LOAD_DATA,
    STORE_DATA,
    CPU_HALT
};

#endif // _STATE_H
