#ifndef HW1_IOUTILS_H
#define HW1_IOUTILS_H

#define LINES_TO_READ 50

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
 * @return -1 se c'è un errore, il file descriptor altrimenti
 */
int open_read_file(const char *filename);

// TODO: read_next_lines con LINES_TO_READ

#endif //HW1_IOUTILS_H
