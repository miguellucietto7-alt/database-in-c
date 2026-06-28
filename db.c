#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include "read.h"
#include "utils.h"
#include "db.h"

#define MKDIR(dir) mkdir(dir, 0777)

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
    table->collums_cap = 4;
    table->collums_count = 0;
    table->collums = malloc(table->collums_cap * sizeof(string));
    table->rows_cap = 16;
    table->rows_count = 0;
    table->rows = malloc(table->rows_cap * sizeof(string*));

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
        string* tmp = realloc(table->collums, table->collums_cap * sizeof(string));
        if (!tmp) return;
        table->collums = tmp;
    }
    table->collums[index] = strdup(collum);
    table->collums_count++;
}

void insert_row(Table* table, string* row, size_t index)
{
    if (!table || !row || !*row) return;
    if (table->rows_cap == table->rows_count)
    {
        table->rows_cap  *= 2;
        string** temp = realloc(table->rows, table->rows_cap * sizeof(string*));
        if (!temp) return;
        table->rows = temp;
    }
    table->rows[index] = malloc(table->collums_count * sizeof(string));
    for (size_t i = 0; i < table->collums_count; i++)
    {
        table->rows[index][i] = strdup(row[i]);
    }
    table->rows_count++;
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

TokenList* tokenize(string line, const string delim)
{
    if (!line) return NULL;
    TokenList* list = malloc(sizeof(TokenList));
    if (!list) return NULL;

    list->count = 0;
    list->cap = 8;
    list->tokens = malloc(list->cap * sizeof(string));

    string token;
    token = strtok(line, delim);

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
        token = strtok(NULL, delim);
    }
    return list;
}

void free_tokens(TokenList* list)
{
    if (!list || !list->tokens) return;

    for (size_t i = 0; i < list->count; i++) free(list->tokens[i]);
    free(list->tokens);
    free(list);
}

Table* load_table(string path)
{
    if (!path) return NULL;
    Table* table = malloc(sizeof(Table));
    if(!table) return NULL;
    FILE* file = fopen(path, "r");

    table->collums_cap = 4;
    table->collums_count = 0;
    table->rows_cap = 16;
    table->rows_count = 0;
    table->collums = malloc(table->collums_cap * sizeof(string));
    table->rows = malloc(table->rows_cap * sizeof(string*));

    char name[128];
    fgets(name, sizeof(name), file);
    name[strcspn(name, "\n")] = '\0';
    table->name = strdup(name);

    char line[2048];
    fgets(line, sizeof(line), file);
    TokenList* list = tokenize(line, "|");
    for (size_t i = 0; i < list->count; i++) insert_collum(table, list->tokens[i], i);
    free_tokens(list);
    while(fgets(line, sizeof(line), file) != NULL)
    {
        list = tokenize(line, "|");
        insert_row(table, list->tokens, table->rows_count);
        free_tokens(list);
    }
    fclose(file);
    return table;
}

DataBase* load_db()
{
    DataBase* db = malloc(sizeof(DataBase));
    if (!db) return NULL;

    db->cap = 4;
    db->count = 0;
    db->tables = malloc(db->cap * sizeof(Table*));

    DIR* dir = opendir(".database_migue");
    if (!dir) return db;

    size_t count = 0;

    struct dirent* archive;
    archive = readdir(dir);
    while (archive != NULL)
    {
        if (strcmp(archive->d_name, ".") == 0 || strcmp(archive->d_name, "..") == 0)
        {
            archive = readdir(dir);
            continue;
        }
        
        char path[1028];
        snprintf(path, sizeof(path), ".database_migue/%s", archive->d_name);
        Table* table = load_table(path);
        if (table) 
        {
            insert_table(db, table);
            count++;
        }
        archive = readdir(dir);
    }
    db->count = count;
    return db;
}

