#include  <stdlib.h>
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