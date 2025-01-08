#include <parser.h>

int parse_args(context *c, int argc, char *argv[])
{
    enum parser_state s = BLANK;
    for (int i = 1; i < argc; i++)
    {
        switch (s)
        {
            case BLANK:
                if (strcmp(argv[i], "-c") == 0) s = CYCLES;
                else if (strcmp(argv[i], "--cycles") == 0) s = CYCLES;
                else
                {
                    printf("Unknown argument specified: %s\n", argv[i]);
                    return 1;
                }
                break;
            
            case CYCLES:
                c->cycles_per_frame = (int)strtol(argv[i], (char **)NULL, 10);
                if (c->cycles_per_frame == 0)
                {
                    printf("Wrong number of cycle specified\n");
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
        case CYCLES:
            printf("Number of cycles is not specified\n");
            return 1;

        default:
            return 0;
    }
}
