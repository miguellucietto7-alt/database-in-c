#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <sys/stat.h>
#include "table.h"

#define MKDIR(dir) mkdir(dir, 0777)

typedef struct DataBase DataBase;

typedef struct DataBase {
    Table** tables;
    size_t count;
    size_t cap;
}DataBase;

DataBase* load_db();
void save_db(DataBase*);

#endif