#ifndef CALC_H
#define CALC_H

#include <stdarg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Interfaz con el lexer */
extern int yylineno;
extern char result_buffer[100];
void yyerror(char *s, ...);

/* Definición de nodos en el árbol de sintaxis abstracta (AST) */
struct ast {
    int nodetype;  
    struct ast *l; 
    struct ast *r; 
};

/* Nodo de valor numérico */
struct numval {
    int nodetype;   
    double number;
};

/* Nodo de función matemática */
struct fncall {
    int nodetype;   
    struct ast *l;  
};

/* Nodo para comparación */
struct cmp {
    int nodetype; 
    int cmp_type; 
    struct ast *l;
    struct ast *r;
};

/* Construcción del AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(double d);
struct ast *newfunc(int nodetype, struct ast *l);
struct ast *newcmp(int cmp_type, struct ast *l, struct ast *r);
struct ast *parse_expression(const char *input);

/* Evaluación del AST */
double eval(struct ast *);

/* Eliminación del AST */
void treefree(struct ast *);

/* Prototipo para la función de ayuda */
void print_help(void);

#endif