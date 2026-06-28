#ifndef TOKENS_H
#define TOKENS_H

typedef struct TokenList TokenList;

typedef struct TokenList {
    char** tokens;
    unsigned int count;
    unsigned int cap;
}TokenList;

TokenList* tokenize(char*);

#endif