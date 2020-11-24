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
//#define SUCCESS 0
//#define INTERNAL_ERROR 1
#define DEFAULTANSWER 42

typedef struct FuncTabEl{
	struct FuncTabEl *ptrNext;
	SymbolFunction FuncData;
} FuncTabEl;

typedef struct VarTabEl{
	struct VarTabEl *ptrNext;
	SymbolVariable *VarData;
} VarTabEl;

//VarTabEl *VarTab[TABSIZE];

typedef struct{
    int size;
    FuncTabEl *data[TABSIZE];
} VariableTable;

FuncTabEl *FuncTab[TABSIZE];

int hashCode (string *string_key);

/*----------------------------------------- Function Table ------------------------------------------*/

int initFunctionTable();

int addFunction(SymbolFunction* function);

int deepCopyFunction(SymbolFunction* function, int hash);

const SymbolFunction* getFunction(const char* id);

void freeFunctionTable();

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
