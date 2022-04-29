#include "ioutils.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void stdout_redirect_if_requested(const struct opt_parsed *parsed_options) {
    if (opt_is_present(parsed_options, 'o')) {
        const char *outfile = opt_get_arg(parsed_options, 'o');
        if (redirect_stdout(outfile) == -1)
            fprintf(stderr, "Errore nella redirezione di stdout su %s: %s\n", outfile, strerror(errno));
    }
}

int redirect_stdout(const char *filename) {
    if (close(STDOUT_FILENO) == -1) return -1;
    if (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644) == -1) return -1;
    return 0;
}

FILE* open_read_file(const char *filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        fprintf(stderr, "Errore nell'apertura del file %s: %s\n", filename, strerror(errno));
    return file;
}

int read_lines(FILE *file, char **buffer) {
    ssize_t lasterror = 0;
    int i;
    for (i = 0; i < LINES_TO_READ && lasterror >= 0; i++) {
        free(buffer[i]);
        char* line = NULL; // Verrà allocato da getline, ma dovrà essere liberato a mano
        size_t alloc_size = 0; // Spazio allocato nel buffer line
        lasterror = getline(&line, &alloc_size, file);
        buffer[i] = line;
    }

    if (errno != 0) {
        // Errore diverso da EOF, che invece ha solo lasterror = -1
        perror("Errore nella lettura della riga del file");
        return -1;
    }

    if (i < LINES_TO_READ) {
        // Non sono state lette tutte le linee.
        // Termina con NULL
        buffer[i] = NULL;
    }

    return i - 1;
}

void free_buffer(char **buffer) {
    for (int i = 0; i < LINES_TO_READ && buffer[i] != NULL; i++)
        free(buffer[i]);
}