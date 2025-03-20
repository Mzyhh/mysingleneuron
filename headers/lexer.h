/*
 * expr ::= term + expr | term - expr | term
 * term ::= factor*factor | factor/factor | factor
 * factor :: = operand | ( expr )
 * operand ::= number | var
 * var ::= letter+
 * letter ::= [_a-zA-Z]
 * number = 0 | digitnozero
 * digitnozero = 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 */

#ifndef __LEXER_H__
#define __LEXER_H__

#include "stringstream.h"

#define MAX_TOKEN_LEN 16

typedef struct parserToken {
//    enum {Op, Num, Var, Open, Close} type;
    char *text;
    size_t len;
} Token;

int isvalidVarChar(const char c);
int getToken(struct stringstream *ss, Token *dest);


#define CREATE_LEAF(node, it)\
    tNode *node = (tNode*)malloc(sizeof(tNode));\
    node->item = (it);\
    node->left = NULL;\
    node->right = NULL;
#define MAKE_LEAF(node, it)\
    node->item = (it);\
    node->left = NULL;\
    node->right = NULL;


typedef struct treeNode {
    Token *item; 
    struct treeNode* left;
    struct treeNode* right;
} tNode;

tNode *createSyntaxTree(stringstream *ss);

#endif //__LEXER_H__
