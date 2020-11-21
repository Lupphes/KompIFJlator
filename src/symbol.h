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

typedef struct {
    string id;
    DataType type; 
} SymbolFunctionParameter;

typedef struct _SymbolFunction{
    string id;
    struct _parameters {
        SymbolFunctionParameter* params;
        int count;
    } parameters;
    struct _returnTypes{
        DataType* types;
        int count;
    } returnTypes;
} SymbolFunction;

typedef struct _SymbolVariable {
    string id;
    DataType type;
} SymbolVariable;

#endif