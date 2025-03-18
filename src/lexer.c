#include "stringstream.h"
#include "lexer.h"
#include <ctype.h>
#include <string.h>

tNode *__expr(stringstream *ss);
tNode *__term(stringstream *ss);
tNode *__factor(stringstream *ss);

int isvalidVarChar(const char c) {
    return isalpha(c) || c == '_';
}

int getToken(struct stringstream *ss, Token* dest) {
    int old_offset = ss->offset;
    while (isspace(ss->data[ss->offset]) && !ss->isempty(ss)) ss->offset++;
    if(ss->isempty(ss)) {
        return 1;
    }
    size_t start = ss->offset;
    if (isalpha(ss->data[start])) {
        while (isvalidVarChar(ss->data[ss->offset]) && !ss->isempty(ss)) ss->offset++;
        size_t end = ss->offset;
        dest->type = Var;
        strncpy(dest->data.varname, ss->data + start, end - start);
    } else if (isdigit(ss->data[start])) {
        dest->type = Num;
        dest->data.number = atof(ss->data + start);
        while(isdigit(ss->data[ss->offset]) || ss->data[ss->offset] == '.') 
            ss->offset++;
         
    } else if (ss->data[start] == '(') {
        dest->type = Open;
        ss->offset++;
    } else if (ss->data[start] == ')') {
        dest->type = Close;
        ss->offset++;
    } else {
        dest->type = Op;
        dest->data.varname[0] = ss->data[ss->offset++];
        dest->data.varname[1] = '\0';
    }

    return EXIT_SUCCESS;
}

tNode *__expr(stringstream *ss) {
    if (ss->isempty(ss)) 
        return NULL;
    tNode *left = __term(ss);
    Token *t = (Token*)malloc(sizeof(Token));
    getToken(ss, t); 
    if(t->type == Op && (t->data.varname[0]=='+'||t->data.varname[0]=='-')) {
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = __expr(ss);
    }
    return left;
}

tNode *__factor(stringstream *ss) {
    if (ss->isempty(ss)) 
        return NULL;
    Token *t = (Token*)malloc(sizeof(Token));
    getToken(ss, t);
    if (t->type ==  Open) {
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
    if (t->type != Op) return left;
    if (t->type == Op &&
       (t->data.varname[0] == '+' || t->data.varname[0] == '-')) return left;
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
