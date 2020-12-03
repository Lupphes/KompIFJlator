/*File name: inter_code.h -------------------------------------------*
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

#include <stdlib.h>
#include <stdio.h>
#include "../../src/inter_code.h"

void printListData(){
	if(InstrListFirst == NULL){
		printf("List is empty\n");
		return;
	}

	Instruction *ptr = InstrListFirst;
	printf("\t");
	while (ptr != NULL){
		printf("%d, ", ptr->data);
		ptr = ptr->ptrNext;
	}
	printf("\n");
}

void addInstructionTest(int data){
	int result = addInstruction(data);
	switch (result){
		case SUCCESS:
			printf("instruction added (%d)\n", data); break;
		case INTERNAL_ERROR:
			printf("instruction NOT added (%d)\n", data); break;
	}
}

int main(){	
	printf("\nInitInstructionList test\n");
	printf("--------------------------------------\n");
	initInstructionList();
	printListData();

	printf("\naddInstruction method test - add 10 items\n");
	printf("--------------------------------------\n");
	for(int i = 0; i < 10; i++)
		addInstructionTest(i * 2);
	printListData();

	printf("\ndeleteFirst method test - delete 10 items\n");
	printf("--------------------------------------\n");
	for(int i = 0; i < 10; i++)
		deleteFirst();
	printListData();

	printf("\naddInstruction method test - add 10 items\n");
	printf("--------------------------------------\n");
	for(int i = 0; i < 10; i++)
		addInstructionTest(i * 2 + 1);
	printListData();

	printf("\nfreeList test\n");
	printf("--------------------------------------\n");
	freeList();
	printListData();

	return 0;
}