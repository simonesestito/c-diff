#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "optsparser.h"
#include "ioutils.h"

/**
 * Gestisci le opzioni da input, leggendo le stringhe in ARGV
 *
 * @param argc Numero di argomenti in argv
 * @param argv Argomenti ricevuti da CLI
 * @param parsed_options Output del parsing delle opzioni ricevute
 * @return 0 se è tutto ok, -1 se c'è un errore
 */
int parse_input(int argc, const char **argv, struct opt_parsed *parsed_options);

int main(int argc, const char **argv) {
    const char *args[OPT_DESCRIPTORS_LEN] = {0};
    struct opt_parsed parsed_options = {0, args};
    if (parse_input(argc, argv, &parsed_options) == -1) {
        return EXIT_FAILURE;
    }

    // Effettua redirect output stdout -> file se -o è presente
    if (opt_is_present(&parsed_options, 'o')) {
        const char *outfile = opt_get_arg(&parsed_options, 'o');
        if (redirect_stdout(outfile) == -1)
            fprintf(stderr, "Errore nella redirezione di stdout su %s: %s\n", outfile, strerror(errno));
    }

    int file1 = open_read_file(opt_get_unnamed_arg(&parsed_options, 0));
    int file2 = open_read_file(opt_get_unnamed_arg(&parsed_options, 1));
    if (file1 == -1 || file2 == -1)
        return EXIT_FAILURE;

    // TODO: Leggi linee read_next_lines(), gestendo tutti gli errori
    // TODO: Per ogni riga, gestisci il diff con strcmp
    // TODO: Gestire tutte le opzioni

    return EXIT_SUCCESS; // TODO: Exit code come il vero diff
}


int parse_input(int argc, const char **argv, struct opt_parsed *parsed_options) {
    opt_descriptors_t options = {0};
    opt_add_unnamed_descriptor(options, 0, "FILE_1");
    opt_add_unnamed_descriptor(options, 1, "FILE_2");
    opt_add_descriptor(options, 'g', "Stampa una riga esplicita nel caso i file siano diversi", 0);
    opt_add_descriptor(options, 's', "Stampa una riga esplicita nel caso i file siano uguali", 0);
    opt_add_descriptor(options, 'd', "Stampa i numeri delle righe diverse", 0);
    opt_add_descriptor(options, 'u', "Stampa i numeri delle righe uguali", 0);
    opt_add_descriptor(options, 'v', "Stampa le singole selezionate (uguali o diverse)", 0);
    opt_add_descriptor(options, 'o', "Redireziona l'output su di un file", 1);

    if (parse_options(argc, argv, options, parsed_options) == -1) {
        print_help(argv[0], options);
        return -1;
    }

    free_descriptor(options);

    // Controlla opzioni mutuamente esclusive
    if (opt_is_present(parsed_options, 'd') && opt_is_present(parsed_options, 'u')) {
        fprintf(stderr, "Le opzioni -d e -u sono mutuamente esclusive\n");
        return -1;
    }
    return 0;
}