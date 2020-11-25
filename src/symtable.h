/*File name: symtable.h ---------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Viktor Rucký (xrucky01)                                |
 |            Vojtěch Vlach (xvlach22)                               |
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

#ifndef SYMTABLE_H
#define SYMTABLE_H
#include "symbol.h"
#include "error.h"
#include "data_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABSIZE 19
#define DEFAULTANSWER 42

typedef struct FuncTabEl{
	struct FuncTabEl *ptrNext;
	SymbolFunction FuncData;
} FuncTabEl;

typedef struct VarTabEl{
	struct VarTabEl *ptrNext;
	SymbolVariable VarData;
} VarTabEl;

FuncTabEl *FuncTab[TABSIZE];

typedef VarTabEl *VariableTable[TABSIZE];

int hashCode (const char *string_key);

/*----------------------------------------- Function Table ------------------------------------------*/

int initFunctionTable();

int addFunction(SymbolFunction* function);

int equalFunctions(const SymbolFunction *f1, const SymbolFunction *f2);

int deepCopyFunction(SymbolFunction* function, int hash);

const SymbolFunction* getFunction(const char* id);

void freeFunctionTable();

void freeFunction(SymbolFunction *func);

/*------------------------------------------ Variable Table -----------------------------------------*/

int initVariableTable(VariableTable* table);

int addVariableToTable(SymbolVariable* variable, VariableTable* table);

SymbolVariable* getVariableFromTable(const char * id, VariableTable* table);

void freeVariableTable(VariableTable* table);

/*--------------------------------------- Variable Table Stack --------------------------------------*/

int initVariableTableStack();

int enterNewStackFrame();

void leaveStackFrame();

int addVariable(SymbolVariable* variable);

SymbolVariable* getVariable(const char* id);

void freeVariableTableStack();

#endif
