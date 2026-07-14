#include  <stdlib.h>
#include  <stdio.h>
#include  <string.h>
#include  <sys/stat.h>
#include <dirent.h>
#include "database.h"
#include "table.h"
#include "utils.h"


DataBase* new_db()
{
    DataBase* db = malloc(sizeof(DataBase));
    CHECK_ALLOC(db);

    db->cap = 10;
    db->count = 0;
    db->buckets = calloc(db->cap, sizeof(Table*));

    return db;
}

void save_db(DataBase* db)
{
    if (!db) return;

    MKDIR(DB_PATH);

    for (size_t i = 0; i < db->cap; i++)
    {
        for (Table* table = db->buckets[i]; table != NULL; table = table->next)
        {
            save_table(table, DB_PATH);
        }
    }
}

DataBase* load_db()
{
    DataBase* db = new_db();
    foreach_file(file, DB_PATH)
    {
        load_table(db, strcat(DB_PATH"/", file->d_name));
    }

}