#include <stdlib.h>
#include <stdio.h>
#include "optsparser.h"

void add_cli_options(opt_descriptors_t options);

int main(int argc, const char **argv) {
    opt_descriptors_t expected_options = {0};
    const char *args[OPT_DESCRIPTORS_LEN] = {0};
    struct opt_parsed parsed_options = {0, args};

    add_cli_options(expected_options);
    if (parse_options(argc, argv, expected_options, &parsed_options) == -1) {
        print_help(argv[0], expected_options);
        return EXIT_FAILURE;
    }

    free_descriptor(expected_options);

    for (int i = 0; i < 2; i++) {
        printf("Argomento %d: %s\n", i, opt_get_unnamed_arg(&parsed_options, i));
    }

    printf("Opzione -%c presente? %d\n", 'g', opt_is_present(&parsed_options, 'g'));
    printf("Opzione -%c presente? %d\n", 's', opt_is_present(&parsed_options, 's'));
    printf("Opzione -%c presente? %d\n", 'd', opt_is_present(&parsed_options, 'd'));
    printf("Opzione -%c presente? %d\n", 'u', opt_is_present(&parsed_options, 'u'));
    printf("Opzione -%c presente? %d\n", 'v', opt_is_present(&parsed_options, 'v'));
    printf("Opzione -%c presente? %d\n", 'o', opt_is_present(&parsed_options, 'o'));
    if (opt_is_present(&parsed_options, 'o'))
        printf("Parametro di -%c: %s\n", 'o', opt_get_arg(&parsed_options, 'o'));
}

void add_cli_options(opt_descriptors_t options) {
    opt_add_unnamed_descriptor(options, 0, "FILE_1");
    opt_add_unnamed_descriptor(options, 1, "FILE_2");
    opt_add_descriptor(options, 'g', "Stampa una riga esplicita nel caso i file siano diversi", 0);
    opt_add_descriptor(options, 's', "Stampa una riga esplicita nel caso i file siano uguali", 0);
    opt_add_descriptor(options, 'd', "Stampa i numeri delle righe diverse", 0);
    opt_add_descriptor(options, 'u', "Stampa i numeri delle righe uguali", 0);
    opt_add_descriptor(options, 'v', "Stampa le singole selezionate (uguali o diverse)", 0);
    opt_add_descriptor(options, 'o', "Redireziona l'output su di un file", 1);
}