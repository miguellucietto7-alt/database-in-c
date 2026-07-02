#ifndef QUERY_H
#define QUERY_H

#include <stdbool.h>
#include <tokens.h>

typedef struct DeleteQuery DeleteQuery;
typedef struct SelectQuery SelectQuery;

typedef struct DeleteQuery {
    char* table_name;
    
    char* col_condition;
    char* value_condition;
}DeleteQuery;

typedef struct SelectQuery {
    char* table_name;

    bool all_cols;
    TokenList* cols;

    bool condition;
    char* col_condition;
    char* value_condition;
}SelectQuery;

void parse_query(TokenList*);

#endif