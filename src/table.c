#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "database.h"
#include "utils.h"

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
        free(table);
        return NULL;
    }
    return table;
}

void create_table(DataBase* db, const char* name)
{
    Table* table = new_table(name);
    if (!name || !db || !table) return NULL;

    size_t index = hash(name) % db->cap;
    Table* head = db->buckets[index];
    table->next = head;
    db->buckets[index] = table;
}