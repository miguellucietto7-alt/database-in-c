#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "read.h"



int main(void)
{
    while (1)
    {
        printf("\n>> ");
        TokenList* list = tokenize(readLine(), ", ");
        for (size_t i = 0; i < list->count; i++)
        {
            printf("Token %zu => %s\n", i + 1, list->tokens[i]);
        }
    }
    
    return 0;
}