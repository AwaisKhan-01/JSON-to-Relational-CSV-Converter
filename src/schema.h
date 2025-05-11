#ifndef SCHEMA_H
#define SCHEMA_H

#include "ast.h"

typedef enum {
    CT_NUMBER,
    CT_STRING,
    CT_BOOLEAN
} ColumnType;

typedef struct Row {
    char **values;
    struct Row *next;
} Row;

typedef struct Table {
    char *name;
    char **columns;
    ColumnType *column_types;
    int num_columns;
    Row *rows;
    int next_id;
    struct Table *next;
} Table;

void process_ast(ASTNode *root, char *out_dir);
void free_tables(Table *tables);

#endif // SCHEMA_H