/*File name: scanner.c ----------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Ondřej Sloup (xsloup02)                                |
 |                                                                   |
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


#include <stdio.h>

#include "expression_analysis.h"
#include "token.h"
#include "error.h"
#include "str.h"



int initStack(Stack *stack, int64_t initialSize) {
    stack->values = malloc(initialSize * sizeof(int64_t));
    if (stack->values == NULL) {
        return INTERNAL_ERROR;
    }
    stack->used = 0;
    stack->size = initialSize;
    return SUCCESS;
}

int pushToStack(Stack *stack, int operation) {
    if (stack->used == stack->size) {
        stack->size *= 2;
        stack->values = realloc(stack->values, stack->size * sizeof(int64_t));
        if (stack->values == NULL) {
            return INTERNAL_ERROR;
        }
    }
    stack->values[stack->used++] = operation;
    return SUCCESS;
}

int popFromStack(Stack *stack, int *operation) {
    operation = stack->values[stack->used];
    stack->values = realloc(stack->values, stack->size-- * sizeof(int64_t));
    if (stack->values == NULL) {
        return INTERNAL_ERROR;
    }
    return SUCCESS;
}

int seekValueStack(Stack *stack) {
    return stack->values[stack->used];
}

void freeStack(Stack *stack) {
    free(stack->values);
    stack->values = NULL;
    stack->used = stack->size = 0;
}

int checkIfValidToken(Token *token, Stack *stack) {
    getToken(&token);
    switch (token->type) {
        case TokenAdd:
            addToStack(&stack, OperatorAdd);
            break;
        case TokenSubtract:
            addToStack(&stack, OperatorSubtract);
            break;
        case TokenMultiply:
            addToStack(&stack, OperatorMultiple);
            break;
        case TokenDivide:
            addToStack(&stack, OperatorDivide);
            break;
        case TokenIsLessThan:
            addToStack(&stack, OperatorLessThan);
            break;
        case TokenIsLessEqual:
            addToStack(&stack, OperatorGreaterEqual);
            break;
        case TokenIsGreaterEqual:
            addToStack(&stack, OperatorIsEqual);
            break;
        case TokenIsGreaterThan:
            addToStack(&stack, OperatorGreaterThan);
            break;
        case TokenIsEqual:
            addToStack(&stack, OperatorIsEqual);
            break;
        case TokenNotEqual:
            addToStack(&stack, OperatorIsNotEqual);
            break;
        case TokenLeftBracket:
            addToStack(&stack, OperatorLeftBracket);
            break;
        case TokenRightBracket:
            addToStack(&stack, OperatorRightBracket);
            break;
        case TokenIdentifier:
            addToStack(&stack, OperatorId);
            break;
        case TokenWholeNbr:
            addToStack(&stack, OperatorWholeNumber);
            break;
        case TokenDecimalNbr:
            addToStack(&stack, OperatorDecimal);
            break;
        case TokenStringLiteral:
            addToStack(&stack, OperatorString);
            break;
    default:
        return SYNTAX_ERROR;
        break;
    }
    return SUCCESS;
}

int parseExpression(Expression* expression) {
    /* Stack init */
    Stack *stack;
    initStack(&stack, 1);
    addToStack(&stack, OperatorEnd);

    /* getToken */
    Token token;

    while (stack->values[0] == OperatorEnd) {
        if(checkIfValidToken(&token, &stack) == SYNTAX_ERROR) {
            return SYNTAX_ERROR;
        }
        break;
    }
    freeStack(&stack);
    return SUCCESS;
}



