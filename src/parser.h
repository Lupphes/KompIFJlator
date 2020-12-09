/*File name: parser.h -----------------------------------------------*
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
#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#include "token.h"
#include "symbol.h"
#include "term.h"
#include "str.h"
#include "helper.h"
#include "ast.h"

int Start(ASTRoot* astRoot);
int Prolog();
int Chief(ASTRoot* astRoot);
int FunctionDefinition(ASTRoot* astRoot);
int FunctionDefinitionParameters_Start(SymbolFunction* function);
int FunctionDefinitionParameters_Next(SymbolFunction* function);
int FunctionReturnValues(SymbolFunction* function);
int FunctionReturnValues_First(SymbolFunction* function);
int FunctionReturnValues_Next(SymbolFunction* function);
int Block(ASTNodeBlock* astBlock,bool generateNewStackFrame);
int Statement(ASTNodeStatement** astStatement);
int StatementStartingWithIdentifier(ASTNodeStatement* astStatement);
int Assignment(ASTNodeStatement* astStatement,SymbolVariableArray* lValues);
int AssignmentOfExpressions(ASTNodeAssignment* astAsignment, const SymbolVariableArray* lValues);
int ExpressionList_Start(ExpressionArray* expressionList);
int ExpressionList_Next(ExpressionArray* expressionList);
int VariableList_Next(SymbolVariableArray* lValues);
int VariableDefinition(ASTNodeAssignment* astAssignmentStatement, string* id_name);
int FunctionCall_rule(ASTNodeStatement* astStatement,SymbolVariableArray* lValues, const SymbolFunction* function, const string* functionName);
int TermList(TermArray* functionParameters);
int TermListNext(TermArray* functionParameters);
int If(ASTNodeStatement* astStatement);
int Return(ASTNodeStatement* astStatement);
int For(ASTNodeStatement* astStatement);
int For_Definition(ASTNodeFor* astForStatement);
int For_Assignment(ASTNodeFor* astForStatement);

#endif