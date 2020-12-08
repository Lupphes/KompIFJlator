/*File name: symbol.h -----------------------------------------------*
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

#ifndef SYMBOL_H
#define SYMBOL_H
#include "str.h"
#include "data_type.h"
#include <stdbool.h>

typedef struct {
    string id;
    DataType type; 
} SymbolFunctionParameter;

typedef struct _SymbolFunction{
    string id;
    struct _parameters {
        SymbolFunctionParameter* params;
        int count;
        bool variadic;
    } parameters;
    struct _returnTypes{
        DataType* types;
        int count;
    } returnTypes;
    bool builtIn;
} SymbolFunction;

typedef struct _SymbolVariable {
    string id; //The name of the variable, as it shown in the source code, e.g. "foo" for when "foo := 42"
    int uid; //The unique id of the variable, which no other variable in the whole source code shares.
    DataType type;
} SymbolVariable;

#endif
