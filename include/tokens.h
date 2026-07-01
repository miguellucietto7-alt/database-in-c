#ifndef TOKENS_H
#define TOKENS_H

typedef struct TokenList TokenList;

typedef struct TokenList {
    char** tokens;
    size_t count;
    size_t cap;
}TokenList;


TokenList* tokenize(char* line, const char* delim);


#endif