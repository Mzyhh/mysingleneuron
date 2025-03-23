/*
 * <expr> ::= <term> + <expr> | <term> - <expr> | <term>
 * <term> ::= <factor>*<term> | <factor>/<term> | <factor>
 * <factor> ::= <primary> ^ <factor> | <primary>
 * <primary> :: = <operand> | ( <expr> )
 * <operand> ::= <number> | <var> | <constant>
 * <constant> ::= e | pi | i
 * <var> ::= <letter>[<letter><digit>]* (except for <constant>)
 * <letter> ::= [_a-zA-Z]
 * <number> ::= <digit>+(\.<digit>+)?
 * <digit> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 */

#ifndef __LEXER_H__
#define __LEXER_H__

#include "stringstream.h"
#include "lexer.h"


#define CREATE_LEAF(node, it)\
    tNode *node = (tNode*)malloc(sizeof(tNode));\
    node->item = (it);\
    node->left = NULL;\
    node->right = NULL;

typedef struct treeNode {
    token item; 
    struct treeNode* left;
    struct treeNode* right;
} tNode;

tNode *createAST(stringstream *ss);

#endif //__LEXER_H__
