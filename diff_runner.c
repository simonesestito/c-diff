//
// Created by Simone on 28/04/2022.
//

#include "diff_runner.h"

int run_diff(const char **buffer1, const char **buffer2, int buffer_lines, int prev_lines,
             const struct opt_parsed *options) {
    const char *filename1 = opt_get_unnamed_arg(options, 0);
    const char *filename2 = opt_get_unnamed_arg(options, 1);

    // TODO: Creativo: fare output del diff colorato

   // TODO -g = Stampa una riga esplicita nel caso i file siano diversi
   // TODO -s = Stampa una riga esplicita nel caso i file siano uguali
   // TODO -d = Stampa i numeri delle righe diverse
   // TODO -u = Stampa i numeri delle righe uguali
   // TODO -v = Stampa le singole selezionate (uguali o diverse)
}