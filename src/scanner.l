%{
#include "json2relcsv.h"
#include "parser.h"  // Ensure YYLTYPE and yylloc are defined
#include <string.h>

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
                       yylloc.first_column = yycolumn; \
                       yylloc.last_column = yycolumn + yyleng - 1; \
                       yycolumn += yyleng;

static int yycolumn = 1;

%}

%option noyywrap
%option yylineno
%option noinput
%option nounput

DIGIT       [0-9]
DIGITS      {DIGIT}+
INT         -?{DIGITS}
FRAC        \.{DIGITS}
EXP         [eE][+-]?{DIGITS}
NUMBER      {INT}({FRAC})?({EXP})?
STRING      \"([^\"\\]|\\[\n\\\/bfnrt]|\\u[0-9a-fA-F]{4})*\"
WHITESPACE  [ \t\n\r]+

%%

"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACK; }
"]"         { return RBRACK; }
":"         { return COLON; }
","         { return COMMA; }
"true"      { yylval.boolval = true; return TRUE; }
"false"     { yylval.boolval = false; return FALSE; }
"null"      { yylval.strval = NULL; return NULLVAL; }
{NUMBER}    { yylval.numval = strdup(yytext); return NUMBER; }
{STRING}    {
                char *str = strdup(yytext + 1);
                str[strlen(str) - 1] = '\0';
                for (int i = 0; str[i]; i++) {
                    if (str[i] == '\\' && str[i+1] == 'u') {
                        str[i] = ' '; // Simple Unicode handling (placeholder)
                        i += 5;
                    }
                }
                yylval.strval = str;
                return STRING;
            }
{WHITESPACE} { /* ignore */ }
.           { yyerror("Invalid character"); }

%%

void reset_scanner() {
    yycolumn = 1;
    yylineno = 1;
}