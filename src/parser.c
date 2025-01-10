#include <stdio.h>
#include <parser.h>

int parse_args(context *c, int argc, char *argv[])
{
    enum parser_state s = BLANK;
    for (int i = 1; i < argc; i++)
    {
        switch (s)
        {
            case BLANK:
                if (strcmp(argv[i], "-c") == 0) s = CYCLES_PER_FRAME;
                else if (strcmp(argv[i], "--cycles") == 0) s = CYCLES_PER_FRAME;
                else if (strcmp(argv[i], "-k") == 0) s = KERNEL_PATH;
                else if (strcmp(argv[i], "--kernel") == 0) s = KERNEL_PATH;
                else if (strcmp(argv[i], "-b") == 0) s = KERNEL_BASE;
                else if (strcmp(argv[i], "--base") == 0) s = KERNEL_BASE;
                else
                {
                    printf("Unknown argument specified: %s\n", argv[i]);
                    return 1;
                }
                break;
            
            case CYCLES_PER_FRAME:
                c->cycles_per_frame = (int)(strtoul(argv[i], (char **)NULL, 10) & 0xFFFFFFFF);
                if (c->cycles_per_frame == 0)
                {
                    printf("Wrong number of cycle specified\n");
                    return 1;
                }
                s = BLANK;
                break;
            
            case KERNEL_PATH:
                c->kernel_path = argv[i];
                s = BLANK;
                break;
            
            case KERNEL_BASE:
                c->kernel_base = (u16)(strtoul(argv[i], (char **)NULL, 16) & 0xFFFF);
                if (c->kernel_base == 0)
                {
                    printf("Bad kernel address specified\n");
                    return 1;
                }
                s = BLANK;
                break;

            default:
                break;
        }
    }

    switch (s)
    {
        case CYCLES_PER_FRAME:
            printf("Number of cycles is not specified\n");
            return 1;
        
        case KERNEL_PATH:
            printf("Kernel path is not specified\n");
            return 1;

        case KERNEL_BASE:
            printf("Kernel base address is not specified\n");
            return 1;

        default:
            return 0;
    }
}
