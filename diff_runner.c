#include "diff_runner.h"
#include "color_io.h"
#include <string.h>

/**
 * Mostra le linee diverse. Gestisci l'output in base ai flag utente.
 *
 * @param line1 Linea diversa nel file 1
 * @param line2 Linea diversa nel file 2
 * @param filename1 Nome del file 1
 * @param filename2 Nome del file 2
 * @param line_index Indice della linea all'interno dei due file
 * @param verbose_output Indica se l'output deve essere verboso
 */
void handle_different_output(const char *line1, const char *line2, const char *filename1, const char *filename2,
                             unsigned int line_index, unsigned int verbose_output);

/**
 * Mostra le linee identiche. Gestisci l'output in base ai flag utente.
 *
 * @param line Linea identica nei due file
 * @param filename1 Nome del file 1
 * @param filename2 Nome del file 2
 * @param line_index Indice della linea all'interno dei due file
 * @param verbose_output Indica se l'output deve essere verboso
 */
void handle_identical_output(const char *line, const char *filename1, const char *filename2, unsigned int line_index,
                             unsigned int verbose_output);

/**
 * Dati due buffer di più linee, esegui il diff.
 * A seconda delle opzioni passate, verrà modificato l'output su stdout.
 *
 * @param buffer1 Primo buffer da confrontare
 * @param buffer2 Secondo buffer da confrontare
 * @param buffer_lines Linee contenute nei due buffer
 * @param prev_lines Offset delle linee rispetto al contenuto vero del file
 * @param options Opzioni da considerare nell'esecuzione del diff
 * @return 1 se ci sono differenze, 0 altrimenti
 */
int run_diff(const char **buffer1, const char **buffer2, int buffer_lines, int prev_lines,
             const struct opt_parsed *options) {
    // Ottieni i nomi dei file dalle opzioni da terminale
    const char *filename1 = opt_get_unnamed_arg(options, 0);
    const char *filename2 = opt_get_unnamed_arg(options, 1);

    // Ottieni i dettagli su come mostrare l'output
    unsigned int show_different_lines = opt_is_present(options, 'd');
    unsigned int show_identical_lines = opt_is_present(options, 'u');
    unsigned int verbose_output = opt_is_present(options, 'v');

    // Indica se sono diversi
    int different = 0;

    // Itera sulle linee
    for (int i = 0; i < buffer_lines; i++) {
        // Ottiene le due righe
        const char *line1 = buffer1[i];
        const char *line2 = buffer2[i];
        if (strcmp(line1, line2) == 0) {
            // Le linee sono identiche
            if (show_identical_lines)
                handle_identical_output(line1, filename1, filename2, i + prev_lines, verbose_output);
        } else {
            // Le linee sono diverse
            different = 1;
            if (show_different_lines)
                handle_different_output(line1, line2, filename1, filename2, i + prev_lines, verbose_output);
        }
    }

    return different;
}

void handle_different_output(const char *line1, const char *line2, const char *filename1, const char *filename2,
                             unsigned int line_index, unsigned int verbose_output) {
    if (verbose_output) {
        // Stampa l'output completo, ovvero nome file, riga (inizia da 1!) e contenuto
        printf_ansi(ANSI_RED_BOLD_PREFIX, "#%d, %s:", line_index + 1, filename1);
        printf_ansi(ANSI_RED_PREFIX, " %s", line1);
        printf_ansi(ANSI_GREEN_BOLD_PREFIX, "#%d, %s:", line_index + 1, filename2);
        printf_ansi(ANSI_GREEN_PREFIX, " %s", line2);
        printf_ansi(ANSI_GREEN_PREFIX, "\n");
    } else {
        // Stampa solo la linea diversa
        printf_ansi(ANSI_RED_PREFIX, "%d\n", line_index + 1);
    }
}

void handle_identical_output(const char *line, const char *filename1, const char *filename2, unsigned int line_index,
                             unsigned int verbose_output) {
    if (verbose_output) {
        // Stampa l'output completo, ovvero nome file, riga e contenuto
        printf_ansi(ANSI_BLUE_BOLD_PREFIX, "#%d, %s:", line_index + 1, filename1);
        printf_ansi(ANSI_BLUE_PREFIX, " %s", line);
        printf_ansi(ANSI_BLUE_BOLD_PREFIX, "#%d, %s:", line_index + 1, filename2);
        printf_ansi(ANSI_BLUE_PREFIX, " %s", line);
        printf_ansi(ANSI_BLUE_PREFIX, "\n");
    } else {
        // Stampa solo la linea diversa
        printf_ansi(ANSI_BLUE_PREFIX, "%d\n", line_index + 1);
    }
}