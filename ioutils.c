#include "ioutils.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int redirect_stdout(const char *filename) {
    if (close(1) == -1) return -1;
    if (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644) == -1) return -1;
    return 0;
}

int open_read_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        fprintf(stderr, "Errore nell'apertura del file %s: %s\n", filename, strerror(errno));
    return fd;
}