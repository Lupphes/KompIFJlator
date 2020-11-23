/*File name: helper.h -----------------------------------------------*
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

#ifndef HELPER_H
#define HELPER_H
#include "str.h"
#include "symbol.h"

//Short hand. Process the "call" (usually a function call, but can technically be any expression in C) and returns its return code if is non-zero (i.e. the function returned unsuccesfully-)
#define callAndHandleException(call) if ((returnCode = (call))) return returnCode;
#define callAndHandleException_clean(call) if ((returnCode = (call))) goto CLEAN_UP;
#define returnAndClean(code) returnCode = code; goto CLEAN_UP;

typedef struct {
    string* arr;
    int count;
} StringArray;

typedef struct {
    SymbolVariable** arr;
    int count;
} SymbolVariableArray;

void initSymbolVariableArray(SymbolVariableArray* arr);
int addSymbolVariableToSymbolVariableArray(SymbolVariableArray* arr, SymbolVariable* var);
int countInSymbolVariableArray(SymbolVariableArray* arr);
void freeSymbolVariableArray(SymbolVariableArray* arr);

void initStringArray(StringArray* arr);
int addStringToStringArray(StringArray* arr, string* str);
int countInStringArray(StringArray* arr);
void freeStringArray(StringArray* arr);

#endif