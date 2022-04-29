#include "optsparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * A partire dai parametri passati in input, determina
 * quali opzioni sono state passate e ne esegue la validazione.
 *
 * @param argc Numero degli argomenti
 * @param argv Argomenti ricevuti dal sistema operativo
 * @param descriptors Descrivono le varie opzioni possibili e aspettate
 * @param output Scrive le opzioni successivamente al parsing
 * @return 0 se è riuscito, -1 se c'è un errore.
 *          Con un errore, è opportuno mostrare il messaggio di help.
 */
int parse_options(const int argc,
                  const char **argv,
                  const opt_descriptors_t descriptors,
                  struct opt_parsed *output) {
    // Conta quanti argomenti obbligatori senza nome mi aspetto (es: file1, file2)
    const int unnamed_options = count_unnamed_options(descriptors);

    if (argc <= unnamed_options) {
        fprintf(stderr, "Non ci sono abbastanza opzioni\n\n");
        return -1;
    }

    // Leggi prima i parametri senza nome (es: i nomi dei file d'input)
    for (int i = 1; i <= unnamed_options; i++) {
        if (argv[i][0] == '-') {
            // Mi aspettavo un'opzione senza nome
            // Ho ricevuto un'opzione con nome
            fprintf(stderr, "Argomento inaspettato\n\n");
            return -1;
        }

        // L'opzione è presente: il bit corrispondente all'i-esimo argomento senza nome
        // Vedere i commenti di struct opt_parsed
        output->opts_mask |= 1 << (25 + i);
        output->opts_args[25 + i] = argv[i];
    }

    // Leggi le opzioni con nome (-o, -v, ...)
    // Indica se mi aspetto di leggere un parametro
    // (> zero, è la lettera dell'opzione di cui attendo il parametro)
    char waiting_for_argument = 0;
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
            // Leggi la prossima opzione
            const char option = argv[i][1];
            const int option_index = option - 'a';
            if (option_index < 0 || option_index >= 26 || argv[i][2] != '\0') {
                // Opzione invalida (es: -1, -A, -parola)
                fprintf(stderr, "Opzione non valida: %s\n\n", argv[i]);
                return -1;
            }
            // Ottieni indicazione sulle specifiche di questa opzione (se esiste)
            const struct opt_descriptor *descriptor = descriptors[option_index];
            if (descriptor == NULL) {
                // Opzione inesistente
                fprintf(stderr, "Opzione non riconosciuta: %s\n\n", argv[i]);
                return -1;
            }

            // Ho bisogno di leggere subito dopo il parametro o un'altra opzione?
            waiting_for_argument = descriptor->has_attribute ? option : '\0';
            opt_set_present(output, option); // L'opzione è presente nell'output parsed
        }
    }

    // Controlla se gli argomenti sono finiti ma attendevo ancora un argomento
    if (waiting_for_argument) {
        fprintf(stderr, "Parametro dell'opzione atteso\n\n");
        return -1;
    }

    return 0;
}

/**
 * Ottieni il numero di opzioni senza nome obbligatorie sono presenti in un opt_descriptor
 *
 * @param descriptors Descrittore delle opzioni
 */
int count_unnamed_options(const opt_descriptors_t descriptors) {
    int i = 26;
    // opt_descriptors_t può terminare con NULL, oppure essere pieno
    while (i < OPT_DESCRIPTORS_LEN && descriptors[i] != NULL) {
        i++;
    }

    return i - 26;
}

/**
 * Aggiungi un parametro nei descrittori delle opzioni
 *
 * @param descriptors Descrittori delle opzioni
 * @param description Nome dell'opzione visualizzata all'utente
 * @param has_arg Indica se ha bisogno di un argomento extra
 */
void opt_add_descriptor(opt_descriptors_t descriptors, char option, const char* description, int has_arg) {
    int i = option - 'a';
    // Alloca una struttura
    descriptors[i] = malloc(sizeof(struct opt_descriptor));
    // Copia la descrizione, evitando l'eliminazione
    descriptors[i]->description = description;
    // Assegna gli altri attributi semplici
    descriptors[i]->has_attribute = has_arg;
}

/**
 * Aggiungi un parametro senza nome nei descrittori dell'input
 *
 * @param descriptors Descrittori delle opzioni
 * @param index Indice dell'opzione dell'input
 * @param name Nome dell'opzione visualizzata all'utente
 */
void opt_add_unnamed_descriptor(opt_descriptors_t descriptors, int index, const char* name) {
    // Equivale a un'opzione obbligatoria, dopo le lettere alfabetiche
    opt_add_descriptor(descriptors, (char)('z' + 1 + index), name, 1);
}

/**
 * Controlla se un'opzione è presente nelle opzioni post-parsing.
 *
 * @param parsed Opzioni post-parsing
 * @param option Carattere nome dell'opzione
 * @return 0 se non è presente, un altro numero altrimenti
 */
unsigned int opt_is_present(const struct opt_parsed *parsed, char option) {
    return parsed->opts_mask & (1u << (option - 'a'));
}

/**
 * Imposta un'opzione come presente nelle opzioni post-parsing.
 *
 * @param parsed Opzioni post-parsing
 * @param option Carattere nome dell'opzione
 */
void opt_set_present(struct opt_parsed *parsed, char option) {
    parsed->opts_mask |= 1 << (option - 'a');
}

/**
 * Ottieni l'argomento dell'opzione.
 *
 * @param parsed Opzioni post-parsing
 * @param option Carattere nome dell'opzione
 * @return La stringa argomento dell'opzione.
 *          Se l'opzione non è presente o non ha un argomento, il risultato è indefinito.
 */
const char* opt_get_arg(const struct opt_parsed *parsed, char option) {
    return parsed->opts_args[option - 'a'];
}

/**
 * Ottieni l'argomento senza nome alla posizione indicata
 *
 * @param parsed Opzioni post-parsing
 * @param option Indice dell'opzione senza posizione
 * @return La stringa argomento dell'opzione.
 *          Se l'opzione non è presente o non ha un argomento, il risultato è indefinito.
 */
const char* opt_get_unnamed_arg(const struct opt_parsed *parsed, int option) {
    return parsed->opts_args[option + 26];
}

/**
 * Ripulisci la memoria allocata dai descrittori dell'input.
 *
 * @param descriptors Descrittori da ripulire
 */
void free_descriptor(const opt_descriptors_t descriptors) {
    for (int i = 0; i < OPT_DESCRIPTORS_LEN; i++) {
        // Libera ogni descrittore, fino alla fine (NULL)
        if (descriptors[i] == NULL)
            continue;

        free(descriptors[i]);
    }
}

/**
 * Scrivi il messaggio di help, sullo stderr.
 *
 * Indica come utilizzare il comando con le sue opzioni
 *
 * @param execfile
 * @param descriptors
 */
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
