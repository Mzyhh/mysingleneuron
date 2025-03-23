#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "lexer.h"
#include "stringstream.h"


static const char EXPR_OPS[] = "+-";
static const char TERM_OPS[] = "*/";
static const char FACTOR_OP = '^'; 
static const char *CONSTANTS[] = {"e", "pi", "i"};

static int isExprOp(char c) {
    for (int i = 0; EXPR_OPS[i] != '\0'; ++i) {
        if (c == EXPR_OPS[i]) return 1;
    }
    return 0;
}

static int isTermOp(char c) {
    for (int i = 0; TERM_OPS[i] != '\0'; ++i) {
        if (c == TERM_OPS[i]) return 1;
    }
    return 0;
}

static int isFactorOp(char c) {
    return c == FACTOR_OP;
}

static int isOp(char c) {
    return isExprOp(c) || isTermOp(c) || isFactorOp(c);
}

static int isvalidVarChar(const char c) {
    return isalpha(c) || c == '_' || isdigit(c);
}

static int isConstant(char *str, size_t len) {
    for (int i = 0; i < NCONSTANTS; ++i) {
        if (!strncmp(str, CONSTANTS[i], len)) return 1;
    }
    return 0;
}

token getNumberToken(struct stringstream *ss) {
    token result;
    int dotflag = 0;
    int start = ss->offset;
    while(isdigit(ss->data[ss->offset]) || ss->data[ss->offset] == '.') {
        if (ss->data[ss->offset] == '.') {
            if (dotflag == 1) break;
            dotflag = 1;
        }
        ss->offset++;
    }
    result.text = ss->data + start;
    result.type = Num;
    result.len = ss->offset - start;
    return result;
}


token getVarConstantToken(struct stringstream *ss) {
    int start = ss->offset;
    while (isvalidVarChar(ss->data[ss->offset]) && !stringstreamIsEmpty(ss))
        ss->offset++;
    token result;
    result.text = ss->data + start;
    result.len = ss->offset - start;
    result.type = isConstant(result.text, result.len) ? Constant : Var;
    return result;
}

token getToken(struct stringstream *ss) {
    token result = {.type = None, .text = NULL, .len = -1};
    int old_offset = ss->offset;
    while (isspace(ss->data[ss->offset]) && !stringstreamIsEmpty(ss)) ss->offset++;
    if(stringstreamIsEmpty(ss)) {
        return result;
    }
    size_t end = ss->offset;
    if (isalpha(ss->data[end])) {
        return getVarConstantToken(ss);
    } 
    if (isdigit(ss->data[end])) {
        return getNumberToken(ss);
    } else if (ss->data[end] == '(') {
        result.text = ss->data + ss->offset;
        result.len = 1;
        result.type = Open;
    } else if (ss->data[end] == ')') {
        result.text = ss->data + ss->offset;
        result.len = 1;
        result.type = Close;
    } else if (isOp(ss->data[end])) {
        if (isExprOp(ss->data[end])) result.type = ExprOp;
        if (isTermOp(ss->data[end])) result.type = TermOp;
        if (isFactorOp(ss->data[end])) result.type = FactorOp;
        result.len = 1;
        result.text = ss->data + ss->offset;
    } else {
        result.text = ss->data + ss->offset;
        result.len = 1;
        result.type = Unknown;
    }
    ss->offset += result.len;
    return result;
}

int backToken(struct stringstream *ss, token t) {
    if (t.text + t.len != ss->data + ss->offset) return 1; // Token not from this ss
    ss->offset -= t.len;
    return 0;
}

token getKnownToken(stringstream *ss) {
    token t;
    while ((t = getToken(ss)).type == Unknown) {
        fprintf(stderr, "Error: unknown token on %dth position: ", (int)(ss->offset - t.len));
        fprintf(stderr, "%.*s\n", (int)t.len, t.text);
    }
    return t;
}
