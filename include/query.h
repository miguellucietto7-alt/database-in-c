#ifndef QUERY_H
#define QUERY_H

#include "tokens.h"
#include "database.h"

typedef struct SelectQuery SelectQuery;
typedef struct DeleteQuery DeleteQuery;
typedef struct InsertQuery InsertQuery;


typedef struct SelectQuery {
    char* table_name;

    bool select_all;
    TokenList* col_list;

    bool condition;
    char* col_condition;
    char* value_condition;
}SelectQuery;


SelectQuery* select_query(char*);
DeleteQuery* delete_query(char*);
InsertQuery* insert_query(char*);

void delete_table(DataBase*, const char*);


#endif