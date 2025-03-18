#include <stdio.h>
#include <string.h>

#include "stringstream.h"
#include "tree.h"

tNode *createSemanticTree(stringstream *ss) {
    size_t buffer_size = strlen(ss->data)*2;
    char *buffer = (char*)malloc(sizeof(char)*buffer_size); // temporary
    size_t offset = 0;
    return NULL;
}


//int main() {
//    char *buffer = (char*)malloc(sizeof(char)*128);
//    char *s = (char*)malloc(sizeof(char)*128);
//    size_t n = 128;
//    getline(&s, &n, stdin); 
//    INIT_SS(ss, s);
//    Token t;
//    while (!getToken(&ss, &t)) {
//        if (t.type == Var) {
//            printf("Var: %s\n", t.data.varname);
//        } else if(t.type == Num) {
//            printf("Num: %lf\n", t.data.number);
//        } else if(t.type == Open) {
//            printf("Open bracket\n");
//        } else if(t.type == Close) {
//            printf("Close bracket\n");
//        } else {
//            printf("Op: %s\n", t.data.varname);
//        }
//    }
//}
