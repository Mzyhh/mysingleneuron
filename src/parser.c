#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

static tNode *__expr(stringstream *ss);
static tNode *__term(stringstream *ss);
static tNode *__factor(stringstream *ss);


static tNode *__expr(stringstream *ss) {
    if (stringstreamIsEmpty(ss)) 
        return NULL;
    tNode *left = __term(ss);
    token t = getKnownToken(ss);
    if (t.type == ExprOp) {
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = __expr(ss);
        return op;
    }
    return left;
}

static tNode *__factor(stringstream *ss) {
    if (stringstreamIsEmpty(ss)) 
        return NULL;
    token t = getKnownToken(ss);
    if(t.type == None) {
        return NULL;
    }
    if (t.type == Open) {
        CREATE_LEAF(result, t);
        result->left = __expr(ss);
        return result;
    }
    CREATE_LEAF(result, t);
    return result;
}

static tNode *__term(stringstream *ss) {
    tNode *left = __factor(ss);
    token t = getKnownToken(ss);
    if (t.type == Close) {
        CREATE_LEAF(close, t);
        left->left = close;
    }
    if (t.type == TermOp) {
        tNode *right = __term(ss);
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = right;
        return op;
    }
    backToken(ss, t);
    return left;
}

tNode *createAST(stringstream *ss) {
    tNode* root = NULL;
    if (!stringstreamIsEmpty(ss)) 
        root = __expr(ss);
    return root;
}
