/*
 * expr ::= term + expr | term - expr | term
 * term ::= factor*factor | factor/factor | factor
 * factor :: = operand | ( expr )
 * operand ::= number | var
 * var ::= letter+
 * letter ::= [_a-zA-Z]
 * number = 0 | digitnozero
 * digitnozero = 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 */

#ifndef __LEXER_H__
#define __LEXER_H__

#include "tree.h"

typedef struct parserToken {
    enum {Op, Num, Var, Open, Close} type;
    union {
        char varname[MAX_TOKEN_LEN];
        double number;
    } data;
} Token;

#endif //__LEXER_H__
