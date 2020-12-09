/*File name: symtable.c --------------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20          |
 |Team:       124, varianta II                                            |
 |Authors:    Vojtěch Vlach (xvlach22)                                    |
 |            hashCode function copied from IAL homework 2 2020 c016.c    |
 |              Petr Přikryl (December 1994), Vaclav Topinka (2005)       |
 |              Karel Masařík (říjen 2014) and Radek Hranický (2014-2018) |
 |                                                                        |
 |    _      _     _   __                   __  _        _                |
 |   | |    (_)   | | /_/                  /_/ | |      | |               |
 |   | |     _  __| | ___   _   _  __   ___   _| |_ __ _| |__  _   _      |
 |   | |    | |/ _` |/ _ \ | | | | \ \ / / | | | __/ _` | '_ \| | | |     |
 |   | |____| | (_| |  __/ | |_| |  \ V /| |_| | || (_| | | | | |_| |     |
 |   |______|_|\__,_|\___|  \__,_|   \_/  \__, |\__\__,_|_| |_|\__,_|     |
 |                                         __/ |                          |
 |                                        |___/                           |
 *------------------------------------------------------------------------*/

#include "symtable.h"

// Represents the pseudo variable "_".
// Initialised in the initVariableTableStack function.
// This variable always exists and thus cannot be stored on any variable stack as there is no variable table that exists for the whole lifetime of the compilation process.
SymbolVariable blackHoleVariable;

/*
 * @brief counts hash from given string
 *
 * @param 	string_key	Key to count hash from.
 */
int hashCode (const char *string_key){
	int retval = 0;
	int keyLen = strlen(string_key);
	for(int i = 0; i < keyLen; i++){
		retval += string_key[i];
	}
	return ( retval % TABSIZE );
}

/*----------------------------------------- Function Table ------------------------------------------*/

/**
 * @brief Initialises the table of function for operation. It is guaranteed that this 
 *		function is called before any other function dealing with the table of functions and is called 
 *		only once. This function does NOT populate the table of functions with built-in functions.
 * 
 * @return SUCCESS If the initialisation was successful
 */
