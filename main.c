#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "optsparser.h"
#include "ioutils.h"
#include "diff_runner.h"
#include "color_io.h"

/**
 * Gestisci le opzioni da input, leggendo le stringhe in ARGV
 *
 * @param argc Numero di argomenti in argv
 * @param argv Argomenti ricevuti da CLI
 * @param parsed_options Output del parsing delle opzioni ricevute
 * @return 0 se è tutto ok, -1 se c'è un errore
 */
int parse_input(int argc, const char **argv, struct opt_parsed *parsed_options);

/**
 * Restituisce il numero minore dei due passati in input.
 * Se i due numeri sono diversi, imposta il flag a 1.
 *
 * @param a Primo numero
 * @param b Secondo numero
 * @param flag Flag impostato a 1 se i due numeri sono diversi
 * @return Il numero minore dei due
 */
inline int min_diff(int a, int b, int *flag) {
    if (a != b) {
        printf("Linee diverse %d %d\n", a, b);
        *flag = 1;
    }

    return a < b ? a : b;
}

int main(int argc, const char **argv) {
    // Esegui il parsing degli argomenti
    const char *args[OPT_DESCRIPTORS_LEN] = {0};
    struct opt_parsed parsed_options = {0, args};
    if (parse_input(argc, argv, &parsed_options) == -1) {
        return EXIT_FAILURE;
    }

    // Effettua redirect output stdout -> file se -o è presente
    stdout_redirect_if_requested(&parsed_options);

    // Apri i due file
    const char *filename1 = opt_get_unnamed_arg(&parsed_options, 0);
    const char *filename2 = opt_get_unnamed_arg(&parsed_options, 1);
    FILE *file1 = open_read_file(filename1);
    FILE *file2 = open_read_file(filename2);
    if (file1 == NULL || file2 == NULL)
        return EXIT_FAILURE;

    // Effettua la lettura in buffer da 50 righe, facendo il diff
    int diff_result = 0; // Indica se i file sono uguali (0) o diversi (1)
    char *buffer1[LINES_TO_READ] = { 0 };
    char *buffer2[LINES_TO_READ] = { 0 };
    int num_lines; // Numero minimo di righe lette
    int prev_lines = 0; // Linee lette nelle iterazioni precedenti

    do {
        // Leggi il primo file, tenendo conto delle linee lette effettive
        num_lines = read_lines(file1, buffer1);

        if (num_lines > 0) {
            // Non leggere se non ho letto nulla, o se c'è stato un errore (-1)
            num_lines = min_diff(read_lines(file2, buffer2), num_lines, &diff_result);
        }

        if (num_lines > 0) {
            // Sono state lette delle linee, al più considerane num_lines
            // Esegui il vero e proprio diff
            diff_result |= run_diff((const char **) buffer1,
                                    (const char **) buffer2,
                                    num_lines,
                                    prev_lines,
                                    &parsed_options);
        }

        prev_lines += num_lines;
    } while (num_lines > 0);

    // Chiudi i file
    fclose(file1);
    fclose(file2);

    if (num_lines < 0) {
        // Si è verificato un errore
        return EXIT_FAILURE;
    }

    // Free delle linee lette nel buffer. Devono essere liberate manualmente.
    free_buffer(buffer1);
    free_buffer(buffer2);

    int show_different_summary = opt_is_present(&parsed_options, 'g');
    int show_identical_summary = opt_is_present(&parsed_options, 's');

    if (show_different_summary && diff_result) {
        printf_ansi(ANSI_BOLD_PREFIX, "I file %s e %s sono differenti\n", filename1, filename2);
    } else if (show_identical_summary && !diff_result) {
        printf_ansi(ANSI_BOLD_PREFIX, "I file %s e %s sono identici\n", filename1, filename2);
    }

    // Come il vero diff, l'exit code è 1 se diversi, 0 se uguali
    return diff_result;
}

int parse_input(int argc, const char **argv, struct opt_parsed *parsed_options) {
    // Specifica le opzioni che ci aspettiamo
    opt_descriptors_t options = {0};
    opt_add_unnamed_descriptor(options, 0, "FILE_1");
    opt_add_unnamed_descriptor(options, 1, "FILE_2");
    opt_add_descriptor(options, 'g', "Stampa una riga esplicita nel caso i file siano diversi", 0);
    opt_add_descriptor(options, 's', "Stampa una riga esplicita nel caso i file siano uguali", 0);
    opt_add_descriptor(options, 'd', "Stampa i numeri delle righe diverse", 0);
    opt_add_descriptor(options, 'u', "Stampa i numeri delle righe uguali", 0);
    opt_add_descriptor(options, 'v', "Stampa le singole selezionate (uguali o diverse)", 0);
    opt_add_descriptor(options, 'o', "Redireziona l'output su di un file", 1);

    // Prova ad eseguire il parsing, oppure mostra il messaggio di utilizzo
    if (parse_options(argc, argv, options, parsed_options) == -1) {
        print_help(argv[0], options);
        free_descriptor(options);
        return -1;
    }

    // Libera la memoria allocata dai descrittori delle opzioni argv
    // (non servono più dopo aver fatto il parsing, servirà solo il risultato di questa azione)
    free_descriptor(options);

    // Controlla opzioni mutuamente esclusive o casi particolari
    if (opt_is_present(parsed_options, 'd') && opt_is_present(parsed_options, 'u')) {
        fprintf(stderr, "Le opzioni -d e -u sono mutuamente esclusive\n");
        return -1;
    }
    return 0;
}
