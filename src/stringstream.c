#include "stringstream.h"

int stringstreamIsEmpty(stringstream *ss) {
    return ss->data[ss->offset] == '\0';
}

int stringstreamInit(stringstream *ss, char *str) {
    ss->data = str; 
    ss->offset = 0;

    return EXIT_SUCCESS;
}
