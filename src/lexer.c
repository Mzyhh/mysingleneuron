#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

tNode *__expr(stringstream *ss);
tNode *__term(stringstream *ss);
tNode *__factor(stringstream *ss);

int isvalidVarChar(const char c) {
    return isalpha(c) || c == '_';
}

int isOp(Token t) {
    return t.type == Op;
}

Token getToken(struct stringstream *ss) {
    Token result = {.type = None, .text = NULL, .len = -1};
    int old_offset = ss->offset;
    while (isspace(ss->data[ss->offset]) && !ss->isempty(ss)) ss->offset++;
    if(ss->isempty(ss)) {
        return result;
    }
    size_t end = ss->offset;
    if (isalpha(ss->data[end])) {
        while (isvalidVarChar(ss->data[end]) && !ss->isempty(ss)) end++;
        result.text = ss->data + ss->offset;
        result.len = end - ss->offset;
        result.type = Var;
    } else if (isdigit(ss->data[end])) {
        int dotflag = 0;
        while(isdigit(ss->data[end]) || ss->data[end] == '.') {
            if (ss->data[end] == '.') {
                if (dotflag == 1) break;
                dotflag = 1;
            }
            end++;
        }
        result.text = ss->data + ss->offset;
        result.len = end - ss->offset;
        result.type = Num;
    } else if (ss->data[end] == '(') {
        result.text = ss->data + ss->offset;
        result.len = 1;
        result.type = Open;
    } else if (ss->data[end] == ')') {
        result.text = ss->data + ss->offset;
        result.len = 1;
        result.type = Close;
    } else {
        result.text = ss->data + ss->offset;
        result.len = 1;
        result.type = Op;
    }
    ss->offset += result.len;
    return result;
}

int backToken(struct stringstream *ss, Token t) {
    if (t.text + t.len != ss->data + ss->offset) return 1; // Token not from this ss
    ss->offset -= t.len;
    return 0;
}

tNode *__expr(stringstream *ss) {
    if (ss->isempty(ss)) 
        return NULL;
    tNode *left = __term(ss);
    Token t = getToken(ss);
    if (t.type == None){
        return left;
    }
    if(t.text[0] == '+' || t.text[0] == '-') {
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = __expr(ss);
        return op;
    }
    return left;
}

tNode *__factor(stringstream *ss) {
    if (ss->isempty(ss)) 
        return NULL;
    Token t = getToken(ss);
    if(t.type == None) {
        return NULL;
    }
    if (t.type == Open) {
        return __expr(ss);
    }
    tNode *result = (tNode*)malloc(sizeof(tNode));
    MAKE_LEAF(result, t);
    return result;
}

tNode *__term(stringstream *ss) {
    tNode *left = __factor(ss);
    Token t = getToken(ss);
    if (t.type == None || !isOp(t) || t.text[0] == '+' || t.text[0] == '-') {
        backToken(ss, t);
        return left;
    }
    tNode *right = __term(ss);
    tNode *op = (tNode*)malloc(sizeof(tNode));
    op->item = t;
    op->left = left;
    op->right = right;
    return op;
}

tNode *createSyntaxTree(stringstream *ss) {
    tNode* root = NULL;
    if (!ss->isempty(ss)) 
        root = __expr(ss);
    return root;
}
