#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read.h"
#include "utils.h"
#include "db.h"


void insert_table(DataBase* db, Table* table)
{
    if (!db || !table) return;

    if (db->cap == db->count)
    {
        db->cap *= 2;
        Table** tmp = realloc(db->tables, db->cap * sizeof(Table*));
        if (!tmp)return;
        db->tables = tmp;
    }
    db->tables[db->count] = table;
    db->count++;
}

void create_table(DataBase* db, const string name)
{
    if (!db || !name) return;

    Table* table = malloc(sizeof(Table));
    if (!table) return;

    table->name = strdup(name);
    table->collums = NULL;
    table->collums_count = 0;
    table->collums_cap = 4;
    table->rows = NULL;
    table->rows_count = 0;
    table->rows_cap = 16;

    insert_table(db, table);
}

Table* search_table(DataBase* db, const string name)
{
    if (!db || !name) return NULL;

    for (size_t i = 0; i < db->count; i++)
    {
        if (strcmp(db->tables[i]->name, name) == 0) return db->tables[i];
    }
    return NULL;
}

void insert_collum(Table* table, string collum, size_t index)
{
    if (!table || !collum)return;

    if (table->collums_cap == table->collums_count)
    {
        table->collums_cap *= 2;
        string* tmp = realloc(table->collums, table->collums_cap);
        if (!tmp) return;
        table->collums = tmp;
    }
    table->collums[index] = strdup(collum);
}

void define_table(DataBase* db, const string name, string* collums, size_t elements_count)
{
    if (!db || !name || !collums || !*collums) return;
    Table* table = search_table(db, name);
    if (!table) return;

    for (size_t i = 0; i < elements_count; i++)
    {
        insert_collum(table, collums[i], i);
    }
}

TokenList* tokenize(string line)
{
    if (!line) return NULL;
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;

    list->count = 0;
    list->cap = 8;
    list->line = strdup(line);
    list->tokens = malloc(list->cap * sizeof(string));

    string token;
    token = strtok(line, " ");

    while(token != NULL)
    {
        if (list->cap == list->count)
        {
            list->cap *= 2;
            string* tmp = realloc(list->tokens, list->cap * sizeof(string));
            if (!tmp)
            {
                free(list->tokens);
                return NULL;
            }
            list->tokens = tmp;
        }
        list->tokens[list->count] = strdup(token);
        list->count++;
        token = strtok(NULL, " ");
    }
    return list;
}

int main(void)
{

}