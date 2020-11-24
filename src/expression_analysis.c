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
#include "scanner.c"

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
    if (stack->values == NULL) {
        return INTERNAL_ERROR;
    }
    *operation = getLastValueStack(stack);
    stack->values = realloc(stack->values, (stack->size - 1) * sizeof(int64_t));
    return SUCCESS;
}

int getLastValueStack(Stack *stack) {
    return stack->values[(stack->used - 1)];
}

void freeStack(Stack *stack) {
    free(stack->values);
    stack->values = NULL;
    stack->used = stack->size = 0;
}

int checkIfValidToken(Token *token, Stack *stack) {
    getToken(token);
    switch (token->type) {
        case TokenAdd:
            pushToStack(stack, OperatorAdd);
            break;
        case TokenSubtract:
            pushToStack(stack, OperatorSubtract);
            break;
        case TokenMultiply:
            pushToStack(stack, OperatorMultiple);
            break;
        case TokenDivide:
            pushToStack(stack, OperatorDivide);
            break;
        case TokenIsLessThan:
            pushToStack(stack, OperatorLessThan);
            break;
        case TokenIsLessEqual:
            pushToStack(stack, OperatorGreaterEqual);
            break;
        case TokenIsGreaterEqual:
            pushToStack(stack, OperatorIsEqual);
            break;
        case TokenIsGreaterThan:
            pushToStack(stack, OperatorGreaterThan);
            break;
        case TokenIsEqual:
            pushToStack(stack, OperatorIsEqual);
            break;
        case TokenNotEqual:
            pushToStack(stack, OperatorIsNotEqual);
            break;
        case TokenLeftBracket:
            pushToStack(stack, OperatorLeftBracket);
            break;
        case TokenRightBracket:
            pushToStack(stack, OperatorRightBracket);
            break;
        case TokenIdentifier:
            pushToStack(stack, OperatorId);
            break;
        case TokenWholeNbr:
            pushToStack(stack, OperatorWholeNumber);
            break;
        case TokenDecimalNbr:
            pushToStack(stack, OperatorDecimal);
            break;
        case TokenStringLiteral:
            pushToStack(stack, OperatorString);
            break;
    default:
        return SYNTAX_ERROR;
        break;
    }
    return SUCCESS;
}

int parseExpression(Expression* expression) {
    /* Stack init */
    Stack stack;
    initStack(&stack, 1);
    pushToStack(&stack, OperatorEnd);

    /* getToken */
    Token token;

    while (stack.values[0] == OperatorEnd) {
        if(checkIfValidToken(&token, &stack) == SYNTAX_ERROR) {
            return SYNTAX_ERROR;
        }
        break;
    }
    // TODO: generateAST(); 


    freeStack(&stack);
    return SUCCESS;
}



