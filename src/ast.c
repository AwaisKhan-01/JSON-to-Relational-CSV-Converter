#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ASTNode *create_object(KeyValuePair *pairs) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_OBJECT;
    node->data.pairs = pairs;
    return node;
}

ASTNode *create_array(ASTNodeList *elements) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY;
    node->data.elements = elements;
    return node;
}

ASTNode *create_string(const char *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_STRING;
    node->data.string = value ? strdup(value) : NULL;
    return node;
}

ASTNode *create_number(const char *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->data.string = value ? strdup(value) : NULL;
    return node;
}

ASTNode *create_bool(bool value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = value ? AST_TRUE : AST_FALSE;
    node->data.string = NULL;
    return node;
}

ASTNode *create_null() {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_NULL;
    node->data.string = NULL;
    return node;
}

KeyValuePair *create_pair(const char *key, ASTNode *value) {
    KeyValuePair *pair = malloc(sizeof(KeyValuePair));
    pair->key = key ? strdup(key) : NULL;
    pair->value = value;
    pair->next = NULL;
    return pair;
}

KeyValuePair *append_pair(KeyValuePair *list, KeyValuePair *pair) {
    if (!list) return pair;
    KeyValuePair *current = list;
    while (current->next) current = current->next;
    current->next = pair;
    return list;
}

ASTNodeList *create_node_list(ASTNode *node) {
    ASTNodeList *list = malloc(sizeof(ASTNodeList));
    list->node = node;
    list->next = NULL;
    return list;
}

ASTNodeList *append_node_list(ASTNodeList *list, ASTNode *node) {
    ASTNodeList *new_list = create_node_list(node);
    if (!list) return new_list;
    ASTNodeList *current = list;
    while (current->next) current = current->next;
    current->next = new_list;
    return list;
}

void print_ast(ASTNode *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->type) {
        case AST_OBJECT:
            printf("Object:\n");
            for (KeyValuePair *p = node->data.pairs; p != NULL; p = p->next) {
                for (int i = 0; i < indent + 1; i++) printf("  ");
                printf("%s: ", p->key);
                print_ast(p->value, indent + 2);
            }
            break;
        case AST_ARRAY:
            printf("Array:\n");
            for (ASTNodeList *e = node->data.elements; e != NULL; e = e->next) {
                print_ast(e->node, indent + 1);
            }
            break;
        case AST_STRING:
            printf("\"%s\"\n", node->data.string ? node->data.string : "");
            break;
        case AST_NUMBER:
            printf("%s\n", node->data.string ? node->data.string : "0");
            break;
        case AST_TRUE:
            printf("true\n");
            break;
        case AST_FALSE:
            printf("false\n");
            break;
        case AST_NULL:
            printf("null\n");
            break;
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;
    switch (node->type) {
        case AST_OBJECT: {
            KeyValuePair *p = node->data.pairs;
            while (p) {
                KeyValuePair *next = p->next;
                free(p->key);
                free_ast(p->value);
                free(p);
                p = next;
            }
            break;
        }
        case AST_ARRAY: {
            ASTNodeList *e = node->data.elements;
            while (e) {
                ASTNodeList *next = e->next;
                free_ast(e->node);
                free(e);
                e = next;
            }
            break;
        }
        case AST_STRING:
        case AST_NUMBER:
            if (node->data.string) free(node->data.string);
            break;
        default:
            break;
    }
    free(node);
}