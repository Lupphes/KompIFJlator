/*File name: generator.h---------------------------------------------*
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

#ifndef GENERATOR_H
#define GENERATOR_H
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "error.h"

int generateTree(ASTRoot *root);
int generateMain(ASTNodeFunction *function);
void generateUserFunctions(ASTNodeFunction *function);
void generateFunction(ASTNodeFunction *function);
void generateFunctionBody(ASTNodeFunction *function);
int generateDefinitionsOfVariables(SymbolVariableArray *variables);
void generateFunctionCall(ASTNodeStatement *functionCall);
void generateFunctionCodeBlock(ASTNodeBlock* code);

void generateDebug();

#endif