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

int isOp(Token *t) {
    char c = t->text[0];
    return c == '-' || c == '+' || c == '*' || c == '/';
}

int pickToken(struct stringstream *ss, Token* dest) {
    int old_offset = ss->offset;
    while (isspace(ss->data[ss->offset]) && !ss->isempty(ss)) ss->offset++;
    if(ss->isempty(ss)) {
        return 1;
    }
    size_t end = ss->offset;
    if (isalpha(ss->data[end])) {
        while (isvalidVarChar(ss->data[end]) && !ss->isempty(ss)) end++;
        dest->text = ss->data + ss->offset;
        dest->len = end - ss->offset;
    } else if (isdigit(ss->data[end])) {
        int dotflag = 0;
        while(isdigit(ss->data[end]) || ss->data[end] == '.') {
            if (ss->data[end] == '.') {
                if (dotflag == 1) break;
                dotflag = 1;
            }
            end++;
        }
        dest->text = ss->data + ss->offset;
        dest->len = end - ss->offset;
    } else if (ss->data[end] == '(') {
        dest->text = ss->data + ss->offset;
        dest->len = 1;
    } else if (ss->data[end] == ')') {
        dest->text = ss->data + ss->offset;
        dest->len = 1;
    } else {
        dest->text = ss->data + ss->offset;
        dest->len = 1;
    }
    return EXIT_SUCCESS;
}

int getToken(struct stringstream *ss, Token* dest) {
    int code = pickToken(ss, dest);
    ss->offset += dest->len; 
    return code;
}

tNode *__expr(stringstream *ss) {
    if (ss->isempty(ss)) 
        return NULL;
    tNode *left = __term(ss);
    Token *t = (Token*)malloc(sizeof(Token));
    if (getToken(ss, t)) {
        free(t);
        return left;
    }
    if(t->text[0] == '+' || t->text[0] == '-') {
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
    Token *t = (Token*)malloc(sizeof(Token));
    if(getToken(ss, t)) {
        free(t);
        return NULL;
    }    if (t->text[0] ==  '(') {
        free(t);
        return __expr(ss);
    }
    tNode *result = (tNode*)malloc(sizeof(tNode));
    MAKE_LEAF(result, t);
    return result;
}

tNode *__term(stringstream *ss) {
    tNode *left = __factor(ss);
    Token *t = (Token*)malloc(sizeof(Token));
    if(pickToken(ss, t) || !isOp(t)) {
        free(t);
        return left;
    }
    if (t->text[0] == '+' || t->text[0] == '-') {
        free(t);
        return left;
    }
    getToken(ss, t);
    tNode *right = __factor(ss);
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
