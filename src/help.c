#include <help.h>
#include <config.h>
#include <stdio.h>

void print_help()
{
    printf("Usage: quark [options]\nOptions:\n");
    printf("  -h, --help    Print this message and exit.\n");
    printf("  -c, --cycles  Set number of cycles per frame.\n");
    printf("  -k, --kernel  Specify custom kernel path.\n");
    printf("  -b, --base    Specify custom kernel base address.\n");
}

void print_version()
{
    printf(
        "quark version %d.%d.%d by %s\n",
        VERSION_MAJOR,
        VERSION_MINOR,
        VERSION_PATCH,
        DEVELOPER
    );
}

