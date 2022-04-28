#ifndef HW1_IOUTILS_H
#define HW1_IOUTILS_H

#include <stdio.h>
#include "optsparser.h"

#define LINES_TO_READ 50

/**
 * Effettua redirect output stdout -> file, se opzione -o è presente
 *
 * @param parsed_options Opzioni da cui leggere i dati
 */
void stdout_redirect_if_requested(const struct opt_parsed *parsed_options);

/**
 * Effettua la redirezione di stdout su di un file di testo
 *
 * @param filename Nome del file dove scrivere
 * @return -1 in caso di errore, 0 altrimenti
 */
int redirect_stdout(const char* filename);

/**
 * Apri un file in sola lettura, gestendo già internamente
 * i messaggi di errore sta stampare all'utente.
 *
 * Rimuove del codice duplicato / ridondante.
 *
 * @param filename Nome del file da aprire
 * @return Il puntatore alla struct FILE, o NULL
 */
FILE* open_read_file(const char *filename);

/**
 * Leggi le prossime LINES_TO_READ linee dal file.
 *
 * Termina prima in caso di EOF o altri errori.
 *
 * @param file File da cui leggere
 * @param buffer Buffer dove scrivere le linee lette.
 * @return Linee lette, o -1 in caso di errore diverso da EOF
 */
int read_lines(FILE *file, char **buffer);

/**
 * Libera la memoria allocata dalle linee del buffer
 *
 * @param buffer Buffer le cui linee sono da deallocare
 */
void free_buffer(char **buffer);

#endif //HW1_IOUTILS_H
