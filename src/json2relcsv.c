#include "json2relcsv.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define root as a global variable
ASTNode *root = NULL;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <json_file> [--out-dir <dir>]\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror("Failed to open input file");
        return 1;
    }

    yyin = input;
    yyparse();

    if (root) {
        char *out_dir = argc > 3 && strcmp(argv[2], "--out-dir") == 0 ? argv[3] : ".";
        process_ast(root, out_dir);
        print_ast(root, 0);
        free_ast(root);
    }

    fclose(input);
    return 0;
}
