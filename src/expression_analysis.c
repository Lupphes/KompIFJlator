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
#include "operator_table.h"

#define ANALYSIS_END -1

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

int useRule() {
    return SUCCESS;
}

// int translateToPSATable(Stack *stack, int *operator) {
//     switch (seekValueStack(stack)) {
//     case OperatorAdd:
//         break;
//     case OperatorSubtract:
//         break;
//     case OperatorMultiply:
//         break;
//     case OperatorDivide:
//         break;
//     case OperatorLeftBracket:
//         break;
//      case OperatorRightBracket:
//         break;
//     case OperatorIsLessThan:
//         break;
//     case OperatorIsLessEqual:
//         break;
//     case OperatorIsGreaterThan:
//         break;
//     case OperatorIsGreaterEqual:
//         break;
//     case OperatorIsEqual:
//         break;
//     case OperatorNotEqual:
//         break;
//     case OperatorEnd:
//         break;
//     case OperatorLeftAssociative:
         
//         break;
//     case OperatorRightAssociative: // use rules
//          evaluateExpression();
//         break;
//     default:
//         break;
//     }
//     return SUCCESS;
// }


int evaluateExpression(Stack *stack, int *operator) {
    Stack expression;
    switch (_PSATable[seekValueStack(stack)][*operator]) {
    case OperatorLeftAssociative:
        pushToStack(stack, _PSATable[seekValueStack(stack)][*operator]);
        pushToStack(stack, *operator);
        break;
    case OperatorRightAssociative:
        while (seekValueStack(stack) != OperatorLeftAssociative) {
            int parsedValue;
            popFromStack(stack, &parsedValue);
            pushToStack(&expression, parsedValue);
            
        }
        useRule();
        
        
        break;
    case OperatorEqualAssociative:
        /* code */
        break;
    case OperatorError:
        /* code */
        break;
    }
    return SUCCESS;
}


int checkIfValidToken(Token *token, Stack *stack, int *operator) {
    int returnCode;
    acceptAny();
    switch (token->type) {
        case TokenAdd:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            *operator = OperatorAdd;
            break;
        case TokenSubtract:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            break;
            *operator = OperatorSubtract;
        case TokenMultiply:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            break;
            *operator = OperatorMultiply;
        case TokenDivide:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            *operator = OperatorDivide;
            break;
        case TokenLeftBracket:
            *operator = OperatorLeftBracket;
            break;
        case TokenRightBracket:
            *operator = OperatorRightBracket;
            break;
        case TokenIsLessThan:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsLessThan;
            break;
        case TokenIsLessEqual:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsGreaterEqual;
            break;
        case TokenIsGreaterThan:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsGreaterThan;
            break;
        case TokenIsGreaterEqual:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsGreaterEqual;
            break;
        case TokenIsEqual:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsEqual;
            break;
        case TokenNotEqual:
            if(isInStackOperator(stack)) 
                return SYNTAX_ERROR;
            *operator = OperatorNotEqual;
            break;
        case TokenIdentifier:
        case TokenWholeNbr:
        case TokenDecimalNbr:
        case TokenStringLiteral:
            if(isInStackExpressionOrIdentifier(stack)) 
                return SYNTAX_ERROR;
            Term* term = malloc(sizeof(term));
            if((returnCode = parseTerm(term)) == SUCCESS){
                parseTerm(term);
                switch (term->type) {
                case TermIntegerLiteral:
                    *operator = OperatorWholeNumber;
                    break;
                case TermFloatLiteral:
                    *operator = OperatorDecimal;
                    break;
                case TermStringLiteral:
                    *operator = OperatorStringLiteral;
                    break;
                case TermVariable:
                    *operator = OperatorIdentifier;
                    break;
                }
            } else {
                free(term);
                return returnCode;
            }
            break;
    default:
        return ANALYSIS_END;
        break;
    }
    evaluateExpression(stack, operator);
    // pushToStack(stack, _PSATable[seekValueStack(stack)][*operator]); // to do if < or > or =? use evaluateExpression
    // pushToStack(stack, *operator);
    return SUCCESS;
}

int parseExpression(Expression* expression) {
    /* Stack init */
    Stack stack;
    initStack(&stack, 1);
    pushToStack(&stack, OperatorEnd);
    int operator;
    while (checkIfValidToken(&curTok, &stack, &operator) == SUCCESS) {
        evaluateExpression(&stack, &operator);
        
        
    }
    // TODO: generateAST(); 


    freeStack(&stack);
    return SUCCESS;
}



