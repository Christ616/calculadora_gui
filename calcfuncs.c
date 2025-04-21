#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "calc.h"


// Declaramos el buffer del lexer
char result_buffer[100] = "Resultado: ";
extern FILE *yyin;
extern char *yytext;
extern int yyparse();
extern struct ast *yylval;

// Parseo de expresiones
struct ast *parse_expression(const char *input) {
    FILE *tmp = fopen("temp_input.txt", "w");
    if (!tmp) {
        yyerror("No se pudo crear el archivo temporal.");
        return NULL;
    }

    // Escribimos la expresión en el archivo temporal
    fprintf(tmp, "%s\n", input);
    fclose(tmp);

    // Reabrimos el archivo temporal para que Flex lo lea
    FILE *old_yyin = yyin;
    yyin = fopen("temp_input.txt", "r");
    if (!yyin) {
        yyerror("No se pudo abrir el archivo temporal para lectura.");
        return NULL;
    }

    // Llamamos al parser
    int parse_result = yyparse();
    fclose(yyin);
    yyin = old_yyin;

    // Devolvemos el resultado si el parseo fue exitoso
    if (parse_result == 0) {
        return yylval;
    } else {
        return NULL;
    }
}

/* Crea un nodo en el AST para operaciones binarias */
struct ast *newast(int nodetype, struct ast *l, struct ast *r) {
    struct ast *a = malloc(sizeof(struct ast));
    if (!a) {
        yyerror("Sin espacio en memoria");
        exit(1);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

/* Crea un nodo para números */
struct ast *newnum(double d) {
    struct numval *a = malloc(sizeof(struct numval));
    if (!a) {
        yyerror("Sin espacio en memoria");
        exit(1);
    }
    a->nodetype = 'K';
    a->number = d;
    return (struct ast *)a;
}

/* Crea un nodo para funciones matemáticas */
struct ast *newfunc(int nodetype, struct ast *l) {
    struct fncall *a = malloc(sizeof(struct fncall));
    if (!a) {
        yyerror("Sin espacio en memoria");
        exit(1);
    }
    a->nodetype = nodetype;
    a->l = l;
    return (struct ast *)a;
}

/* Crea un nodo para operaciones de comparación */
struct ast *newcmp(int cmp_type, struct ast *l, struct ast *r) {
    struct cmp *a = malloc(sizeof(struct cmp));
    if (!a) {
        yyerror("Sin espacio en memoria");
        exit(1);
    }
    a->nodetype = 'X';
    a->cmp_type = cmp_type;
    a->l = l;
    a->r = r;
    return (struct ast *)a;
}

/* Evalúa el AST */
double eval(struct ast *a) {
    double v;

    if (!a) {
        yyerror("Intento de evaluar un nodo nulo");
        return 0;
    }

    switch (a->nodetype) {
        case 'S': v = sin(eval(a->l)); break;
        case 'C': v = cos(eval(a->l)); break;
        case 'T': v = tan(eval(a->l)); break;
        case 'L': v = log(eval(a->l)); break;
        case 'E': v = exp(eval(a->l)); break;
        case 'Q': v = sqrt(eval(a->l)); break;

        case '+': v = eval(a->l) + eval(a->r); break;
        case '-': v = eval(a->l) - eval(a->r); break;
        case '*': v = eval(a->l) * eval(a->r); break;
        case '/': {
            double den = eval(a->r);
            if (den == 0) {
                yyerror("Error: división por cero");
                return 0;
            }
            v = eval(a->l) / den;
            break;
        }
        case '|': v = fabs(eval(a->l)); break;
        case 'M': v = -eval(a->l); break;
        case 'K': v = ((struct numval *)a)->number; break;

        /* Comparaciones */
        case 'X': {
            struct cmp *c = (struct cmp *)a;
            double left = eval(c->l);
            double right = eval(c->r);
            switch (c->cmp_type) {
                case 1: v = left > right; break;
                case 2: v = left < right; break;
                case 3: v = left != right; break;
                case 4: v = left == right; break;
                case 5: v = left >= right; break;
                case 6: v = left <= right; break;
                default:
                    yyerror("Error: comparación desconocida '%d'", c->cmp_type);
                    v = 0;
            }
            break;
        }

        default:
            yyerror("Error: Nodo desconocido '%c'", a->nodetype);
            return 0;
    }

    return v;
}

/* Muestra la ayuda para el usuario */
void print_help(void) {
    printf("\n=== Ayuda de la Calculadora ===\n");
    printf("Operaciones básicas:\n");
    printf("  - Suma: 2 + 3\n");
    printf("  - Resta: 5 - 2\n");
    printf("  - Multiplicación: 4 * 3\n");
    printf("  - División: 10 / 2\n");
    printf("\nFunciones matemáticas:\n");
    printf("  - Seno: sin(PI/2)\n");
    printf("  - Coseno: cos(0)\n");
    printf("  - Tangente: tan(PI/4)\n");
    printf("  - Logaritmo natural: log(10)\n");
    printf("  - Exponencial: exp(1)\n");
    printf("  - Raíz cuadrada: sqrt(16)\n");
    printf("\nOperadores especiales:\n");
    printf("  - Valor absoluto: |5|\n");
    printf("  - Negativo: -5\n");
    printf("\nComparaciones:\n");
    printf("  - Mayor que: 5 > 3\n");
    printf("  - Menor que: 2 < 4\n");
    printf("  - Igualdad: 3 == 3\n");
    printf("  - Diferente: 3 <> 2\n");
    printf("  - Mayor o igual: 5 >= 5\n");
    printf("  - Menor o igual: 2 <= 3\n");
    printf("\nComandos especiales:\n");
    printf("  - help : Muestra esta ayuda.\n");
    printf("  - Ctrl+D: Finalizar el programa.\n");
    printf("===============================\n\n");
}

/* Libera la memoria del AST */
void treefree(struct ast *a) {
    if (!a) return;

    switch (a->nodetype) {
        case '+': case '-': case '*': case '/':
            treefree(a->r);
        case '|': case 'M':
            treefree(a->l);
            break;
        case 'S': case 'C': case 'T': case 'L': case 'E': case 'Q':
            treefree(((struct fncall *)a)->l);
            break;
        case 'K': break;
        case 'X':
            treefree(((struct cmp *)a)->l);
            treefree(((struct cmp *)a)->r);
            break;
        default:
            yyerror("Error interno: nodo inválido '%c'", a->nodetype);
    }

    free(a);
}