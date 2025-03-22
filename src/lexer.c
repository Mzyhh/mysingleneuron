#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "stringstream.h"


int isvalidVarChar(const char c) {
    return isalpha(c) || c == '_';
}

token getNumberToken(struct stringstream *ss) {
    token result;
    int dotflag = 0;
    int start = ss->offset;
    while(isdigit(ss->data[ss->offset]) || ss->data[ss->offset] == '.') {
        if (ss->data[ss->offset] == '.') {
            if (dotflag == 1) break;
            dotflag = 1;
        }
        ss->offset++;
    }
    result.text = ss->data + start;
    result.type = Num;
    result.len = ss->offset - start;
    return result;
}

token getVarToken(struct stringstream *ss) {
    int start = ss->offset;
    while (isvalidVarChar(ss->data[ss->offset]) && !stringstreamIsEmpty(ss))
        ss->offset++;
    token result;
    result.text = ss->data + start;
    result.len = ss->offset - start;
    result.type = Var;
    return result;
}

token getToken(struct stringstream *ss) {
    token result = {.type = None, .text = NULL, .len = -1};
    int old_offset = ss->offset;
    while (isspace(ss->data[ss->offset]) && !stringstreamIsEmpty(ss)) ss->offset++;
    if(stringstreamIsEmpty(ss)) {
        return result;
    }
    size_t end = ss->offset;
    if (isalpha(ss->data[end])) {
        return getVarToken(ss);
    } 
    if (isdigit(ss->data[end])) {
        return getNumberToken(ss);
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

int backToken(struct stringstream *ss, token t) {
    if (t.text + t.len != ss->data + ss->offset) return 1; // Token not from this ss
    ss->offset -= t.len;
    return 0;
}
