/*File name: generator-test.c ----------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Vojtěch Vlach (xvlach22)                               |
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

#include "./../../src/generator.h"
#include "./../../src/ast.h"
#include "./../../src/str.h"
#include "./../../src/symbol.h"
#include "./../../src/data_type.h"
#include "./../../src/helper.h"
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>

// free all the tree
void freeTree(ASTRoot *root){
	if(root != NULL){
		if(root->mainFunction != NULL){
			/*if(root->mainFunction->function != NULL){
				SymbolFunction *oldFunction = root->mainFunction->function; 
				strFree(&oldFunction->id);

				free(oldFunction);
			}*/
			free(root->mainFunction);
			freeSymbolVariableArray(&root->mainFunction->variables);
		}
	}
}

void testProg1(ASTRoot *root){
	/*
	package main

	main(){
		a := 1
		a = a + 1;
	}
	*/

	root->userFunctions = NULL;
	root->mainFunction = (ASTNodeFunction *) malloc(sizeof(ASTNodeFunction));

	// Define main function as shown higher
	SymbolFunction *newFunction = (SymbolFunction *)malloc(sizeof(SymbolFunction));
	strInit(&newFunction->id);
	strCopyConstString(&newFunction->id, "main");
	newFunction->parameters.count = 0;
	newFunction->parameters.params = NULL;
	newFunction->returnTypes.count = 0;
	newFunction->returnTypes.types = NULL;
	newFunction->builtIn = false;
	root->mainFunction->function = newFunction;

	initSymbolVariableArray(&root->mainFunction->variables);
	SymbolVariable *newVar = (SymbolVariable *)malloc(sizeof(SymbolVariable));
	strInit(&newVar->id);
	strCopyConstString(&newVar->id, "a");
	newVar->uid = 42;
	newVar->type = TypeInt;
	addToSymbolVariableArray(&root->mainFunction->variables, newVar);

	root->mainFunction->code = (ASTNodeBlock *)malloc(sizeof(ASTNodeBlock));

	root->mainFunction->code->firstStatement = (ASTNodeStatement *)malloc(sizeof(ASTNodeStatement));
	root->mainFunction->code->firstStatement->type = StatementTypeAssignment;
	initSymbolVariableArray(&root->mainFunction->code->firstStatement->value.assignment.lValues);
	addToSymbolVariableArray(&root->mainFunction->code->firstStatement->value.assignment.lValues, newVar);

	initExpressionArray(&root->mainFunction->code->firstStatement->value.assignment.rValues);
	ExpExp *newEE = (ExpExp *)malloc(sizeof(ExpExp));
	newEE->dataType = TypeInt;
	newEE->type = ExpExpAtom;
	newEE->ExpProperties.atom.type = TypeInt;
	newEE->ExpProperties.atom.value.i = 1;
	addToExpressionArray(&root->mainFunction->code->firstStatement->value.assignment.rValues, newEE);

	root->mainFunction->code->firstStatement->next = (ASTNodeStatement *)malloc(sizeof(ASTNodeStatement));
	root->mainFunction->code->firstStatement->next->type = StatementTypeAssignment;
	initSymbolVariableArray(&root->mainFunction->code->firstStatement->next->value.assignment.lValues);
	addToSymbolVariableArray(&root->mainFunction->code->firstStatement->next->value.assignment.lValues, newVar);

	initExpressionArray(&root->mainFunction->code->firstStatement->next->value.assignment.rValues);
	ExpExp *PlusOpEE = (ExpExp *)malloc(sizeof(ExpExp));
	PlusOpEE->dataType = TypeInt;
	PlusOpEE->type = ExpExpOperation;
	PlusOpEE->ExpProperties.operation.type = OperationAdd;

	ExpExp *newA = (ExpExp *)malloc(sizeof(ExpExp));
	newA->dataType = TypeInt;
	newA->type = ExpExpAtom;
	newA->ExpProperties.atom.type = TypeInt;
	newA->ExpProperties.atom.value.v = newVar;
	PlusOpEE->ExpProperties.operation.value.binary.first = newA;

	PlusOpEE->ExpProperties.operation.value.binary.first = newEE;//neco////////////////
	addToExpressionArray(&root->mainFunction->code->firstStatement->next->value.assignment.rValues, PlusOpEE);

	root->mainFunction->next = NULL;
}

int main() {
	ASTRoot root;

	testProg1(&root);
	generateTree(&root);
	freeTree(&root);

	return 0;
}
