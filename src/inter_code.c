/*File name: inter_code.c -------------------------------------------*
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

#include "inter_code.h"

/**
 *	@brief	Initialize Instruction list. When called multiple times, can cause memory leak.
 */
void initInstructionList(){
	InstrListFirst = NULL;
	InstrListLast = NULL;
}

/**
 *	@brief	Add instruction to instruction list, copy operands and result pointers (or creates deep copy, idk)
 *
 *	@param	
 *	@param	
 *	@param	
 *	@param	
 *
 *	@return	SUCCESS	If addition gone successfully
 *	@return INTERNAL_ERROR	If there was a problem with adding the function (like malloc)
 */
int addInstruction(/* TODO */ int data){
	// malloc new instruction
	Instruction *newInstruction = (Instruction *)malloc(sizeof(Instruction));
	if(newInstruction == NULL)
		return INTERNAL_ERROR;

	// copy all parametrs to new instruction
	newInstruction->ptrNext = NULL;
	newInstruction->ptrPrev = InstrListLast;
	/* TODO */
	newInstruction->data = data;

	// link to the END of a list
	if(InstrListLast == NULL){		// vkladame prvni instrukci seznamu
		InstrListFirst = InstrListLast = newInstruction;
		if(InstrListFirst == NULL)
			printf("blbyyyyy\n");
	}
	else{
		InstrListLast->ptrNext = newInstruction;
		InstrListLast = newInstruction;
	}
	return SUCCESS;
}

/**
 *	@brief	Return pointer to first instruction in list or NULL
 *
 *	@return 	Pointer to firstfunction in list 	If list is not empty
 *	@return 	NULL	If list is empty
 */
Instruction *GetFrist(){
	return InstrListFirst;
}

/**
 *	@brief	Delete first instruction in a list
 */
void deleteFirst(){
	if(InstrListFirst == InstrListLast){
		free(InstrListFirst);
		InstrListLast = InstrListFirst = NULL;
	}
	else{
		Instruction *deleteInstr = InstrListFirst;
		InstrListFirst = InstrListFirst->ptrNext;
		InstrListFirst->ptrPrev = NULL;
		free(deleteInstr);
	}
}

/**
 *	@brief	free the list
 */
void freeList(){
	while(InstrListFirst != NULL){
		Instruction *deleteInstr = InstrListFirst;
		InstrListFirst = InstrListFirst->ptrNext;
		free(deleteInstr);
	}
	InstrListLast = NULL;
}
