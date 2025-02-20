#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "stringstream.h"


int getToken(struct stringstream *ss, char *dest) {
    while (isspace(ss->data[ss->offset]) && !ss->isempty(ss)) ss->offset++;
    int code = ss->isempty(ss);
    size_t start = ss->offset;
    if (isalnum(ss->data[start])) {
        while (isalnum(ss->data[ss->offset]) && !ss->isempty(ss)) ss->offset++;
    } else {
        ss->offset++;
    }
    size_t end = ss->offset;
    strncpy(dest, ss->data + start, end - start);
    dest[end-start] = '\0';
    return code;
}

int main() {
    char *buffer = (char*)malloc(sizeof(char)*128);
    char *s = (char*)malloc(sizeof(char)*128);
    size_t n = 128;
    getline(&s, &n, stdin); 
    INIT_SS(ss, s);
    int code;
    while (!(code = getToken(&ss, buffer)))
        printf("%d %s\n", code, buffer);
    printf("%d\n", code);
}
