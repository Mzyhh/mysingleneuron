#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

static tNode *parseExpression(stringstream *ss);
static tNode *parseTerm(stringstream *ss);
static tNode *parsePrimary(stringstream *ss);
static tNode *parseFactor(stringstream *ss);


static tNode *parseExpression(stringstream *ss) {
    if (stringstreamIsEmpty(ss)) 
        return NULL;
    tNode *left = parseTerm(ss);
    token t = getKnownToken(ss);
    if (t.type == ExprOp) {
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = parseExpression(ss);
        return op;
    }
    return left;
}

static tNode *parseTerm(stringstream *ss) {
    tNode *left = parseFactor(ss);
    token t = getKnownToken(ss);
    if (t.type == Close) {
        CREATE_LEAF(close, t);
        left->left = close;
    }
    if (t.type == TermOp) {
        tNode *right = parseTerm(ss);
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = right;
        return op;
    }
    backToken(ss, t);
    return left;
}

static tNode *parseFactor(stringstream *ss) {
    if (stringstreamIsEmpty(ss)) 
        return NULL;
    tNode *left = parsePrimary(ss);
    token t = getKnownToken(ss);
    if (t.type == FactorOp) {
        tNode *right = parseFactor(ss);
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = right;
        return op;
    }
    backToken(ss, t);
    return left;
}

static tNode *parsePrimary(stringstream *ss) {
    if (stringstreamIsEmpty(ss)) 
        return NULL;
    token t = getKnownToken(ss);
    if(t.type == None) {
        return NULL;
    }
    if (t.type == Open) {
        CREATE_LEAF(result, t);
        result->left = parseExpression(ss);
        return result;
    }
    CREATE_LEAF(result, t);
    return result;
}

tNode *createAST(stringstream *ss) {
    tNode* root = NULL;
    if (!stringstreamIsEmpty(ss)) 
        root = parseExpression(ss);
    return root;
}
