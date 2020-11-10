#ifndef ERROR_H
#define ERROR_H

#define LEXICAL_ERROR 1
#define SYNTAX_ERROR 2
#define SEMANTIC_ERROR_DEFINITION 3
#define SEMANTIC_ERROR_TYPE_INFERENCE 4
#define SEMANTIC_ERROR_TYPE_EXPRESION 5
#define SEMANTIC_ERROR_TYPE_FUNCTION 6
#define SEMANTIC_ERROR_OTHER 7
#define SEMANTIC_ERROR_DIV_ZERO 9
#define INTERNAL_ERROR 99

typedef int make_iso_compilers_happy;

#endif