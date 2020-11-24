/*File name: symbol.h -----------------------------------------------*
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

#ifndef SYMTABLE_H
#define SYMTABLE_H
#include "symbol.h"

typedef struct{
    int todo;
} FunctionTable;

typedef struct{
    int todo;
} VariableTable;

/**
 * @brief Initialises the table of function for operation. It is guaranteed that this function is called before any other function dealing with the table of functions and is called only once. This function does NOT populate the table of functions with built-in functions.
 * 
 * @return SUCCESS If the initialisation was successful
 * @return INTERNAL_ERROR If there was an error with the function initialisation.
 */
int initFunctionTable();

/**
 * @brief Adds a function to the table of functions. Performs a deep copy of the given function.
 * 
 * @param function The function to add.
 * 
 * @return SUCCESS If the function was added succesfully.
 * @return SEMANTIC_ERROR_DEFINITION If the function already exists in the symtable.
 * @return INTENRAL_ERROR If there was a problem with adding the function (like malloc).
 */
int addFunction(SymbolFunction* function);

/**
 * @brief Gets a reference to a function in the table of functions.
 * 
 * @param id The name of the function to find.
 * 
 * @return const SymbolFunction* A pointer to the found function symbol or NULL if it wasn't found.
 */
const SymbolFunction* getFunction(const char* id);

/**
 * @brief Cleans up all memory used by the function table. It is guaranteed that this function is called only once and that, after it's called, no additional functions dealing with the table of functions are called.
 * 
 */
void freeFunctionTable();

/**
 * @brief Initialises the table of variables pointed by table. It is guaranteed that table points to a valid block of memory of size sizeof(VariableTable)
 * 
 * @param table The table to be initialised.
 * 
 * @return SUCCESS If the function initialised the variable table succesfully.
 * @return INTERNAL_ERROR If there was an error in initialising the table.
 */
int initVariableTable(VariableTable* table);


/**
 * @brief Adds a variable to the specified table of variables.
 * 
 * @param variable A pointer to a valid SymbolVariable to be added.
 * @param table A pointer to a valid VariableTable into which to add the new variable entry.
 * @return SUCCESS If the variable was successfully added to the table of variables.
 * @return SEMANTIC_ERROR_DEFINITION If the variable with the specified name already exists.
 * @return INTERNAL_ERROR f there was a problem with adding the variable (like malloc).
 */
int addVariableToTable(SymbolVariable* variable, VariableTable* table);

/**
 * @brief Gets a reference to a variable in the specified table of variables.
 * 
 * @param id The name of the variable to find.
 * @param table A pointer to a VariableTable to look in.
 * @return SymbolVariable* A pointer to the found variable symbol or NULL if it wasn't found.
 */
SymbolVariable* getVariableFromTable(const char * id, VariableTable* table);

/**
 * @brief Cleans up all memory used by the variable table. It is guaranteed that this function is called only once and that, after it's called, no additional functions dealing with the table of variables are called.
 * 
 */
void freeVariableTable(VariableTable* table);


/**
 * @brief Initialisases the stack of variable tables used by the parser.
 * 
 * @return SUCCESS Initialisation was successful.
 * @return INTERNAL_ERROR Initialisation wasn't successfuly (malloc or similar).
 */
int initVariableTableStack();

/**
 * @brief Enters a new "frame" for defining variables; this becomes the "active" frame (like a stack push). See other functions for details.
 * 
 * @return SUCCESS Creation of new stack frame was successful.
 * @return INTERNAL_ERROR Creation of new stack frame wasn't successful (malloc or similar).
 */
int enterNewStackFrame();

/**
 * @brief Deletes and cleans up the memory used by the active variable stack frame and moves back to the previous one (like a stack pop). Using this function when the stack is empty is guaranteed not to happen.
 * 
 */
void leaveStackFrame();

/**
 * @brief Adds the specified variable to the active variable stack frame.
 * 
 * @param variable The variable to add.
 * @return SUCCESS uhh... you know what.
 * @return SEMANTIC_ERROR_DEFINITION The variable already exists in the active variable stack frame. (The varialbe may or may not exist in the previous stack frames, but this is does not concern the output of this function.)
 * @return INTERNAL_ERROR uhh... you know what
 */
int addVariable(SymbolVariable* variable);

/**
 * @brief Gets a reference to the FIRST occurence of the variable in the stack of variable tables. I.e. this function first looks in the current variable stack frame and if it doesn't find the variable there, it goes to the previous and so on until it hits the top of the stack.
 * 
 * @param id The name of the variable to find.
 * @return SymbolVariable* The FIRST occurence of the variable in the stack of variable tables or NULL if such a variable doesn't exist ANYWHERE.
 */
SymbolVariable* getVariable(const char* id);

/**
 * @brief Cleans up the entire stacks of variable tables.
 * 
 */
void freeVariableTableStack();

#endif SYMTABLE_H