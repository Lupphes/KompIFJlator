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
#include "parser_common.h"

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
    *operation = seekValueStack(stack);
    stack->values = realloc(stack->values, (stack->size - 1) * sizeof(int64_t));
    return SUCCESS;
}

int seekValueStack(Stack *stack) {
    return stack->values[(stack->used - 1)];
}

void freeStack(Stack *stack) {
    free(stack->values);
    stack->values = NULL;
    stack->used = stack->size = 0;
}

int checkIfValidToken(Token *token, Stack *stack) {
    int returnCode;
    acceptAny();
    switch (token->type) {
        case TokenAdd:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorAdd);
            break;
        case TokenSubtract:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorSubtract);
            break;
        case TokenMultiply:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorMultiply);
            break;
        case TokenDivide:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorDivide);
            break;
        case TokenIsLessThan:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorIsLessThan);
            break;
        case TokenIsLessEqual:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorIsGreaterEqual);
            break;
        case TokenIsGreaterThan:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorIsGreaterThan);
            break;
        case TokenIsGreaterEqual:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorIsEqual);
            break;
        case TokenIsEqual:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorIsEqual);
            break;
        case TokenNotEqual:
            if(isInStackOperator(&stack)) 
                return SYNTAX_ERROR;
            pushToStack(stack, OperatorNotEqual);
            break;
        case TokenLeftBracket:
            pushToStack(stack, OperatorLeftBracket);
            break;
        case TokenRightBracket:
            pushToStack(stack, OperatorRightBracket);
            break;
        case TokenIdentifier:
        case TokenWholeNbr:
        case TokenDecimalNbr:
        case TokenStringLiteral:
            int returnCode;
            if(isInStackExpressionOrIdentifier(&stack)) 
                return SYNTAX_ERROR;
            Term* term = malloc(sizeof(term));
            if(returnCode = parseTerm(term) == SUCCESS){
                parseTerm(&term);
                switch (term->type) {
                case TokenWholeNbr:
                    pushToStack(stack, OperatorWholeNumber);
                    break;
                case TokenDecimalNbr:
                    pushToStack(stack, OperatorDecimal);
                    break;
                case TokenStringLiteral:
                    pushToStack(stack, OperatorStringLiteral);
                    break;
                case TokenIdentifier:
                    pushToStack(stack, OperatorIdentifier);
                    break;
                }
            } else {
                free(term);
                return returnCode;
            }
            break;
    default:
        return SYNTAX_ERROR;
        break;
    }
    return SUCCESS;
}

bool isInStackOperator(Stack *stack) {
    int lastValue = seekValueStack(stack);
    switch (lastValue) {
    case OperatorAdd:
    case OperatorSubtract:
    case OperatorMultiply:
    case OperatorDivide:
    case OperatorIsLessThan:
    case OperatorIsLessEqual:
    case OperatorIsGreaterThan:
    case OperatorIsGreaterEqual:
    case OperatorIsEqual:
    case OperatorNotEqual:
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool isInStackExpressionOrIdentifier(Stack *stack) {
    int lastValue = seekValueStack(stack);
    switch (lastValue) {
    case OperatorIdentifier:
    case OperatorExpression:
    case OperatorWholeNumber:
    case OperatorDecimal:
    case OperatorStringLiteral:
        return true;
        break;
    default:
        return false;
        break;
    }
}

int parseExpression(Expression* expression) {
    /* Stack init */
    Stack stack;
    initStack(&stack, 1);
    pushToStack(&stack, OperatorEnd);
    while (checkIfValidToken(&curTok, &stack) == SUCCESS) {
        
    }
    // TODO: generateAST(); 


    freeStack(&stack);
    return SUCCESS;
}



