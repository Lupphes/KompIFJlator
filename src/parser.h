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

int accept(_TokenType type);
bool peek(_TokenType type);
int nextToken();
int parseExpression_Dummy();

int beginParsing();
int Start();
int Prolog();
int Chief();
int FunctionDefinition();
int FunctionDefinitionParameters_Start(SymbolFunction* function);
int FunctionDefinitionParameters_Next(SymbolFunction* function);
int FunctionReturnValues(SymbolFunction* function);
int FunctionReturnValues_First(SymbolFunction* function);
int FunctionReturnValues_Next(SymbolFunction* function);
int Block();
int Statement();
int StatementStartingWithIdentifier();
int Assignment();
int ExpressionList_Start();
int ExpressionList_Next();
int IDList_Next();
int VariableDefinition(string* id_name);
int FunctionCall();
int TermList();
int TermListNext();
int Term();
int If();
int Return();
int For();
int For_Definition();
int For_Assignment();

#endif