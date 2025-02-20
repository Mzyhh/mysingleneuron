#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "stringstream.h"
#include "tree.h"

int isvalidVarChar(char c) {
    return isalpha(c) || c == '_';
}

Token *getToken(struct stringstream *ss) {
    int old_offset = ss->offset;
    while (isspace(ss->data[ss->offset]) && !ss->isempty(ss)) ss->offset++;
    if(ss->isempty(ss)) {
        return NULL;
    }
    size_t start = ss->offset;
    Token *t;
    if (isalpha(ss->data[start])) {
        while (isvalidVarChar(ss->data[ss->offset]) && !ss->isempty(ss)) ss->offset++;
        size_t end = ss->offset;
        t = (Token*)malloc(sizeof(Token) + sizeof(char)*(end-start));
        t->type = Var;
        t->varname = (char*)(t+sizeof(Token));
        strncpy(t->varname, ss->data + start, end - start);
    } else if (isdigit(ss->data[start])) {
        t = (Token*)malloc(sizeof(Token));
        t->type = Num;
        t->number = atof(ss->data + start);
        while(isdigit(ss->data[ss->offset]) || ss->data[ss->offset] == '.') 
            ss->offset++;
         
    } else if (ss->data[start] == '(') {
        t = (Token*)malloc(sizeof(Token));
        t->type = Open;
        ss->offset++;
    } else if (ss->data[start] == ')') {
        t = (Token*)malloc(sizeof(Token));
        t->type = Close;
        ss->offset++;
    } else {
        t = (Token*)malloc(sizeof(Token) + sizeof(char));
        t->type = Op;
        t->varname = (char*)(t + sizeof(Token));
        t->varname[0] = ss->data[ss->offset++];
    }
    return t;
}

tNode *createSemanticTree(stringstream *ss) {
    size_t buffer_size = strlen(ss->data)*2;
    char *buffer = (char*)malloc(sizeof(char)*buffer_size); // temporary
    size_t offset = 0;
    return NULL;
}


int main() {
    char *buffer = (char*)malloc(sizeof(char)*128);
    char *s = (char*)malloc(sizeof(char)*128);
    size_t n = 128;
    getline(&s, &n, stdin); 
    INIT_SS(ss, s);
    Token *t;
    while ((t = getToken(&ss))) {
        if (t->type == Var) {
            printf("Var: %s\n", t->varname);
        } else if(t->type == Num) {
            printf("Num: %lf\n", t->number);
        } else if(t->type == Open) {
            printf("Open bracket\n");
        } else if(t->type == Close) {
            printf("Close bracket\n");
        } else {
            printf("Op: %s\n", t->varname);
        }
        free(t);
    }
}
