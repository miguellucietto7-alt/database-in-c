#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "query.h"
#include "table.h"
#include "tokens.h"

void debug_tokens(TokenList*);
void debug_db(DataBase*);
void debug_table(Table*);
void debug_select_query(SelectQuery*);
void debug_insert_query(InsertQuery*);
void debug_delete_query(DeleteQuery*);

#endif