#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum {
    AST_OBJECT,
    AST_ARRAY,
    AST_STRING,
    AST_NUMBER,
    AST_TRUE,
    AST_FALSE,
    AST_NULL
} ASTNodeType;

typedef struct KeyValuePair {
    char *key;
    struct ASTNode *value;
    struct KeyValuePair *next;
} KeyValuePair;

typedef struct ASTNodeList {
    struct ASTNode *node;
    struct ASTNodeList *next;
} ASTNodeList;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        KeyValuePair *pairs;    // For objects
        ASTNodeList *elements;  // For arrays
        char *string;           // For strings and numbers
    } data;
} ASTNode;

ASTNode *create_object(KeyValuePair *pairs);
ASTNode *create_array(ASTNodeList *elements);
ASTNode *create_string(const char *value);
ASTNode *create_number(const char *value);
ASTNode *create_bool(bool value);
ASTNode *create_null();
KeyValuePair *create_pair(const char *key, ASTNode *value);
KeyValuePair *append_pair(KeyValuePair *list, KeyValuePair *pair);
ASTNodeList *create_node_list(ASTNode *node);
ASTNodeList *append_node_list(ASTNodeList *list, ASTNode *node);
void print_ast(ASTNode *node, int indent);
void free_ast(ASTNode *node);

#endif // AST_H