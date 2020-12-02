/*File name: term.h -------------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Viktor Rucký (xrucky01)                                |
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

#ifndef TERM_H
#define TERM_H
#include <stdint.h>
#include "data_type.h"
#include "str.h"
#include "symbol.h"

typedef enum {
    TermIntegerLiteral = TypeInt,
    TermFloatLiteral = TypeFloat64,
    TermStringLiteral = TypeString,
    TermTypeBool,
    TermVariable
} TermType;

typedef struct {
    TermType type;
    union {
        int64_t i;
        double d;
        string s;
        const SymbolVariable* v;
    } value;
} Term;

#endif