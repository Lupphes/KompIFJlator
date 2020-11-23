/*File name: symtable.c ---------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Vojtěch Vlach (xvlach22) 	                             |
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

#ifndef SYMTABLE_C
#define SYMTABLE_C
#include "symtable.h"


/*
 * @brief counts hash from given string
 *
 * @TODO Copied from IAL Homework 2020 c016.c
 *		Copyright belongs to Petr Přikryl (prosinec 1994), Vaclav Topinka (2005), 
 *		Karel Masařík (říjen 2014) and Radek Hranický (2014-2018)
 */
int hashCode (string *string_key){
	int retval = 0;
	int keylen = strGetLength(string_key);
	char* key = strGetStr(string_key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % TABSIZE );
}

/*----------------------------------------- Function Table ------------------------------------------*/

/**
 * @brief Initialises the table of function for operation. It is guaranteed that this 
 *		function is called before any other function dealing with the table of functions and is called 
 *		only once. This function does NOT populate the table of functions with built-in functions.
 * 
 * @return SUCCESS If the initialisation was successful
 * @return INTERNAL_ERROR If there was an error with the function initialisation.
 */
int initFunctionTable(){
	return DEFAULTANSWER;

}

/**
 * @brief Adds a function to the table of functions. Performs a deep copy of the given function.
 * 
 * @param function The function to add.
 * 
 * @return SUCCESS If the function was added succesfully.
 * @return SEMANTIC_ERROR_DEFINITION If the function already exists in the symtable.
 * @return INTENRAL_ERROR If there was a problem with adding the function (like malloc).
 */
int addFunction(SymbolFunction* function){
	return DEFAULTANSWER;
}

/**
 * @brief Gets a reference to a function in the table of functions.
 * 
 * @param id The name of the function to find.
 * 
 * @return const SymbolFunction* A pointer to the found function symbol or NULL if it wasn't found.
 */
const SymbolFunction* getFunction(const char* id){
	return NULL;
}

/**
 * @brief Cleans up all memory used by the function table. It is guaranteed that this function 
 * 		is called only once and that, after it's called, no additional functions dealing 
 *		with the table of functions are called.
 * 
 */
void freeFunctionTable(){

}

/*------------------------------------------ Variable Table -----------------------------------------*/

/**
 * @brief Initialises the table of variables pointed by table. It is guaranteed that table points to a 
 *		valid block of memory of size sizeof(VariableTable)
 * 
 * @param table The table to be initialised.
 * 
 * @return SUCCESS If the function initialised the variable table succesfully.
 * @return INTERNAL_ERROR If there was an error in initialising the table.
 */
int initVariableTable(VariableTable* table){
	return DEFAULTANSWER;
}


/**
 * @brief Adds a variable to the specified table of variables.
 * 
 * @param variable A pointer to a valid SymbolVariable to be added.
 * @param table A pointer to a valid VariableTable into which to add the new variable entry.
 * @return SUCCESS If the variable was successfully added to the table of variables.
 * @return SEMANTIC_ERROR_DEFINITION If the variable with the specified name already exists.
 * @return INTERNAL_ERROR f there was a problem with adding the variable (like malloc).
 */
int addVariableToTable(SymbolVariable* variable, VariableTable* table){
	return DEFAULTANSWER;
}

/**
 * @brief Gets a reference to a variable in the specified table of variables.
 * 
 * @param id The name of the variable to find.
 * @param table A pointer to a VariableTable to look in.
 * @return SymbolVariable* A pointer to the found variable symbol or NULL if it wasn't found.
 */
SymbolVariable* getVariableFromTable(const char * id, VariableTable* table){
	return NULL;
}

/**
 * @brief Cleans up all memory used by the variable table. It is guaranteed that this function 
 *		is called only once and that, after it's called, no additional functions dealing 
 *		with the table of variables are called.
 */
void freeVariableTable(VariableTable* table){

}

/*--------------------------------------- Variable Table Stack --------------------------------------*/

/**
 * @brief Initialisases the stack of variable tables used by the parser.
 * 
 * @return SUCCESS Initialisation was successful.
 * @return INTERNAL_ERROR Initialisation wasn't successfuly (malloc or similar).
 */
int initVariableTableStack(){
	return DEFAULTANSWER;
}

/**
 * @brief Enters a new "frame" for defining variables; this becomes the "active" frame (like a stack push). 
 *		See other functions for details.
 * 
 * @return SUCCESS Creation of new stack frame was successful.
 * @return INTERNAL_ERROR Creation of new stack frame wasn't successful (malloc or similar).
 */
int enterNewStackFrame(){
	return DEFAULTANSWER;
}

/**
 * @brief Deletes and cleans up the memory used by the active variable stack frame and moves back 
 *		to the previous one (like a stack pop). Using this function when the stack is empty 
 *		is guaranteed not to happen.
 *		zmena: neuvolnuje pamet, ale ulozi si referenci na danou tabulku do "allTheTables" nebo tak nejak
 */
void leaveStackFrame(){

}

/**
 * @brief Adds the specified variable to the active variable stack frame.
 * 
 * @param variable The variable to add.
 * @return SUCCESS uhh... you know what.
 * @return SEMANTIC_ERROR_DEFINITION The variable already exists in the active variable stack frame. 
 *		(The varialbe may or may not exist in the previous stack frames, but this is does not concern 
 *		the output of this function.)
 *
 * @return INTERNAL_ERROR uhh... you know what
 */
int addVariable(SymbolVariable* variable){
	return DEFAULTANSWER;
}

/**
 * @brief Gets a reference to the FIRST occurence of the variable in the stack of variable tables. 
 *		I.e. this function first looks in the current variable stack frame and if it doesn't find 
 *		the variable there, it goes to the previous and so on until it hits the top of the stack.
 * 
 * @param id The name of the variable to find.
 * @return SymbolVariable* The FIRST occurence of the variable in the stack of variable tables 
 *		or NULL if such a variable doesn't exist ANYWHERE.
 */
SymbolVariable* getVariable(const char* id){
	return NULL;
}

/**
 * @brief Cleans up the entire stacks of variable tables.
 *		zmena: smaz a uvolni pamet celeho pole "allTheTables"
 * 
 */
void freeVariableTableStack(){

}

#endif
