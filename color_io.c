#include "color_io.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

int printf_ansi(const char *ansi_code, const char *format, ...) {
    // Leggi i variadic parameters dai ..., da passare a printf
    va_list args;
    va_start(args, format);

    int result;
    if (isatty(STDOUT_FILENO)) {
        // Sono su un terminale TTY, usa l'output colorato
        printf("%s", ansi_code);
        result = vprintf(format, args); // Serve Vprintf perchè stiamo usando va_list
        printf("%s", ANSI_RESET_PREFIX);
    } else {
        // Non è un TTY (es: file), usa la printf classica
        result = printf(format, args);
    }

    // Dealloca gli argomenti
    va_end(args);
    return result;
}
