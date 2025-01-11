#include <help.h>
#include <stdio.h>

void print_help()
{
    printf("Usage: quark [options]\nOptions:\n");
    printf("  -h, --help    Print this message and exit.\n");
    printf("  -c, --cycles  Set number of cycles per frame.\n");
    printf("  -k, --kernel  Specify custom kernel path.\n");
    printf("  -b, --base    Specify custom kernel base address.\n");
}
