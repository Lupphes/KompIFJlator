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

bool accept(_TokenType type);
bool peek(_TokenType type);
int nextToken();

int beginParsing();
int Start();
int Prolog();
int Chief();
int FunctionDefinition();
int FunctionDefinitionParameters_Start();
int FunctionDefinitionParameters_Next();
int FunctionReturnValues();
int FunctionReturnValues_First();
int FunctionReturnValues_Next();
int Block();

#endif