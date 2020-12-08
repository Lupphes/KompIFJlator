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
#include <stdlib.h>
#include <stdlib.h>

// free all the tree
void freeTree(ASTRoot *root){
	/* TODO */
	fprintf(stderr, "Freeing root\n");
}

void testProg1(ASTRoot *root){
	/*
	package main

	main(){
		a := 0
		a = 1
		a++;
	}
	*/ 

	root->userFunctions = NULL;
	root->mainFunction = (ASTNodeFunction *) malloc(sizeof(ASTNodeFunction));

	// TODO define main function as shown higher

	root->mainFunction->next = NULL;
}

int main() {
	ASTRoot root;

	testProg1(&root);
	generateTree(&root);
	free(&root);

	return 0;
}
