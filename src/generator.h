/*File name: generator.h---------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Vojtěch Vlach (xvlach22)                               |
 |            Viktor Rucký (xrucky01)                                |
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

#ifndef GENERATOR_H
#define GENERATOR_H
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "error.h"

int generateTree(ASTRoot *root);
int generateMain(ASTNodeFunction *function);
void generateUserFunctions(ASTNodeFunction *function);
void generateFunctionBody(ASTNodeFunction *function);
void generateFunctionCodeBlock(ASTNodeStatement* codeStmnt);
char* generateVariableName(const SymbolVariable* var, char* out);
char* generateIntLiteral(int64_t lit, char* out);
char* generateFloatLiteral(double lit, char* out);
char* generateStringLiteral(const char* lit,char* out);
char* generateTermRepresentation(const Term* term,char* out);

// built-in functions generation
void generateBuiltInPrint(ASTNodeFunctionCall* printCall);
void generateBuiltInInt2Float(ASTNodeFunctionCall* int2floatCall);
void generateBuiltInFloat2Int(ASTNodeFunctionCall* float2intCall);
void generateBuiltInLen(ASTNodeFunctionCall* lenCall);
void generateBuiltInChr(ASTNodeFunctionCall* chrCall);
void generateBuiltInOrd(ASTNodeFunctionCall* ordCall);
void generateBuiltInSubstr(ASTNodeFunctionCall* substrCall);
void generateBuiltInInputs(ASTNodeFunctionCall* inputsStatment);
void generateBuiltInInputi(ASTNodeFunctionCall* inputiStatment);
void generateBuiltInInputf(ASTNodeFunctionCall* inputfStatment);
void generateExpresion(ExpExp* exp);
void generateOperation(OperationType opType, DataType dataType);

// code statement generation
void generateAssignment(ASTNodeAssignment *assignment);
void generateFunctionCall(ASTNodeFunctionCall* call);
void generateBuiltInFunctionCall(ASTNodeFunctionCall* call);
void generateUserFunctionCall(ASTNodeFunctionCall* call);
void generateIf(ASTNodeIf* ifStatement);
void generateFor(ASTNodeFor* forStatement);
void generateReturn(ASTNodeReturn* returnStatement);

void generateDebug();

int getUID();
void getUIDLabelName(char* out);

// dummy methods
void generateExpExp(ExpExp* condition);
#endif