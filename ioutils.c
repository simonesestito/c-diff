#include "ioutils.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void stdout_redirect_if_requested(const struct opt_parsed *parsed_options) {
    // Controlla se l'opzione -o è presente
    if (opt_is_present(parsed_options, 'o')) {
        const char *outfile = opt_get_arg(parsed_options, 'o');
        // Se c'è, prova a fare redirect dello stdout
        if (redirect_stdout(outfile) == -1)
            fprintf(stderr, "Errore nella redirezione di stdout su %s: %s\n", outfile, strerror(errno));
    }
}

int redirect_stdout(const char *filename) {
    // Chiudi il file descriptor dello stdout
    if (close(STDOUT_FILENO) == -1) return -1;
    // Apri il nuovo file.
    // Gli verrà assegnato il primo disponibile, ovvero proprio quello dello stdout appena chiuso
    if (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644) == -1) return -1;
    return 0;
}

FILE* open_read_file(const char *filename) {
    // Apri un file in sola lettura, gestendo l'output di errore
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        fprintf(stderr, "Errore nell'apertura del file %s: %s\n", filename, strerror(errno));
    return file;
}

int read_lines(FILE *file, char **buffer) {
    // Indica gli ultimi caratteri letti, o se negativo, l'avvenuto errore (EOF o altro)
    ssize_t lasterror = 0;

    // Leggi 50 linee di fila
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
    // Libera tutte le linee allocate da getline() nel buffer
    for (int i = 0; i < LINES_TO_READ && buffer[i] != NULL; i++)
        free(buffer[i]);
}