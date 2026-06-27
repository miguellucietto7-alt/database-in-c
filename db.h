#ifndef DB_H
#define DB_H

#include <stdio.h>

#define string char*

typedef struct TokenList TokenList;
typedef struct Table Table;
typedef struct DataBase DataBase;

typedef struct TokenList {
    string* tokens;
    size_t count;
    size_t cap;
}TokenList;

typedef struct Table {
    string name;

    string* collums;
    size_t collums_count;
    size_t collums_cap;

    string** rows;
    size_t rows_count;
    size_t rows_cap;
}Table;

typedef struct DataBase {
    Table** tables;
    size_t cap;
    size_t count;
}DataBase;



void save_db(DataBase*);
void load_db();

Table* search_table(DataBase*, const string);
string search_in_table(DataBase*, const string, const string);

void define_table(DataBase*, const string, string*, size_t);
void create_table(DataBase*, const string);
void insert_table(DataBase*, Table*);
void insert_collum(Table*, string, size_t);

TokenList* tokenize(string);
void free_tokens(TokenList*);

void exec_command(string*, size_t);

#endif