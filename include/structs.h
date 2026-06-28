#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdbool.h>

#define string char*


typedef struct SelectQuery SelectQuery;
typedef struct TokenList TokenList;
typedef struct Table Table;
typedef struct DataBase DataBase;


typedef struct TokenList {
    string* tokens;
    size_t count;
    size_t cap;
}TokenList;

typedef struct SelectQuery {
    string table_name;

    bool select_all;
    TokenList* col_list;

    bool condition;
    string condition_col;
    string condition_value;
}SelectQuery;

typedef struct Table {
    string name;

    string* collumns;
    size_t collumns_count;
    size_t collumns_cap;

    string** rows;
    size_t rows_count;
    size_t rows_cap;

    Table* next;
}Table;

typedef struct DataBase {
    Table** tables;
    size_t count;
    size_t cap;
}DataBase;

#endif