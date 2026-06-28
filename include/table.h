#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include "database.h"

typedef struct Table Table;
typedef struct Table {
    const char* name;
    const char* path_name;

    char** cols;
    size_t cols_count;
    size_t cols_cap;

    char*** rows;
    size_t rows_count;
    size_t rows_cap;
    
    Table* next;
}Table;


void create_table(DataBase*, const char*);
void define_table(DataBase*, const char*);


#endif