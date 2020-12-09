/*File name: error.h ------------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Viktor Rucký (xrucky01)                                |
 |            Ondřej Sloup (xsloup02)                                |
 |                                                                   |
 |  _      _     _   __                   __  _        _             |
 | | |    (_)   | | /_/                  /_/ | |      | |            |
 | | |     _  __| | ___   _   _  __   ___   _| |_ __ _| |__  _   _   |
 | | |    | |/ _` |/ _ \ | | | | \ \ / / | | | __/ _` | '_ \| | | |  |
 | | |____| | (_| |  __/ | |_| |  \ V /| |_| | || (_| | | | | |_| |  |
 | |______|_|\__,_|\___|  \__,_|   \_/  \__, |\__\__,_|_| |_|\__,_|  |
 |                                       __/ |                       |
 |                                      |___/                        |
 *-------------------------------------------------------------------*/

#ifndef ERROR_H
#define ERROR_H

#define SUCCESS 0
#define LEXICAL_ERROR 1
#define SYNTAX_ERROR 2
#define SEMANTIC_ERROR_DEFINITION 3
#define SEMANTIC_ERROR_TYPE_INFERENCE 4
#define SEMANTIC_ERROR_TYPE_EXPRESSION 5
#define SEMANTIC_ERROR_TYPE_FUNCTION 6
#define SEMANTIC_ERROR_OTHER 7
#define SEMANTIC_ERROR_DIV_ZERO 9
#define INTERNAL_ERROR 99

#define NO_EXPRESSION 42
#define MORE_RVALUES_THAN_LVALUES 43

typedef int make_iso_compilers_happy;

#endif