void save_table(const string path, Table* table)
{
    if (!table) return;
    FILE* file = fopen(path, "w");

    fprintf(file, "%s\n", table->name);
    for (size_t i = 0; i < table->collums_count - 1; i++)
    {
        fprintf(file, "%s|", table->collums[i]);
    }
    fprintf(file, "%s\n", table->collums[table->collums_count -1]);

    for (size_t j = 0; j < table->rows_count; j++)
    {
        for (size_t i = 0; i < table->collums_count; i++)
        {
            fprintf(file, "%s|", table->rows[j][i]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void save_db(DataBase* db)
{
    if (!db) return;
    MKDIR(".database_migue");

    for (size_t i = 0; i < db->count; i++)
    {
        char path[512];
        snprintf(path, sizeof(path), ".database_migue/%s", db->tables[i]->name);
        save_table(path, db->tables[i]);
    }
}

void free_table(Table* table)
{
    if (!table || !table->collums || !table->rows || !table->name) return;
    for (size_t i = 0; i < table->rows_count; i++)
    {
        for (size_t j = 0; j < table->collums_count; j++)
        {
            free(table->rows[i][j]);
        }
        free(table->rows[i]);
    }
    for (size_t i = 0; i < table->collums_count; i++)
    {
        free(table->collums[i]);
    }
    free(table->collums);
    free(table->name);
    free(table);
}

void free_db(DataBase* db)
{
    if (!db || !db->tables) return;
    for (size_t i = 0; i < db->count; i++) free_table(db->tables[i]);
    free(db->tables);
    free(db);
}


void debug_tokens(TokenList* list)
{
    if (!list) {
        printf("Cannot debug NULL pointer\n");
        return;
    }
    printf("\n=========================================\n");
    for (size_t i = 0; i < list->count; i++)
    {
        printf("Token %zu: %s\n", i, list->tokens[i]);
    }
    printf("=========================================\n");
}

bool expect(TokenList* list, size_t* current, const string expected)
{
    if (!list || !current || *current >= list->count || !expected) return false;
    if (strcmp(list->tokens[*current], expected) != 0) return false;
    
    (*current)++;
    return true;
}

string peek(TokenList* list, size_t current)
{
    if (!list || !list->tokens || current >= list->count) return NULL;

    return list->tokens[current];
}

string advance(TokenList* list, size_t* current)
{
    if (!list || !current || *current >= list->count) return NULL;

    return list->tokens[(*current)++];
}

SelectQuery* parse_select(string line)
{
    SelectQuery* query = malloc(sizeof(SelectQuery));
    TokenList* list = tokenize(line, " ");
    if (!query || !list) return NULL;

    size_t current = 0;
    if(!expect(list, &current, "SELECT"))
    {
        free_tokens(list);
        free(query);
        return NULL;
    }
    string token;
    do{
        token = advance(list, &current);
        if (!token)
        {
            printf("Syntax error: always use FROM with SELECT\n");
            free_tokens(list);
            free(query);
        }
    }while(strcmp(token, "FROM") != 0);

    char collums[256] = "";
    for (size_t i = 1; i < current - 1; i++)
    {
        strcat(collums, list->tokens[i]);
    }
    query->col_list = tokenize(collums, ", ");

    query->table_name = strdup(advance(list, &current));

    token = peek(list, current);
    if (!token || strcmp(token, "WHERE") != 0)
    {
        query->condition_collum = NULL;
        query->condition_value = NULL;
        query->has_condition = false;
        return query;
    }
    
    advance(list, &current);
    
    query->condition_collum = strdup(advance(list, &current));
    
    advance(list, &current);

    query->condition_value = strdup(advance(list, &current));
    free_tokens(list);

    return query;
}

int main(void) {
    DataBase* db = load_db();

    char s[10][1000] = {"SELECT name, id FROM people", 
                "SELECT name,id FROM people", 
                "SELECT name,id FROM people WHERE this = that"};

    SelectQuery* query[3] = {parse_select(s[0]), parse_select(s[1]), parse_select(s[2])};

    for (size_t i = 0; i < 3; i++)
    {
        debug_tokens(query[i]->col_list);
    }

    return 0;
}
