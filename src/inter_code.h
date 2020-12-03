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

#ifndef INTER_CODE_H
#define INTER_CODE_H
#include "error.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _instruction{
	struct _instruction *ptrNext;
	/*Operation *operation;
	ExpExp *operand1;
	ExpExp *operand2;
	Result *result;*/
	int data;	/* TOD */
} Instruction;

Instruction *InstrListFirst;
Instruction *InstrListLast;

// Initialize inctruction list
void initInstructionList();

// Add instruction to instruction list, copy operands and result pointers (or creates deep copy, idk)
// return SUCCESS or INTERNAL_ERROR
int addInstruction(/* TODO */);

Instruction *GetFrist();

void deleteFirst();

void freeList();

#endif

