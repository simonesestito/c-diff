#include "optsparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_options(const int argc,
                  const char **argv,
                  const opt_descriptors_t descriptors,
                  struct opt_parsed *output) {
    const int unnamed_options = count_unnamed_options(descriptors);

    if (argc <= unnamed_options) {
        fprintf(stderr, "Non ci sono abbastanza opzioni\n\n");
        return -1;
    }

    // Leggi prima i parametri senza nome (es: i nomi dei file di input)
    for (int i = 1; i <= unnamed_options; i++) {
        if (argv[i][0] == '-') {
            // Mi aspettavo un'opzione senza nome
            // Ho ricevuto un'opzione con nome
            fprintf(stderr, "Argomento inaspettato\n\n");
            return -1;
        }

        output->opts_mask |= 1 << (25 + i);
        output->opts_args[25 + i] = argv[i];
    }

    // Leggi le opzioni
    char waiting_for_argument = 0; // Mi aspetto di leggere un parametro, se >0
    for (int i = unnamed_options + 1; i < argc; i++) {
        if ((argv[i][0] == '-') != (waiting_for_argument == 0)) {
            // Ricevuto diverso da ciò che mi aspettavo
            fprintf(stderr, "Parametro dell'opzione atteso, o l'opposto\n\n");
            return -1;
        }

        if (waiting_for_argument) {
            // Leggi il parametro dell'opzione
            output->opts_args[waiting_for_argument - 'a'] = argv[i];
            waiting_for_argument = 0;
        } else {
            // Leggi l'opzione
            const char option = argv[i][1];
            const int option_index = option - 'a';
            if (option_index < 0 || option_index >= 26 || argv[i][2] != '\0') {
                // Opzione invalida
                fprintf(stderr, "Opzione non valida: %s\n\n", argv[i]);
                return -1;
            }
            const struct opt_descriptor *descriptor = descriptors[option_index];
            if (descriptor == NULL) {
                // Opzione inesistente
                fprintf(stderr, "Opzione non riconosciuta: %s\n\n", argv[i]);
                return -1;
            }

            waiting_for_argument = descriptor->has_attribute ? option : 0;
            opt_set_present(output, option);
        }
    }

    // Controlla se gli argomenti sono finiti ma attendevo ancora un argomento
    if (waiting_for_argument) {
        fprintf(stderr, "Parametro dell'opzione atteso\n\n");
        return -1;
    }

    return 0;
}

int count_unnamed_options(const opt_descriptors_t descriptors) {
    int i = 26;
    while (i < OPT_DESCRIPTORS_LEN && descriptors[i] != NULL) {
        i++;
    }

    return i - 26;
}

void opt_add_descriptor(opt_descriptors_t descriptors, char option, const char* description, int has_arg) {
    int i = option - 'a';
    // Alloca una struttura
    descriptors[i] = malloc(sizeof(struct opt_descriptor));
    // Copia la descrizione, evitando l'eliminazione
    descriptors[i]->description = calloc(strlen(description), sizeof(char));
    strcpy(descriptors[i]->description, description);
    // Assegna gli altri attributi semplici
    descriptors[i]->has_attribute = has_arg;
}

void opt_add_unnamed_descriptor(opt_descriptors_t descriptors, int index, const char* name) {
    opt_add_descriptor(descriptors, 'z' + 1 + index, name, 1);
}

int opt_is_present(const struct opt_parsed *parsed, char option) {
    return parsed->opts_mask & (1 << (option - 'a'));
}

void opt_set_present(struct opt_parsed *parsed, char option) {
    parsed->opts_mask |= 1 << (option - 'a');
}

const char* opt_get_arg(const struct opt_parsed *parsed, char option) {
    return parsed->opts_args[option - 'a'];
}

const char* opt_get_unnamed_arg(const struct opt_parsed *parsed, int option) {
    return parsed->opts_args[option + 26];
}

void free_descriptor(const opt_descriptors_t descriptors) {
    for (int i = 0; i < OPT_DESCRIPTORS_LEN; i++) {
        if (descriptors[i] == NULL)
            continue;

        free((char*) descriptors[i]->description);
        free(descriptors[i]);
    }
}

void print_help(const char* execfile, const opt_descriptors_t descriptors) {
    fprintf(stderr, "Utilizzo:\n");
    fprintf(stderr, "%s", execfile);

    // Stampa gli argomenti senza nome
    const int unnamed_options = count_unnamed_options(descriptors);
    for (int i = 0; i < unnamed_options; i++) {
        fprintf(stderr, " [%s]", descriptors[26 + i]->description);
    }

    // Descrivi le opzioni vere e proprie
    fprintf(stderr, " [...OPZIONI]\n");
    for (int i = 0; i < 26; i++) {
        const struct opt_descriptor *descriptor = descriptors[i];
        if (descriptor != NULL) {
            fprintf(stderr, "-%c %s", 'a' + i, descriptor->description);
            if (descriptor->has_attribute)
                fprintf(stderr, " [...]");
            fprintf(stderr, "\n");
        }
    }
}
