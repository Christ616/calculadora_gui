%{
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include "calc.h"

int yylex(void);
void yyerror(char *s, ...);
%}

/* Definir el tipo de valor de `yylval` */
%union {
    struct ast *a;
    double d;
    int fn;
}

/* Declaración de tokens y tipos de datos */
%token <d> NUMBER
%token SIN COS TAN LOG EXP SQRT
%token IF THEN ELSE WHILE DO LET
%token <fn> CMP
%token HELP
%token EOL
%type <a> exp factor term

/* Definir precedencia de operadores */
%left '+' '-'
%left '*' '/'
%nonassoc CMP
%nonassoc IF THEN ELSE WHILE DO LET

%%

calclist:
    /* Nada */
    | calclist exp EOL {
            double result = eval($2);
            extern char result_buffer[100];
            snprintf(result_buffer, sizeof(result_buffer), "Resultado: %g", result);
            treefree($2);
    }
    | calclist HELP EOL {
            print_help();
            printf("> ");
    }
    | calclist EOL { printf("> "); }
    ;

exp: factor
    | exp '+' factor { $$ = newast('+', $1, $3); }
    | exp '-' factor { $$ = newast('-', $1, $3); }
    | exp CMP factor { $$ = newcmp($2, $1, $3); }
    ;

factor: term
    | factor '*' term { $$ = newast('*', $1, $3); }
    | factor '/' term { $$ = newast('/', $1, $3); }
    ;

term: NUMBER { $$ = newnum($1); }
    | '|' term { $$ = newast('|', $2, NULL); }
    | '(' exp ')' { $$ = $2; }
    | '-' term { $$ = newast('M', $2, NULL); }
    | SIN '(' exp ')' { $$ = newast('S', $3, NULL); }
    | COS '(' exp ')' { $$ = newast('C', $3, NULL); }
    | TAN '(' exp ')' { $$ = newast('T', $3, NULL); }
    | LOG '(' exp ')' { $$ = newast('L', $3, NULL); }
    | EXP '(' exp ')' { $$ = newast('E', $3, NULL); }
    | SQRT '(' exp ')' { $$ = newast('Q', $3, NULL); }
    ;

%%

void yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}