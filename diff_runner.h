#ifndef HW1_DIFF_RUNNER_H
#define HW1_DIFF_RUNNER_H

#include "optsparser.h"

/**
 * Dati due buffer di più linee, esegui il diff.
 * A seconda delle opzioni passate, verrà modificato l'output su stdout.
 *
 * @param buffer1 Primo buffer da confrontare
 * @param buffer2 Secondo buffer da confrontare
 * @param buffer_lines Linee contenute nei due buffer
 * @param prev_lines Offset delle linee rispetto al contenuto vero del file
 * @param options Opzioni da considerare nell'esecuzione del diff
 * @return 1 se ci sono differenze, 0 altrimenti
 */
int run_diff(const char **buffer1, const char **buffer2, int buffer_lines, int prev_lines,
             const struct opt_parsed *options);

#endif //HW1_DIFF_RUNNER_H
