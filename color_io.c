#include "color_io.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

/**
 * Esegue una print su stdout, includendo il codice ANSI dalle costanti,
 * tali da rendere colorato o modificare lo stile dell'output.
 *
 * @param ansi_code Codice di controllo ANSI
 * @param format Formato dell'output di printf
 * @param ... Argomenti di printf
 * @return Output di printf
 */
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
        // Non è un TTY (es: file), usa la printf classica (vprintf per via di va_list)
        result = vprintf(format, args);
        errno = 0; // Reset del errno (isatty lo imposta), altrimenti crea problema ad altre funzioni successivamente.
    }

    // Dealloca gli argomenti
    va_end(args);
    return result;
}
