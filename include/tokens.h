#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h>

typedef struct TokenList TokenList;

typedef struct TokenList {
    char** tokens;
    size_t count;
    size_t cap;
}TokenList;


TokenList* tokenize(char* line, const char* delim);
void free_tokens(TokenList*);

#endif