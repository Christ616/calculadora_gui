%option noyywrap nodefault yylineno

%{
# include <stdio.h>
# include <stdlib.h>
# include "calc.h"
# include "calc.tab.h"
%}

%%

"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")"     { return yytext[0]; }

"sin"   { return SIN; }
"cos"   { return COS; }
"tan"   { return TAN; }
"log"   { return LOG; }
"exp"   { return EXP; }
"sqrt"  { return SQRT; }

">"     { yylval.fn = 1; return CMP; }
"<"     { yylval.fn = 2; return CMP; }
"<>"    { yylval.fn = 3; return CMP; }
"=="    { yylval.fn = 4; return CMP; }
">="    { yylval.fn = 5; return CMP; }
"<="    { yylval.fn = 6; return CMP; }

"if"    { return IF; }
"then"  { return THEN; }
"else"  { return ELSE; }
"while" { return WHILE; }
"do"    { return DO; }
"let"   { return LET;}

"help"  { return HELP; }

([0-9]*"."?[0-9]+|[0-9]+"."?)([Ee][-+]?[0-9]+)? {
    yylval.d = atof(yytext);
    return NUMBER;
}

\n      { return EOL; }
"//".*  { /* Ignorar comentarios */ }
[ \t]   { /* Ignorar espacios en blanco */ }

.       { yyerror("Caracter desconocido: %c", *yytext); }

%%
