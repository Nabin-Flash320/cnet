

#include "stdio.h"
#include "stdint.h"
#include "trace.h"
#include "stdarg.h"

void print_e(char *printable_format, ...)
{
    va_list args;
    va_start(args, printable_format);
    printf(ANSI_COLOR_RED "%s:%d==> ", __FILE__, __LINE__);
    vprintf(printable_format, args);
    printf("\n" ANSI_COLOR_RESET);
    va_end(args);
}

void print_i(char *printable_format, ...)
{
    va_list args;
    va_start(args, printable_format);
    printf(ANSI_COLOR_GREEN "%s:%d==> ", __FILE__, __LINE__);
    vprintf(printable_format, args);
    printf("\n" ANSI_COLOR_RESET);
    va_end(args);
}

void print_w(char *printable_format, ...)
{
    va_list args;
    va_start(args, printable_format);
    printf(ANSI_COLOR_YELLOW "%s:%d==> ", __FILE__, __LINE__);
    vprintf(printable_format, args);
    printf("\n" ANSI_COLOR_RESET);
    va_end(args);
}

void print_b(char *printable_format, ...)
{
    va_list args;
    va_start(args, printable_format);
    printf(ANSI_COLOR_BLUE "%s:%d==> ", __FILE__, __LINE__);
    vprintf(printable_format, args);
    printf("\n" ANSI_COLOR_RESET);
    va_end(args);
}


