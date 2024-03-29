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
#include "term.h"
#include "expression_analysis.h"

//Short hand. Process the "call" (usually a function call, but can technically be any expression in C) and returns its return code if is non-zero (i.e. the function returned unsuccesfully.)
#define callAndHandleException(call) {if ((returnCode = (call))) return returnCode;}

//Short hand. Process the "call" (usually a function call, but can technically be any expression in C). If the call resulted in a failure, sets the returnCode variable to whatever the call returned and goes to the CLEAN_UP label.
#define callAndHandleException_clean(call) {if ((returnCode = (call))) goto CLEAN_UP;}

//Short hand. Sets the returnCode variable to the provided code and goes to the CLEAN_UP label. 
#define returnAndClean(code) {returnCode = code; goto CLEAN_UP;}

typedef struct {
    string* arr;
    int count;
} StringArray;

typedef struct {
    const SymbolVariable** arr;
    int count;
} SymbolVariableArray;

typedef struct {
    Term** arr;
    int count;
} TermArray;

typedef struct {
    ExpExp** arr;
    int count;
} ExpressionArray;

#include "ast.h"

typedef struct{ //There is some circular dependency magic going on; couldn't fix it in time.
    string functionName;
    SymbolVariableArray* lValues;
    TermArray* functionParameters;
    struct _ASTNodeFunctionCall* astRepresentation;
} DubiousFunctionCall;

typedef struct {
    DubiousFunctionCall* arr;
    int count;
} DubiousFunctionCallArray;

void initSymbolVariableArray(SymbolVariableArray* arr);
int addToSymbolVariableArray(SymbolVariableArray* arr, const SymbolVariable* var);
int countInSymbolVariableArray(const SymbolVariableArray* arr);
void freeSymbolVariableArray(SymbolVariableArray* arr);

void initTermArray(TermArray* arr);
int addToTermArray(TermArray* arr, Term* var);
int countInTermArray(const TermArray* arr);
void freeTermArray(TermArray* arr);

void initStringArray(StringArray* arr);
int addToStringArray(StringArray* arr, string* str);
int countInStringArray(const StringArray* arr);
void freeStringArray(StringArray* arr);

void initDubiousFunctionCallArray(DubiousFunctionCallArray* arr);
int addToDubiousFunctionCallArray(DubiousFunctionCallArray* arr, DubiousFunctionCall* functionCall);
int countInDubiousFunctionCallArray(const DubiousFunctionCallArray* arr);
void freeDubiousFunctionCallArray(DubiousFunctionCallArray* arr);

void initExpressionArray(ExpressionArray* arr);
int addToExpressionArray(ExpressionArray* arr, ExpExp* expression);
int countInExpressionArray(const ExpressionArray* arr);
void freeExpressionArray(ExpressionArray* arr);

#endif