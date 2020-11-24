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
//#include "./../../src/data_type.h"
#include <stdlib.h>
#include <stdlib.h>

#define DATATYPEMAX 3
#define EMPTYID "emptyID"

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

/*
 * @brief Prints out all function ids from FuncTab
 */
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

void prepareFunction(SymbolFunction *function, char *id, int paramsCount, int returnTypesCount){
	//printf("========= prepareFunction =====\n");
	//printf("function->parameters.count: %d\n", function->parameters.count);
	strInit(&function->id);
	strCopyConstString(&function->id, id);
	//function->id = str;
	//printf("function id: %s\n", strGetStr(&function->id));
	function->parameters.count = paramsCount;
	function->parameters.params = (SymbolFunctionParameter *)malloc(sizeof(SymbolFunctionParameter) * paramsCount);
	for(int i = 0; i < paramsCount; i++){
		strInit(&function->parameters.params[i].id);
		strCopyConstString(&function->parameters.params[i].id, EMPTYID);
		function->parameters.params[i].type = (TypeInt + i) % DATATYPEMAX;
	}
	function->returnTypes.count = returnTypesCount;
	function->returnTypes.types = (DataType *)malloc(sizeof(DataType) * returnTypesCount);
	for(int i = 0; i < returnTypesCount; i++){
		function->returnTypes.types[i] = (TypeInt + i) % DATATYPEMAX;
		//printf("new type: %d\n", function->returnTypes.types[i]);
	}
}

/*===================== individual method tests =====================*/

void hashCodeTest(char** key, int count){
	string str;
	string *s = &str;
	for(int i = 0; i < count; i++){
		strInit(s);
		strCopyConstString(s, key[i]);
		int hash = hashCode(s);
		printf("hash(%s): \t%d\n", key[i], hash);
		strFree(s);
	}
}

void addFunctionTest(char *id, int paramsCount, int returnTypesCount){
	SymbolFunction newFunction;
	prepareFunction(&newFunction, id, paramsCount, returnTypesCount);

	int result = addFunction(&newFunction);

	switch (result){
		case SUCCESS:
			printf("Function added (%s).\n", id); break;
		case INTERNAL_ERROR:
			printf("function returned INTERNAL_ERROR\n"); break;
		case SEMANTIC_ERROR_DEFINITION:
			printf("Function already exists in FunctionTable\n");
	}
}

int main(int argc, char const *argv[]) {
	printf("\nSymtable tests\n======================================\n");

	printf("\nHashCode function tests:\n");
	char *key[] = {"Hello", "there", "how", "are", "you", "I", "am", "fine"};
	int count = 8;
	hashCodeTest(key, count);

	printf("\nInitFunctionTable test\n");
	initFunctionTable();
	printFuncTable();

	printf("\nAddFunction method test\n");
	for(int i = 0; i < count; i++){
		addFunctionTest(key[i], i % 3, i % 3);
	}
	printFuncTable();


	printf("Thank you for watching.\n======================================\n");
	return 0;
}
