#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdlib.h>
#include "stringstream.h"

#define NEXPR_OPS 2
#define NTERM_OPS 2
#define NCONSTANTS 3

typedef struct parserToken {
    enum {None, Unknown, 
        ExprOp, TermOp, FactorOp,
        Num, Var, Constant,
        Open, Close} type;
    char *text;
    size_t len;
} token;

token getToken(struct stringstream *ss);
int backToken(struct stringstream *ss, token t);
token getKnownToken(stringstream *ss);

#endif // __TOKEN_H__
