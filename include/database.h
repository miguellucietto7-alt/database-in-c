#ifndef DATABASE_H
#define DATABASE_H


typedef struct Table Table;
typedef struct DataBase DataBase;

typedef struct DataBase {
    Table** buckets;
    size_t count;
    size_t cap;
}DataBase;

DataBase* new_db();

#endif