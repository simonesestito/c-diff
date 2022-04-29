#ifndef HW1_COLOR_IO_H
#define HW1_COLOR_IO_H
#include <wchar.h>

#define ANSI_RESET_PREFIX       "\x1b[0m"
#define ANSI_BOLD_PREFIX        "\x1b[1m"
#define ANSI_RED_PREFIX         "\x1b[91m"
#define ANSI_RED_BOLD_PREFIX    "\x1b[1;41m"
#define ANSI_GREEN_PREFIX       "\x1b[92m"
#define ANSI_GREEN_BOLD_PREFIX  "\x1b[1;42m"
#define ANSI_BLUE_PREFIX        "\x1b[34m"
#define ANSI_BLUE_BOLD_PREFIX   "\x1b[1;44m"

/**
 * Esegue una print su stdout, includendo il codice ANSI dalle costanti,
 * tali da rendere colorato o modificare lo stile dell'output.
 *
 * @param ansi_code Codice di controllo ANSI
 * @param format Formato dell'output di printf
 * @param ... Argomenti di printf
 * @return Output di printf
 */
int printf_ansi(const char *ansi_code, const char *format, ...);

#endif //HW1_COLOR_IO_H
