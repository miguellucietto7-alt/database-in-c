#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include "utils.h"

static TokenList* create_token_list()
{
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;

    list->cap = 8;
    list->count = 0;
    list->tokens = malloc(list->cap * sizeof(char*));
    
    if (!list->tokens)
    {
        free(list);
        return NULL;
    }

    return list;
}

static void token_insert(TokenList* list, const char* token)
{
    if (!list || !token) return;

    if (list->count + 1 >= list->cap)
    {
        size_t new_cap = list->cap * 2;
        char** temp = realloc(list->tokens, new_cap * sizeof(char*));
        if (!temp) return;
        list->cap = new_cap;
        list->tokens = temp;
    }
    list->tokens[list->count] = strdup(token);
    list->count++;
}

TokenList* tokenize(char* _line, const char* delim)
{
    if (!_line || !delim) return NULL;
    TokenList* list = create_token_list();
    char* line = strdup(_line);

    char* token = strtok(line, delim);
    while (token != NULL)
    {
        token_insert(list, token);
        token = strtok(NULL, delim);
    }
    free(line);
    return list;
}

void free_tokens(TokenList* list)
{
    if (!list) return;
    for (size_t i = 0; i < list->count; i++)
    {
        free(list->tokens[i]);
    }
    free(list->tokens);
    free(list);
}
