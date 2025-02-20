#include <stdlib.h>

#ifndef __STRINGSTREAM_H__
#define __STRINGSTREAM_H__

#define INIT_SS(name, str) struct stringstream name;\
                      stringstreamInit(&name, str);

typedef struct stringstream {
    char* data;
    size_t offset;

    int (*isempty)(struct stringstream*);
}stringstream;

int stringstreamIsEmpty(stringstream *ss);

void stringstreamInit(stringstream *ss, char *str);

#endif // __STRINGSTREAM_H__
