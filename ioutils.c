#include "ioutils.h"
#include <unistd.h>
#include <fcntl.h>

int redirect_stdout(const char* filename) {
    if (close(1) == -1) return -1;
    if (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644) == -1) return -1;
    return 0;
}