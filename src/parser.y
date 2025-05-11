%{
#include "json2relcsv.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern void yyerror(const char *msg);
extern ASTNode *root;
%}

%locations
%define parse.error verbose

%union {
    char *strval;
    char *numval;
    bool boolval;
    ASTNode *node;
    KeyValuePair *pair;
    ASTNodeList *nodelist;
}

%token LBRACE RBRACE LBRACK RBRACK COLON COMMA
%token <boolval> TRUE FALSE
%token <strval> STRING NULLVAL
%token <numval> NUMBER

%type <node> value object array
%type <pair> pair pairs
%type <nodelist> values

%start json

%%

json: value { root = $1; }
    ;

value: object { $$ = $1; }
     | array { $$ = $1; }
     | STRING { $$ = create_string($1); }
     | NUMBER { $$ = create_number($1); }
     | TRUE { $$ = create_bool(true); }
     | FALSE { $$ = create_bool(false); }
     | NULLVAL { $$ = create_null(); }
     ;

object: LBRACE pairs RBRACE { $$ = create_object($2); }
      | LBRACE RBRACE { $$ = create_object(NULL); }
      ;

pairs: pair { $$ = $1; }
     | pairs COMMA pair { $$ = append_pair($1, $3); }
     ;

pair: STRING COLON value {
        $$ = create_pair($1, $3);
    }
    ;

array: LBRACK values RBRACK { $$ = create_array($2); }
     | LBRACK RBRACK { $$ = create_array(NULL); }
     ;

values: value { $$ = create_node_list($1); }
      | values COMMA value { $$ = append_node_list($1, $3); }
      ;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error: %s at line %d, column %d\n", msg, yylloc.first_line, yylloc.first_column);
    exit(1);
}