#ifndef JSON2RELCSV_H
#define JSON2RELCSV_H

#include <stdio.h>  // Added to define FILE
#include <stdbool.h>
#include "ast.h"

extern FILE *yyin;
extern ASTNode *root;

void print_ast(ASTNode *node, int indent);
void process_ast(ASTNode *root, char *out_dir);

#endif // JSON2RELCSV_H