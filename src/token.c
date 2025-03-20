#include <string.h>
#include <ctype.h>
#include "token.h"
#include "stringstream.h"


int isvalidVarChar(const char c) {
    return isalpha(c) || c == '_';
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
