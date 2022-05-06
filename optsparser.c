#include "optsparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_unnamed_option(struct opt_parsed *output, const char *argv_string, int *current_unnamed_option);

void parse_named_argument(struct opt_parsed *output, const char *argv_string, char *waiting_for_argument);

int parse_named_options(struct opt_parsed *output, const char *argv_string, char *waiting_for_argument,
                        const opt_descriptors_t descriptors);

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
int parse_options(const int argc, const char **argv, const opt_descriptors_t descriptors, struct opt_parsed *output) {
    // Conta quanti argomenti obbligatori senza nome mi aspetto (es: file1, file2)
    const int unnamed_options = count_unnamed_options(descriptors);

    if (argc <= unnamed_options) {
        fprintf(stderr, "Non ci sono abbastanza opzioni\n\n");
        return -1;
    }

    // Indice della prossima opzione senza nome attesa da leggere
    int current_unnamed_option = 0;

    // Leggi le opzioni con nome (-o, -v, ...)
    // Indica se mi aspetto di leggere un parametro
    // (> zero, è la lettera dell'opzione di cui attendo il parametro)
    char waiting_for_argument = '\0';
    for (int i = 1; i < argc; i++) {
        int is_parsing_option = argv[i][0] == '-';

        if (!is_parsing_option && !waiting_for_argument && current_unnamed_option < unnamed_options) {
            // Ho letto un argomento, non inizia per - quindi non è un'opzione,
            // non mi sto aspettando argomenti per l'opzione precedente,
            // e non avevo finito di leggere gli argomenti senza opzione (es: nomi dei file).
            // Allora, leggi quella.
            parse_unnamed_option(output, argv[i], &current_unnamed_option);
        } else if (is_parsing_option != (waiting_for_argument == '\0')) {
            // Ricevuto diverso da ciò che mi aspettavo
            fprintf(stderr, "Parametro dell'opzione atteso e ricevuta opzione, o viceversa\n\n");
            return -1;
        } else if (waiting_for_argument) {
            // Leggi il parametro dell'opzione
            parse_named_argument(output, argv[i], &waiting_for_argument);
        } else if (argv[i][1] == '\0') {
            // Opzione vuota (-), diamo solo warning
            fprintf(stderr, "WARN: opzione vuota trovata\n");
        } else if (parse_named_options(output, argv[i], &waiting_for_argument, descriptors) == -1) {
            // Errore nel parsing sulle opzioni in forma compatta
            return -1;
        }
    }

    // Controlla se gli argomenti sono finiti ma attendevo ancora un argomento
    if (waiting_for_argument || current_unnamed_option < unnamed_options) {
        fprintf(stderr, "Un altro parametro era atteso\n\n");
        return -1;
    }

    return 0;
}

/**
 * Esegui il parsing da argv_string di un'opzione senza nome (es: nome file)
 *
 * @param output Struttura dove raccogliere i risultati del parsing
 * @param argv_string Stringa senza nome di cui fare parsing
 * @param current_unnamed_option Indice dell'opzione senza nome che si sta leggendo
 */
void parse_unnamed_option(struct opt_parsed *output, const char *argv_string, int *current_unnamed_option) {
    // L'opzione è presente: il bit corrispondente all'i-esimo argomento senza nome
    // Vedere i commenti di struct opt_parsed
    output->opts_mask |= 1 << (26 + *current_unnamed_option);
    output->opts_args[26 + *current_unnamed_option] = argv_string;
    (*current_unnamed_option)++;
}

/**
 * Esegui il parsing da argv_string di un argomento di un opzione con nome (es: -o)
 *
 * @param output Struttura dove raccogliere i risultati del parsing
 * @param argv_string Argomento dell'opzione
 * @param waiting_for_argument Carattere dell'opzione di cui si sta leggendo l'argomento
 */
void parse_named_argument(struct opt_parsed *output, const char *argv_string, char *waiting_for_argument) {
    output->opts_args[*waiting_for_argument - 'a'] = argv_string;
    *waiting_for_argument = '\0';
}

/**
 * Esegui il parsing da argv_string di una serie di opzioni, anche in maniera compatta (-uvo)
 *
 * @param output Struttura dove raccogliere i risultati del parsing
 * @param argv_string Stringa con opzioni compatte di cui fare parsing
 * @param waiting_for_argument Carattere dell'opzione di cui si attenderà l'argomento
 * @param descriptors Descrittori delle opzioni
 * @return 0 se tutto ok, -1 in caso di errore
 */
int parse_named_options(struct opt_parsed *output, const char *argv_string, char *waiting_for_argument,
                        const opt_descriptors_t descriptors) {
    for (int char_i = 1; char_i < strlen(argv_string); char_i++) {
        // Leggi la prossima opzione
        const char option = argv_string[char_i];
        const int option_index = option - 'a';

        if (option_index < 0 || option_index >= 26) {
            // Opzione invalida (es: -1, -A)
            fprintf(stderr, "Opzione non valida: -%c\n\n", option);
            return -1;
        }

        // Ottieni indicazione sulle specifiche di questa opzione (se esiste)
        const struct opt_descriptor *descriptor = descriptors[option_index];
        if (descriptor == NULL) {
            // Opzione inesistente
            fprintf(stderr, "Opzione non riconosciuta: -%c\n\n", option);
            return -1;
        }

        // Ho bisogno di leggere subito dopo il parametro o un'altra opzione?
        if (!descriptor->has_attribute) {
            // Nessun argomento richiesto
            *waiting_for_argument = '\0';
        } else if (argv_string[char_i + 1] == '\0') {
            // Mi aspetto un argomento subito dopo
            *waiting_for_argument = option;
        } else {
            // Dopo ho ancora altre opzioni compatte? Sarebbe un errore perché voglio il parametro
            fprintf(stderr, "Atteso argomento per l'opzione -%c\n\n", option);
            return -1;
        }

        // L'opzione è presente nell'output parsed
        opt_set_present(output, option);
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
void opt_add_descriptor(opt_descriptors_t descriptors, char option, const char *description, int has_arg) {
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
void opt_add_unnamed_descriptor(opt_descriptors_t descriptors, int index, const char *name) {
    // Equivale a un'opzione obbligatoria, dopo le lettere alfabetiche
    opt_add_descriptor(descriptors, (char) ('z' + 1 + index), name, 1);
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
const char *opt_get_arg(const struct opt_parsed *parsed, char option) {
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
void print_help(const char *execfile, const opt_descriptors_t descriptors) {
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
