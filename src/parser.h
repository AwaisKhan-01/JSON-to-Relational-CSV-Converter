#ifndef YY_YY_BUILD_PARSER_H_INCLUDED
# define YY_YY_BUILD_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
    enum yytokentype
    {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    LBRACE = 258,                  /* LBRACE  */
    RBRACE = 259,                  /* RBRACE  */
    LBRACK = 260,                  /* LBRACK  */
    RBRACK = 261,                  /* RBRACK  */
    COLON = 262,                   /* COLON  */
    COMMA = 263,                   /* COMMA  */
    TRUE = 264,                    /* TRUE  */
    FALSE = 265,                   /* FALSE  */
    STRING = 266,                  /* STRING  */
    NULLVAL = 267,                 /* NULLVAL  */
    NUMBER = 268                   /* NUMBER  */
    };
    typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "src/parser.y"

    char *strval;
    char *numval;
    bool boolval;
    ASTNode *node;
    KeyValuePair *pair;
    //KeyValuePairList *pairlist;
    ASTNodeList *nodelist;

#line 91 "build/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);

#endif /* !YY_YY_BUILD_PARSER_H_INCLUDED  */