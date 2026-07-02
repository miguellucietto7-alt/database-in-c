#ifndef DATABASE_H
#define DATABASE_H

#include "table.h"

typedef struct DataBase DataBase;

typedef struct DataBase {
    Table** buckets;
    size_t count;
    size_t cap;
}DataBase;

#endif