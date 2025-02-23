#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "stringstream.h"
#include "tree.h"

int isvalidVarChar(char c) {
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
    Token t;
    while (!getToken(&ss, &t)) {
        if (t.type == Var) {
            printf("Var: %s\n", t.data.varname);
        } else if(t.type == Num) {
            printf("Num: %lf\n", t.data.number);
        } else if(t.type == Open) {
            printf("Open bracket\n");
        } else if(t.type == Close) {
            printf("Close bracket\n");
        } else {
            printf("Op: %s\n", t.data.varname);
        }
    }
}
