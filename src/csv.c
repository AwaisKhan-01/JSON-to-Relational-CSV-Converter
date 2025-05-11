#include "csv.h"
#include <stdio.h>
#include <string.h>

void write_csv(Table *table, char *out_dir) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s.csv", out_dir, table->name);
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        perror("fopen");
        return;
    }

    // Write header
    for (int i = 0; i < table->num_columns; i++) {
        fprintf(fp, "%s", table->columns[i]);
        if (i < table->num_columns - 1) fprintf(fp, ",");
    }
    fprintf(fp, "\n");

    // Write rows
    for (Row *r = table->rows; r != NULL; r = r->next) {
        for (int i = 0; i < table->num_columns; i++) {
            if (r->values[i]) {
                if (table->column_types[i] == CT_STRING && strcmp(table->columns[i], "id") != 0 &&
                    strstr(table->columns[i], "_id") == NULL) {
                    fprintf(fp, "\"%s\"", r->values[i]);
                } else {
                    fprintf(fp, "%s", r->values[i]);
                }
            }
            if (i < table->num_columns - 1) fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}