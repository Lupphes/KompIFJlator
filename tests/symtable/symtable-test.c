/*File name: symtable-test.c ----------------------------------------*
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

#include "./../../src/symtable.h"
#include <stdlib.h>
#include <stdlib.h>

#define DATATYPEMAX 3
#define EMPTYID "emptyID"

// print DataType according to given number
char *printDataType(DataType type){
	switch (type){
		case TypeInt:
			return "TypeInt"; break;
		case (TypeFloat64):
			return "TypeFloat64"; break;
		case (TypeString):
			return "TypeString"; break;
	}
	return "";
}

/*=========================== Function table methods ==========================*/

// Print given function to one line
void printFunction(SymbolFunction *function){
	printf("\t%s, ", strGetStr(&function->id));
	printf("%d{", function->parameters.count);	// print parameters count
	for(int i = 0; i < function->parameters.count; i++){
		// print all parameters	
		printf("(%s, ", strGetStr(&function->parameters.params[i].id));	// param id
		printf("%s)", printDataType(function->parameters.params[i].type));	// param type
		if(function->parameters.count - i > 1)	
			printf(", ");
	}
	printf("}, %d{", function->returnTypes.count);
	for(int i = 0 ; i < function->returnTypes.count; i++){
		printf("%s", printDataType(function->returnTypes.types[i]));
		if(function->returnTypes.count - i > 1)	
			printf(", ");
	}
	printf("}\n");
}

// Print out all functions from FuncTab
void printFuncTable(){
	printf("\n");
	if(FuncTab == NULL){
		printf("FunctionTable empty.\n");
		return;
	}

	int empty = 1;	//true

	for(int i = 0; i < TABSIZE; i++){
		FuncTabEl *ptr = FuncTab[i];
		if(ptr == NULL){
			continue;
		}
		empty = 0; 	// false
		printf("  %d.", i);
		while(ptr != NULL){
			printFunction(&ptr->FuncData);
			ptr = ptr->ptrNext;
		}
	}
	if(empty){
		printf("The FunctionTable is empty.\n");
	}
	printf("\n");
}

// prepare function to add to FuncTab according to given arguments
void prepareFunction(SymbolFunction *function, char *id, int paramsCount, int returnTypesCount){
	strInit(&function->id);
	strCopyConstString(&function->id, id);

	// fill all parameters
	function->parameters.count = paramsCount;
	function->parameters.params = (SymbolFunctionParameter *)malloc(sizeof(SymbolFunctionParameter) * paramsCount);

	for(int i = 0; i < paramsCount; i++){
		strInit(&function->parameters.params[i].id);
		strCopyConstString(&function->parameters.params[i].id, EMPTYID);
		function->parameters.params[i].type = (TypeInt + i) % DATATYPEMAX;
	}

	// fill all return types 
	function->returnTypes.count = returnTypesCount;
	function->returnTypes.types = (DataType *)malloc(sizeof(DataType) * returnTypesCount);

	for(int i = 0; i < returnTypesCount; i++)
		function->returnTypes.types[i] = (TypeInt + i) % DATATYPEMAX;
}

// delete given function and free memory
void freeFunctionPtrPtr(SymbolFunction **function){
	free((*function)->returnTypes.types);

	for(int i = 0; i < (*function)->parameters.count; i++){
		strFree(&((*function)->parameters.params[i].id));
	}

	free((*function)->parameters.params);
	strFree(&((*function)->id));
	free(*function);
}

/*--------------------- Individual function method tests ------------------*/

void addFunctionTest(char *id, int paramsCount, int returnTypesCount){
	SymbolFunction *newFunction = (SymbolFunction *)malloc(sizeof(SymbolFunction));
	prepareFunction(newFunction, id, paramsCount, returnTypesCount);


	int result = addFunction(newFunction);

	switch (result){
		case SUCCESS:
			printf("Function added (%s).\n", id); break;
		case INTERNAL_ERROR:
			printf("function returned INTERNAL_ERROR\n"); break;
		case SEMANTIC_ERROR_DEFINITION:
			printf("Function (%s) already exists in FunctionTable\n", id);
	}
	freeFunctionPtrPtr(&newFunction);
	//freeFunction(newFunction);
	//free(newFunction);
}

void getFunctionTest(char * id, int paramsCount){
	const SymbolFunction *function = getFunction(id);
	if(function != NULL && function->parameters.count == paramsCount){
		printf("Function found (%s)\n", id);
	}
	else {
		printf("Function NOT found (%s)\n", id);	
	}
}

/*=========================== Variable method tests ==========================*/

