#include "schema.h"
#include "csv.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Table *tables = NULL;
static int table_counter = 1;

static void process_array(ASTNode *node, Table *parent_table, int parent_id, const char *key);

// Utility to find a table by name
static Table *find_table(const char *name) {
    for (Table *t = tables; t != NULL; t = t->next) {
        if (strcmp(t->name, name) == 0) return t;
    }
    return NULL;
}

static Table *create_table(const char *name, char **columns, ColumnType *types, int num_columns) {
    Table *table = malloc(sizeof(Table));
    table->name = strdup(name);
    table->columns = malloc(num_columns * sizeof(char *));
    table->column_types = malloc(num_columns * sizeof(ColumnType));
    for (int i = 0; i < num_columns; i++) {
        table->columns[i] = strdup(columns[i]);
        table->column_types[i] = types[i];
    }
    table->num_columns = num_columns;
    table->rows = NULL;
    table->next_id = 1;
    table->next = tables;
    tables = table;
    return table;
}

static int find_or_create_row(Table *table, const char *key, const char *value) {
    int key_index = -1;
    for (int i = 0; i < table->num_columns; i++) {
        if (strcmp(table->columns[i], key) == 0) {
            key_index = i;
            break;
        }
    }
    if (key_index == -1) return -1;

    for (Row *r = table->rows; r != NULL; r = r->next) {
        if (r->values[key_index] && strcmp(r->values[key_index], value) == 0) {
            return atoi(r->values[0]);
        }
    }

    Row *row = malloc(sizeof(Row));
    row->values = calloc(table->num_columns, sizeof(char *));
    row->values[0] = malloc(16);
    snprintf(row->values[0], 16, "%d", table->next_id++);
    row->values[key_index] = strdup(value);
    row->next = table->rows;
    table->rows = row;
    return atoi(row->values[0]);
}

static int add_row(Table *table, char **values) {
    Row *row = malloc(sizeof(Row));
    row->values = malloc(table->num_columns * sizeof(char *));
    for (int i = 0; i < table->num_columns; i++) {
        row->values[i] = values[i] ? strdup(values[i]) : NULL;
    }
    row->next = table->rows;
    table->rows = row;
    int id = table->next_id++;
    row->values[0] = malloc(16);
    snprintf(row->values[0], 16, "%d", id);
    return id;
}

static void generate_table_name(char *buf, size_t size, const char *base, KeyValuePair *pairs) {
    snprintf(buf, size, "%s", base);
    for (KeyValuePair *p = pairs; p != NULL; p = p->next) {
        strncat(buf, "_", size - strlen(buf) - 1);
        strncat(buf, p->key, size - strlen(buf) - 1);
    }
}

static int process_object(ASTNode *node, Table *parent_table, int parent_id, const char *key) {
    KeyValuePair *pairs = node->data.pairs;
    if (!pairs) return -1;

    char table_name[128];
    generate_table_name(table_name, sizeof(table_name), key ? key : "obj", pairs);

    Table *table = find_table(table_name);
    if (!table) {
        // Analyze columns
        int count = 0;
        for (KeyValuePair *p = pairs; p; p = p->next) count++;
        char *columns[count + 1];
        ColumnType types[count + 1];
        columns[0] = "id";
        types[0] = CT_NUMBER;

        int i = 1;
        for (KeyValuePair *p = pairs; p; p = p->next) {
            columns[i] = p->key;
            switch (p->value->type) {
                case AST_NUMBER: types[i] = CT_NUMBER; break;
                case AST_TRUE:
                case AST_FALSE: types[i] = CT_BOOLEAN; break;
                case AST_NULL:
                case AST_STRING: default: types[i] = CT_STRING; break;
            }
            i++;
        }
        table = create_table(table_name, columns, types, count + 1);
    }

    char *values[table->num_columns];
    values[0] = NULL;

    for (int i = 1; i < table->num_columns; i++) {
        for (KeyValuePair *p = pairs; p != NULL; p = p->next) {
            if (strcmp(p->key, table->columns[i]) == 0) {
                switch (p->value->type) {
                    case AST_STRING:
                    case AST_NUMBER:
                        values[i] = p->value->data.string;
                        break;
                    case AST_TRUE:
                        values[i] = "true"; break;
                    case AST_FALSE:
                        values[i] = "false"; break;
                    case AST_NULL:
                        values[i] = NULL; break;
                    case AST_OBJECT:
                        values[i] = NULL;
                        process_object(p->value, table, parent_id, p->key);
                        break;
                    case AST_ARRAY:
                        process_array(p->value, table, parent_id, p->key);
                        values[i] = NULL;
                        break;
                }
            }
        }
    }

    return add_row(table, values);
}

static void process_array(ASTNode *node, Table *parent_table, int parent_id, const char *key) {
    ASTNodeList *list = node->data.elements;
    if (!list) return;

    if (list->node->type != AST_OBJECT) {
        // Scalars
        char table_name[64];
        snprintf(table_name, sizeof(table_name), "%s", key);
        Table *table = find_table(table_name);
        if (!table) {
            char *columns[] = {"parent_id", "index", "value"};
            ColumnType types[] = {CT_NUMBER, CT_NUMBER, CT_STRING};
            table = create_table(table_name, columns, types, 3);
        }

        int index = 0;
        for (ASTNodeList *e = list; e; e = e->next) {
            char id_str[16], idx_str[16];
            snprintf(id_str, sizeof(id_str), "%d", parent_id);
            snprintf(idx_str, sizeof(idx_str), "%d", index++);

            char *value = NULL;
            switch (e->node->type) {
                case AST_STRING:
                case AST_NUMBER:
                    value = e->node->data.string;
                    break;
                case AST_TRUE:
                    value = "true"; break;
                case AST_FALSE:
                    value = "false"; break;
                default:
                    value = NULL; break;
            }

            char *row[3] = {id_str, idx_str, value};
            add_row(table, row);
        }
    } else {
        // Objects
        for (ASTNodeList *e = list; e; e = e->next) {
            process_object(e->node, parent_table, parent_id, key);
        }
    }
}

void process_ast(ASTNode *root, char *out_dir) {
    if (!root) return;

    if (root->type == AST_OBJECT) {
        process_object(root, NULL, 0, "root");
    } else if (root->type == AST_ARRAY) {
        process_array(root, NULL, 0, "root");
    }

    for (Table *t = tables; t; t = t->next) {
        write_csv(t, out_dir);
    }

    free_tables(tables);
    tables = NULL;
    table_counter = 1;
}

void free_tables(Table *tables) {
    while (tables) {
        Table *next = tables->next;
        free(tables->name);
        for (int i = 0; i < tables->num_columns; i++) free(tables->columns[i]);
        free(tables->columns);
        free(tables->column_types);

        Row *row = tables->rows;
        while (row) {
            Row *next_row = row->next;
            for (int i = 0; i < tables->num_columns; i++) {
                if (row->values[i]) free(row->values[i]);
            }
            free(row->values);
            free(row);
            row = next_row;
        }
        free(tables);
        tables = next;
    }
}
