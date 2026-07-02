#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>

typedef struct DataBase DataBase;
typedef struct Table Table;

typedef struct Table {
    char* name;

    char** cols;
    size_t cols_cap;
    size_t cols_count;

    char*** rows;
    size_t rows_cap;
    size_t rows_count;
    
    Table* next;
}Table;

Table* search_table(DataBase*, const char*);
void create_table(DataBase*, const char*);

#endif