void printVarTable(VariableTable *varTab){
	printf("\n");
	if(varTab == NULL){
		printf("VariableTable not initialized.\n");
		return;
	}

	int empty = 1;	//true

	for(int i = 0; i < TABSIZE; i++){
		VarTabEl *ptr = (*varTab)[i];
		if(ptr == NULL){
			continue;
		}
		empty = 0; 	// false
		printf("  %d.", i);
		while(ptr != NULL){
			printf("\t%s, %s\n", strGetStr(&ptr->VarData.id), printDataType(ptr->VarData.type));
			ptr = ptr->ptrNext;
		}
	}
	if(empty){
		printf("The VariableTable is empty.\n");
	}
	printf("\n");
}

/*--------------------- Individual Variable method tests ------------------*/

// Prepares new variable and adds it to table
void addVariableToTableTest(VariableTable * varTabPtr, char *id, DataType type){
	SymbolVariable newVariable;
	strInit(&newVariable.id);
	strCopyConstString(&newVariable.id, id);

	newVariable.type = type;

	int result = addVariableToTable(&newVariable, varTabPtr);

	switch (result){
		case SUCCESS:
			printf("Variable added (%s)\n", id); break;
		case SEMANTIC_ERROR_DEFINITION:
			printf("Variable already in Table (%s)\n", id); break;
		case INTERNAL_ERROR:
			printf("Malloc error (%s)\n", id); break;
	}

	strFree(&newVariable.id);
}


int main(int argc, char const *argv[]) {
	char *keysF[] = {"Hello", "there", "how", "are", "you", "I", "am", "fine"};
	int countF = 8;
	char *keysV[] = {"sum", "var", "i", "height", "width", "true", "false", "type", "ptr"};
	int countV = 9;

	VariableTable *varTabPtr = (VariableTable *)malloc(sizeof(VariableTable));

	printf("\nSymtable tests\n");
	printf("======================================\n");
	printf("======================================\n");
/*---------------------------- Function Table TESTS ----------------------------*/
	printf("\nFunction table tests\n");
	printf("======================================\n");

	printf("\nHashCode function tests:\n");
	for(int i = 0; i < countF; i++)
		printf("hash(%s): \t%d\n", keysF[i], hashCode(keysF[i]));
	
	printf("\nInitFunctionTable test\n");
	printf("--------------------------------------\n");
	initFunctionTable();
	printFuncTable();

	printf("\nfreeFunctionTable test - free empty table\n");
	printf("--------------------------------------\n");
	freeFunctionTable();
	printFuncTable();

	printf("\nInitFunctionTable test again\n");
	printf("--------------------------------------\n");
	initFunctionTable();
	printFuncTable();

	printf("\nAddFunction method test\n");
	printf("--------------------------------------\n");
	for(int i = 0; i < countF; i++)
		addFunctionTest(keysF[i], i % 3, i % 3);
	printFuncTable();
	
	printf("\nAddFunction existing method test\n");
	printf("--------------------------------------\n");
	addFunctionTest(keysF[2], 2, 2);
	printFuncTable();

	printf("\ngetFunction test - should return functions.\n");
	printf("--------------------------------------\n");
	for (int i = 0; i < countF; i++)
		getFunctionTest(keysF[i], i % 3);

	printf("\ngetFunction test - should return NULL.\n");
	printf("--------------------------------------\n");
	getFunctionTest("Compiler", 0);

	printf("\nfreeFunctionTable test\n");
	printf("--------------------------------------\n");
	freeFunctionTable();
	printFuncTable();

/*---------------------------- Variable Table TESTS ----------------------------*/

	printf("\nHashCode function tests:\n");
	for(int i = 0; i < countV; i++)
		printf("hash(%s): \t%d\n", keysV[i], hashCode(keysV[i]));
	
	printf("\nVariable table tests\n");
	printf("======================================\n");

	printf("\nInitVariableTable test\n");
	printf("--------------------------------------\n");
	initVariableTable(varTabPtr);
	printVarTable(varTabPtr);

	printf("\nfreeVariableTable test - free empty table\n");
	printf("--------------------------------------\n");
	freeVariableTable(varTabPtr);
	printVarTable(varTabPtr);

	printf("\nInitVariableTable test again\n");
	printf("--------------------------------------\n");
	initVariableTable(varTabPtr);
	printVarTable(varTabPtr);

	printf("\addVariableToTable method test\n");
	printf("--------------------------------------\n");
	for(int i = 0; i < countV; i++)
		addVariableToTableTest(varTabPtr, keysV[i], i % 3);
	printVarTable(varTabPtr);

	printf("\addVariableToTable with existing variable\n");
	printf("--------------------------------------\n");
	addVariableToTableTest(varTabPtr, keysV[2], 2);
	printVarTable(varTabPtr);


	printf("\nThank you for watching.\n");
	printf("======================================\n");
	printf("======================================\n");
	return 0;
}
