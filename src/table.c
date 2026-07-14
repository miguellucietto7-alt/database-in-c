#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include "table.h"
#include "database.h"
#include "utils.h"

Table* search_table(DataBase* db, const char* name)
{
    if (!db || !name) return NULL;

    size_t index = hash(name) % db->cap;
    Table* head = db->buckets[index];
    if (!head) return NULL;

    for (Table* table = head; table != NULL; table = table->next)
    {
        if (strcmp(table->name, name) == 0) return table;
    }
    return NULL;
}

static Table* new_table(const char* name)
{
    if (!name) return NULL;

    Table* table = malloc(sizeof(Table));
    if (!table) return NULL;

    table->name = strdup(name);

    table->cols_cap = 8;
    table->cols_count = 0;
    table->rows_cap = 8;
    table->rows_count = 0;

    table->rows = malloc(sizeof(char**) * table->rows_cap);
    table->cols = malloc(sizeof(char*) * table->cols_cap);
    if (!table->cols || !table->rows)
    {
        free(table->name);
        free(table);
        return NULL;
    }
    return table;
}

void create_table(DataBase* db, const char* name)
{
    Table* table = new_table(name);
    if (!name || !db || !table) return;

    size_t index = hash(name) % db->cap;
    Table* head = db->buckets[index];
    table->next = head;
    db->buckets[index] = table;
}

void insert_col(Table* table, char* col)
{
    if (!table || !col || !table->cols) return;

    if (table->cols_count + 1 >= table->cols_cap)
    {
        size_t new_cap = table->cols_cap * 2;
        char** temp = realloc(table->cols, new_cap * sizeof(char*));
        if (!temp) return;
        table->cols = temp;
    }
    table->cols[table->cols_count++] = strdup(col);
}

void insert_row(Table* table, char** row)
{
    if (!table || !row) return;

    if (table->rows_count + 1 >= table->rows_cap)
    {
        size_t ncap = table->rows_cap * 2;
        char*** temp = realloc(table->rows, ncap * sizeof(char**));
        if (!temp) return;
        table->rows = temp;
        table->rows_cap = ncap;
    }
    for (size_t i = 0; i < table->cols_count; i++)
    {
        table->rows[table->rows_count][i] = strdup(row[i]);
    }
    table->rows_count++;
}

void define_table(DataBase* db, const char* name, TokenList* list)
{
    Table* table = search_table(db, name);
    if (!db || !name || !list || !table) return;

    for (size_t i = 0; i < list->count; i++)
    {
        insert_col(table, list->tokens[i]);
    }
}