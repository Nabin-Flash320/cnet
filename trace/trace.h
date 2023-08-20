

#ifndef __TRACE_H__
#define __TRACE_H__

#include "stdio.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define TRACE_E(...)                                         \
    printf(ANSI_COLOR_RED "%s:%d ==> ", __FILE__, __LINE__); \
    printf(__VA_ARGS__);                                     \
    printf("\n" ANSI_COLOR_RESET);

#define TRACE_I(...)                                          \
    printf(ANSI_COLOR_GREEN "%s:%d==> ", __FILE__, __LINE__); \
    printf(__VA_ARGS__);                                     \
    printf("\n" ANSI_COLOR_RESET);

#define TRACE_W(...)                         \
    printf(ANSI_COLOR_YELLOW "%s:%d==> ", __FILE__, __LINE__); \
    printf(__VA_ARGS__);                                      \
    printf("\n" ANSI_COLOR_RESET);

#define TRACE_B(...)                       \
    printf(ANSI_COLOR_BLUE "%s:%d==> ", __FILE__, __LINE__); \
    printf(__VA_ARGS__);                                    \
    printf("\n" ANSI_COLOR_RESET);

void print_e(char *printable_format, ...);
void print_i(char *printable_format, ...);
void print_w(char *printable_format, ...);
void print_b(char *printable_format, ...);

#endif // __TRACE_H__