int initFunctionTable(){
	for (int i = 0; i < TABSIZE; ++i)
		FuncTab[i] = NULL;

	return SUCCESS;
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
int addFunction(const SymbolFunction* function){
	if(function == NULL)
		return INTERNAL_ERROR;

	int hash = hashCode(strGetStr(&function->id));

	FuncTabEl *ptr = FuncTab[hash];
	while(ptr != NULL){		// go through the linked list with given hash
		if(!strCmpString(&ptr->FuncData.id, &function->id))	// functions have same id
			return SEMANTIC_ERROR_DEFINITION;

		ptr = ptr->ptrNext;
	}
	return deepCopyFunction(function, hash);
}

/*
 * @brief Creates a deep copy of given function in HashTable
 *
 * @param function 	The function to add.
 * @param hash 		Hash of given function.
 *
 * @return 	SUCCESS 		If the function was added succesfully.
 * @return 	INTENRAL_ERROR 	If there was a problem with adding the function (like malloc).
 */
int deepCopyFunction(const SymbolFunction* function, int hash){
	// Alloc new function
	FuncTabEl *newElPtr = (FuncTabEl *)malloc(sizeof(FuncTabEl));
	
	if(newElPtr == NULL)
		return INTERNAL_ERROR;

	if(strInit(&newElPtr->FuncData.id) == STR_ERROR){
		free(newElPtr);
		return INTERNAL_ERROR;
	}

	if(strCopyString(&newElPtr->FuncData.id, &function->id) == STR_ERROR){
		free(newElPtr);
		strFree(&newElPtr->FuncData.id);
		return INTERNAL_ERROR;
	}

	// Alloc parametrs
	newElPtr->FuncData.parameters.params = (SymbolFunctionParameter *)malloc(sizeof(SymbolFunctionParameter) * function->parameters.count);
	if(newElPtr->FuncData.parameters.params == NULL){
		strFree(&newElPtr->FuncData.id);
		free(newElPtr);
		return INTERNAL_ERROR;
	}
	// Fill parametrs
	for(int i = 0; i < function->parameters.count; i++){
		if(strInit(&newElPtr->FuncData.parameters.params[i].id) == STR_ERROR ||
			strCopyString(&newElPtr->FuncData.parameters.params[i].id, &function->parameters.params[i].id) == STR_ERROR){
			// if str_error, free this function
			// first prepare function to properly free itself
			newElPtr->FuncData.parameters.count = i + 1;
			newElPtr->FuncData.returnTypes.count = 0;

			// free function
			freeFunction(&newElPtr->FuncData);
			free(newElPtr);

			return INTERNAL_ERROR;
		}
		newElPtr->FuncData.parameters.params[i].type = function->parameters.params[i].type;
	}
	newElPtr->FuncData.parameters.count = function->parameters.count;

	// Alloc returntype
	newElPtr->FuncData.returnTypes.types = (DataType *)malloc(sizeof(DataType) * function->returnTypes.count);
	if(newElPtr->FuncData.returnTypes.types == NULL){
		freeFunction(&newElPtr->FuncData);
		free(newElPtr);
		return INTERNAL_ERROR;
	}
	// Fill return Types
	for(int i = 0; i < function->returnTypes.count; i++){
		newElPtr->FuncData.returnTypes.types[i] = function->returnTypes.types[i];
	}
	newElPtr->FuncData.returnTypes.count = function->returnTypes.count;

	newElPtr->FuncData.builtIn = function->builtIn;
	newElPtr->FuncData.parameters.variadic = function->parameters.variadic;

	// Join new function to the linked list of synonyms
	newElPtr->ptrNext = FuncTab[hash];
	FuncTab[hash] = newElPtr;

	return SUCCESS;
}

/**
 * @brief Gets a reference to a function in the table of functions.
 * 
 * @param id The name of the function to find.
 * 
 * @return const SymbolFunction* A pointer to the found function symbol or NULL if it wasn't found.
 */
const SymbolFunction* getFunction(const char* id){
	if(id == NULL)
		return NULL;

	int hash = hashCode(id);

	FuncTabEl *ptr = FuncTab[hash];
	while(ptr != NULL){		// go through the linkd list with given hash
		if(!strCmpConstStr(&ptr->FuncData.id, id)){	// functions have same id
			return &ptr->FuncData;
		}
		else{
			ptr = ptr->ptrNext;
		}
	}

	return NULL;
}

/**
 * @brief Cleans up all memory used by the function table. It is guaranteed that this function 
 * 		is called only once and that, after it's called, no additional functions dealing 
 *		with the table of functions are called.
 */
void freeFunctionTable(){
	if(FuncTab == NULL)
		return;

	for(int i = 0; i < TABSIZE; i++){
		// free this FuncTabEl
		FuncTabEl **ptrptr = &FuncTab[i];
		while(*ptrptr != NULL){
			FuncTabEl *ptrNext = (*ptrptr)->ptrNext;

			freeFunction(&(*ptrptr)->FuncData);

			free(*ptrptr);
			*ptrptr = ptrNext;
		}
	}
}

void freeFunction(SymbolFunction *func){
	free(func->returnTypes.types);

	for(int i = 0; i < func->parameters.count; i++)
		strFree(&func->parameters.params[i].id);

	free(func->parameters.params);

	strFree(&func->id);
}

/*------------------------------------------ Variable Table -----------------------------------------*/

/**
 * @brief Initialises the table of variables pointed by table. It is guaranteed that table points to a 
 *		valid block of memory of size sizeof(VariableTable)
 * 
 * @param table The table to be initialised.
 * 
 * @return SUCCESS If the function initialised the variable table succesfully.
 */
int initVariableTable(VariableTable* table){
	for (int i = 0; i < TABSIZE; ++i)
		(*table)[i] = NULL;

	return SUCCESS;
}

/**
 * @brief Adds a variable to the specified table of variables. Performs a deep copy of given variable.
 * 
 * @param variable A pointer to a valid SymbolVariable to be added.
 * @param table A pointer to a valid VariableTable into which to add the new variable entry.
 * @return SUCCESS If the variable was successfully added to the table of variables.
 * @return SEMANTIC_ERROR_DEFINITION If the variable with the specified name already exists.
 * @return INTERNAL_ERROR If there was a problem with adding the variable (like malloc).
 */
int addVariableToTable(const SymbolVariable* variable, VariableTable* table){
	if(table == NULL || variable == NULL)
		return INTERNAL_ERROR;

	int hash = hashCode(strGetStr(&variable->id));

	VarTabEl *ptr = (*table)[hash];
	while(ptr != NULL){		// go through the linked list with given hash
		if(!strCmpString(&ptr->VarData.id, &variable->id))
			return SEMANTIC_ERROR_DEFINITION;
		ptr = ptr->ptrNext;
	}

	// create a new Variable element
	VarTabEl *newElPtr = (VarTabEl *)malloc(sizeof(VarTabEl));
	if(newElPtr == NULL)
		return INTERNAL_ERROR;

	if(strInit(&newElPtr->VarData.id) == STR_ERROR ||
		strCopyString(&newElPtr->VarData.id, &variable->id) == STR_ERROR){
		strFree(&newElPtr->VarData.id);
		free(newElPtr);
		return INTERNAL_ERROR;
	}

	newElPtr->VarData.type = variable->type;
	newElPtr->VarData.uid = variable->uid;

	newElPtr->ptrNext = (*table)[hash];
	(*table)[hash] = newElPtr;

	return SUCCESS;
}

/**
 * @brief Gets a reference to a variable in the specified table of variables.
 * 
 * @param id The name of the variable to find.
 * @param table A pointer to a VariableTable to look in.
 * @return SymbolVariable* A pointer to the found variable symbol or NULL if it wasn't found.
 */
const SymbolVariable* getVariableFromTable(const char * id, VariableTable* table){
	if(table == NULL)
		return NULL;

	int hash = hashCode(id);

	VarTabEl *ptr = (*table)[hash];
	while(ptr != NULL){		// go through the linked list with given hash
		if(!strCmpConstStr(&ptr->VarData.id, id)){
			return &ptr->VarData;
		}
		else{
			ptr = ptr->ptrNext;
		}
	}

	return NULL;
}

/**
 * @brief Cleans up all memory used by the variable table. It is guaranteed that this function 
 *		is called only once and that, after it's called, no additional functions dealing 
 *		with the table of variables are called.
 */
void freeVariableTable(VariableTable* table){
	if(table == NULL)
		return;

	for (int i = 0; i < TABSIZE; ++i){
		VarTabEl **ptr = &(*table)[i];
		while(*ptr != NULL){
			VarTabEl *ptrNext = (*ptr)->ptrNext;

			strFree(&(*ptr)->VarData.id);
			free(*ptr);

			*ptr = ptrNext;
		}
	}
}

/*--------------------------------------- Variable Table Stack --------------------------------------*/

/**
 * @brief Initialisases the two stacks of variable tables used by the parser. mainStack and binStack.
 * 
 * @return SUCCESS Initialisation was successful.
 */
int initVariableTableStack(){
	mainStack = NULL;
	binStack = NULL;
	if (strInit(&blackHoleVariable.id) == STR_ERROR || strCopyConstString(&blackHoleVariable.id,"_") == STR_ERROR){
		strFree(&blackHoleVariable.id);
		return INTERNAL_ERROR;
	}
	blackHoleVariable.type = TypeBlackHole;
	return SUCCESS;
}

/**
 * @brief Enters a new "frame" for defining variables; this becomes the "active" frame (like a stack push). 
 *		See other functions for details.
 * 
 * @return SUCCESS Creation of new stack frame was successful.
 * @return INTERNAL_ERROR Creation of new stack frame wasn't successful (malloc or similar).
 */
int enterNewStackFrame(){
	StackEl *newFramePtr;
	if((newFramePtr = (StackEl *)malloc(sizeof(StackEl))) == NULL)
		return INTERNAL_ERROR;

	initVariableTable(&newFramePtr->table);

	newFramePtr->ptrNext = mainStack;
	mainStack = newFramePtr;

	return SUCCESS;
}

/**
 * @brief Saves reference on active variable to binStack and deletes from mainStack (like a stack pop)
 * 			Using this function when the stack is empty is guaranteed not to happen.
 */
void leaveStackFrame(){
	if(mainStack == NULL)
		return;

	StackEl *formerMainHead = mainStack;
	mainStack = mainStack->ptrNext;

	formerMainHead->ptrNext = binStack;
	binStack = formerMainHead;
}

/**
 * @brief Adds the specified variable to the active variable stack frame.
 * 
 * @param variable The variable to add.
 * 
 * @return SUCCESS If the variable was successfully added to active table of variables.
 * @return INTERNAL_ERROR If there was a problem with adding the variable (like malloc).
 * @return SEMANTIC_ERROR_DEFINITION The variable already exists in the active variable stack frame. 
 *		(The variable may or may not exist in the previous stack frames, but this is does not concern 
 *		the output of this function.)
 */
int addVariable(SymbolVariable* variable){
	static int uid = 0;
	variable->uid = uid;
	uid++;
	return addVariableToTable(variable, &mainStack->table);
}

/**
 * @brief Gets a reference to the FIRST occurence of the variable in the stack of variable tables. 
 *		I.e. this function first looks in the current variable stack frame and if it doesn't find 
 *		the variable there, it goes to the previous and so on until it hits the end of the stack.
 * 
 * @param id The name of the variable to find.
 * @return SymbolVariable* The FIRST occurence of the variable in the stack of variable tables 
 *		or NULL if such a variable doesn't exist in mainStack.
 */
const SymbolVariable* getVariable(const char* id){
	if (!strCmpConstStr(&blackHoleVariable.id,id))
		return &blackHoleVariable;
	
	StackEl *ptr = mainStack;

	while (ptr != NULL){
		const SymbolVariable *varPtr = getVariableFromTable(id, &ptr->table);
		if(varPtr != NULL)
			return varPtr;
		ptr = ptr->ptrNext;
	}

	return NULL;
}

/**
 * @brief Cleans up both stacks of variable tables, mainStack and binStack, and the smart string used in the blackHoleVariable object.
 */
void freeVariableTableStack(){
	freeStack(&mainStack);
	freeStack(&binStack);
	strFree(&blackHoleVariable.id);
}

/**
 * @brief	Cleans up the entire given stack and frees the memory
 */
void freeStack(Stack *stack){
	while (*stack != NULL){
		StackEl *ptrNext = (*stack)->ptrNext;
		freeVariableTable(&(*stack)->table);
		free(*stack);
		*stack = ptrNext;
	}
}
