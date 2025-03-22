#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdlib.h>
#include "stringstream.h"

typedef struct parserToken {
    enum {None, Op, Num, Var, Open, Close} type;
    char *text;
    size_t len;
} token;

token getToken(struct stringstream *ss);
int backToken(struct stringstream *ss, token t);

#endif // __TOKEN_H__
