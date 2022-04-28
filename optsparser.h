#ifndef HW1_OPTSPARSER_H
#define HW1_OPTSPARSER_H

#include <inttypes.h>
#include <stddef.h>

/**
 * Descrittore di un'opzione che è possibile avere da linea di comando.
 * Sia le opzioni con parametro che singoli.
 * Contenuto all'interno di opt_descriptors_t.
 */
struct opt_descriptor {
    /**
     * Descrizione dell'opzione, mostrata all'utente nell'help
     */
    char *description;

    /**
     * Indica se questa opzione vuole un attributo
     */
    char has_attribute;
};

/**
 * Lunghezza ideale dell'array di opt_descriptors_t
 */
#define OPT_DESCRIPTORS_LEN 32

/**
 * Array di descrittori delle opzioni.
 *
 * All'indice i, se i < 26 si intende l'opzione con nome corrispondente al numero della lettera.
 * Quindi, [0] = -a, [1] = -b, ...
 * Se i >= 26, si intendono gli argomenti senza nome, obbligatorie, posti prima.
 *
 * Se nella posizione i-esima è presente NULL, l'opzione non è presente
 */
typedef struct opt_descriptor *opt_descriptors_t[OPT_DESCRIPTORS_LEN];

/**
 * Descrive un'opzione ricevuta e di cui è stato eseguito il parsing.
 */
struct opt_parsed {
    /**
     * Indica quali parametri sono stati ricevuti.
     *
     * Il bit corrispondente al numero della lettera del parametro
     * dell'opzione sarà settato se l'opzione è presente.
     */
    uint32_t opts_mask;

    /**
     * Contiene le stringhe delle opzioni.
     *
     * All'indice [numero della lettera del parametro] contiene la stringa attributo, oppure NULL.
     *
     * Esempio: con -a ciao, avremo opts_args[0] = "ciao". Seguono gli argomenti senza opzione
     */
    char const **opts_args;
};

/**
 * Scrivi il messaggio di help, sullo stderr.
 *
 * Indica come utilizzare il comando con le sue opzioni
 *
 * @param execfile
 * @param descriptors
 */
void print_help(const char *execfile, const opt_descriptors_t descriptors);

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
int parse_options(int argc,
                  const char **argv,
                  const opt_descriptors_t descriptors,
                  struct opt_parsed *output);

/**
 * Ottieni il numero di opzioni senza nome obbligatorie sono presenti in un opt_descriptor
 *
 * @param descriptors Descrittore delle opzioni
 */
int count_unnamed_options(const opt_descriptors_t descriptors);

/**
 * Controlla se un'opzione è presente nelle opzioni post-parsing.
 *
 * @param parsed Opzioni post-parsing
 * @param option Carattere nome dell'opzione
 * @return 0 se non è presente, un altro numero altrimenti
 */
int opt_is_present(const struct opt_parsed *parsed, char option);

/**
 * Imposta un'opzione come presente nelle opzioni post-parsing.
 *
 * @param parsed Opzioni post-parsing
 * @param option Carattere nome dell'opzione
 */
void opt_set_present(struct opt_parsed *parsed, char option);

/**
 * Ottieni l'argomento dell'opzione.
 *
 * @param parsed Opzioni post-parsing
 * @param option Carattere nome dell'opzione
 * @return La stringa argomento dell'opzione.
 *          Se l'opzione non è presente o non ha un argomento, il risultato è indefinito.
 */
const char* opt_get_arg(const struct opt_parsed *parsed, char option);

/**
 * Ottieni l'argomento senza nome alla posizione indicata
 *
 * @param parsed Opzioni post-parsing
 * @param option Indice dell'opzione senza posizione
 * @return La stringa argomento dell'opzione.
 *          Se l'opzione non è presente o non ha un argomento, il risultato è indefinito.
 */
const char* opt_get_unnamed_arg(const struct opt_parsed *parsed, int option);

/**
 * Aggiungi un parametro nei descrittori delle opzioni
 *
 * @param descriptors Descrittori delle opzioni
 * @param description Nome dell'opzione visualizzata all'utente
 * @param has_arg Indica se ha bisogno di un argomento extra
 */
void opt_add_descriptor(opt_descriptors_t descriptors, char option, const char* description, int has_arg);

/**
 * Aggiungi un parametro senza nome nei descrittori dell'input
 *
 * @param descriptors Descrittori delle opzioni
 * @param index Indice dell'opzione dell'input
 * @param name Nome dell'opzione visualizzata all'utente
 */
void opt_add_unnamed_descriptor(opt_descriptors_t descriptors, int index, const char* name);

/**
 * Ripulisci la memoria allocata dai descrittori dell'input.
 *
 * @param descriptors Descrittori da ripulire
 */
void free_descriptor(const opt_descriptors_t descriptors);

#endif //HW1_OPTSPARSER_H
