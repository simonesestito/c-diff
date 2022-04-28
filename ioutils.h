#ifndef HW1_IOUTILS_H
#define HW1_IOUTILS_H

/**
 * Effettua la redirezione di stdout su di un file di testo
 *
 * @param filename Nome del file dove scrivere
 * @return -1 in caso di errore, 0 altrimenti
 */
int redirect_stdout(const char* filename);

#endif //HW1_IOUTILS_H
