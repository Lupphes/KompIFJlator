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

typedef struct FuncTabEl{
	struct FuncTabEl *ptrNext;
	SymbolFunction FuncData;
} FuncTabEl;

FuncTabEl *FuncTab[TABSIZE];

typedef struct VarTabEl{
	struct VarTabEl *ptrNext;
	SymbolVariable VarData;
} VarTabEl;

typedef VarTabEl *VariableTable[TABSIZE];

typedef struct StackEl{
	struct StackEl *ptrNext;
	VariableTable table;
} StackEl;

typedef StackEl *Stack;
Stack mainStack;
Stack binStack;

int hashCode (const char *string_key);

/*----------------------------------------- Function Table ------------------------------------------*/

int initFunctionTable();

int addFunction(SymbolFunction* function);

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

const SymbolVariable* getVariable(const char* id);

void freeVariableTableStack();

void freeStack(Stack *stack);

#endif
