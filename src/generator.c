/*File name: generator.c---------------------------------------------*
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


#include "generator.h"

/**
 *	@brief	Generates the whole tree
 *
 *	@param	root	Root of the tree
 */
int generateTree(ASTRoot *root){
	if(root != NULL){
		printf(".IFJcode20\n\n");
		printf("WRITE string@Hello\\032generator\\032world!\\010\n");
		printf("JUMP main\n");

		generateUserFunctions(root->userFunctions);
		generateMain(root->mainFunction);
	}
	else {
		printf("ERROR ROOT = NULL\n");
	}

	return 0;
}

/**
 *	@brief	Generates main function of given program
 *
 * 	@param	function 	Pointer to the function
 *
 *	@return	SUCCESS			If generating was successful
 *	@return	INTERNAL_ERROR	If param function is NULL and main does not exist
 */
int generateMain(ASTNodeFunction *function){
	if(function == NULL){
		fprintf(stderr, "missing main\n");
		return INTERNAL_ERROR;
	}

	printf("#------------------- main --------------------#\n");
	printf("LABEL main\n");
	printf("WRITE string@main\\032over\\032here!!!\\010\n");
	return SUCCESS;
}

/**
 *	@brief	Generates all user finctions by walking through linked list of functions
 *
 * 	@param	function 	Pointer to the first function
 */
void generateUserFunctions(ASTNodeFunction *function){
	while(function != NULL){
		generateFunction(function);
		function = function->next;
	}

	printf("#----------- end of user funtions ------------#\n");
}

/**
 *	@brief	Generates given function
 *
 * 	@param	function 	Pointer to the function definition
 */
void generateFunction(ASTNodeFunction *function){
	printf("WRITE string@New\\032function\\010\n\n");
}

