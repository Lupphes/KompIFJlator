/*File name: expression_analysis.c -----------------------------------*
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
#include "parser_common.h"
#include "operator_table.h"
#include "str.h"

#define ANALYSIS_END -1
ExpItem endStartOperator = {.type = ExpItemEnd, .value = {OperatorEnd}};
ExpExp nothingOperator = {.type = ExpExpOperation, .ExpProperties = {{OperatorNothing}}};

/** ---------------------- Stack Functions ---------------------- **/

int initExpStack(ExpStack *expStack, int64_t initialSize) {
    expStack->used = 0;
    expStack->values = malloc(initialSize * sizeof(ExpItem));
    if (expStack->values == NULL)
        return INTERNAL_ERROR;
    expStack->initializedSize = initialSize;
    return SUCCESS;
}

int pushToStack(ExpStack *expStack, ExpItem stackItem) {
    if (expStack->used == expStack->initializedSize) {
        expStack->initializedSize += 1;
        expStack->values = realloc(expStack->values, expStack->initializedSize * sizeof(ExpItem));
        if (expStack->values == NULL)
            return INTERNAL_ERROR;
    }
    expStack->values[expStack->used].type = stackItem.type;
    expStack->values[expStack->used].value = stackItem.value;
    expStack->used += 1;
    return SUCCESS;
}

int pushToStackBehindE(ExpStack *expStack, ExpItem expItem) {
    if (expStack->used == expStack->initializedSize) {
        expStack->initializedSize += 1;
        expStack->values = realloc(expStack->values, expStack->initializedSize * sizeof(ExpItem));
        if (expStack->values == NULL)
            return INTERNAL_ERROR;
    }
    if (expStack->values[expStack->used - 1].type == ExpItemExpression) {
        for (int i = expStack->used - 1; i >= 0; i--) {     
            if (expStack->values[i].type == ExpItemExpression) {
                for (int j = i; j < expStack->used; j++) {
                    expStack->values[j+1].type = expStack->values[j].type;
                    expStack->values[j+1].value = expStack->values[j].value;
                }
                expStack->values[expStack->used-1] = expItem;
                expStack->used += 1;
                return SUCCESS;
            }
        }
    }
    expStack->values[expStack->used] = expItem;
    expStack->used += 1;
    return SUCCESS;
}

int seekValueBehindE(ExpStack *expStack, ExpItem *expItem) {
    for (int i = expStack->used - 1; i >= 0; i--) {
        if (!(expStack->values[i].type == ExpItemExpression || expStack->values[i].type == ExpItemAssociativity)) {
            *expItem = expStack->values[i];
            break;
        }  
    }
    return SUCCESS;
}

bool isUnaryOperationInStack(ExpStack *expStack) {
    for (int i = expStack->used - 1; i >= 0; i--) {
        if (expStack->values[i].type == ExpItemExpression) {
            if (i-1 >= 0) {
                if (expStack->values[i-1].type == ExpItemOperator &&
                (expStack->values[i-1].value.op == OperatorAdd || expStack->values[i-1].value.op == OperatorSubtract)) {
                    if (i-2 >= 0 && expStack->values[i-2].type == ExpItemAssociativity && expStack->values[i-2].value.as == AssociativityLeft) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int seekValueStackValue(ExpStack *expStack, ExpItem *expItem) {
    if (expStack->used != 0) {
        *expItem = expStack->values[expStack->used - 1];
        return SUCCESS;
    }    
    return INTERNAL_ERROR; 
}

int popFromStack(ExpStack *expStack, ExpItem *expItem) {
    if (expStack->values == NULL)
        return INTERNAL_ERROR;
    if (seekValueStackValue(expStack, expItem) == INTERNAL_ERROR)
        return INTERNAL_ERROR; 
    expStack->initializedSize -= 1;
    expStack->used -= 1;
    expStack->values = realloc(expStack->values, expStack->initializedSize * sizeof(ExpItem));
    return SUCCESS;
}

void printStack(ExpStack *expStack) {
    for (int i = 0; i < expStack->used; i++) {
        switch (expStack->values[i].type) {
        case ExpItemOperator:
            printf("%s,", enumOperatorTranslate[expStack->values[i].value.op]);
            break;
        case ExpItemExpression:
            switch (expStack->values[i].value.ee.type) {
            case ExpExpAtom:
                printf("%s,", enumAtomTermTranslate[expStack->values[i].value.ee.ExpProperties.atom.type]);
                break;
            case ExpExpOperation:
                printf("E,");
                break;
            }
            break;
        case ExpItemAssociativity:
            printf("%s,", enumAssociativityTranslate[expStack->values[i].value.as]);
            break;
        case ExpItemEnd:
            printf("$,");
            break;
        }
    }
    printf("\n");
}

void freeExpStack(ExpStack *expStack) {
    for (int i = 0; i < expStack->used; i++) {
        if (expStack->values[i].type == ExpItemExpression) {
            freeExpExp(&expStack->values[i].value.ee);
        }
    }
    free(expStack->values);
    expStack->values = NULL;
    expStack->used = 0;
    expStack->initializedSize = 0;
}

void freeExpExp(ExpExp *expExp) {
    switch(expExp->type){
        case ExpExpAtom:
            freeTerm(&expExp->ExpProperties.atom);
            break;
        case ExpExpOperation:
            if(expExp->ExpProperties.operation.type == OperationPar ||
            expExp->ExpProperties.operation.type == OperationUnA ||
            expExp->ExpProperties.operation.type == OperationUnS) { //TODO: add checks for other unary operations
                freeExpExp(expExp->ExpProperties.operation.value.unary.first);
                free(expExp->ExpProperties.operation.value.unary.first);
            } else {
                freeExpExp(expExp->ExpProperties.operation.value.binary.first);
                freeExpExp(expExp->ExpProperties.operation.value.binary.second);
                free(expExp->ExpProperties.operation.value.binary.first);
                free(expExp->ExpProperties.operation.value.binary.second);
            }
            break;
    }
}

/** ---------------------- Stack Functions ---------------------- **/

/** ------------------------ Short-Hands ------------------------ **/

bool isInStackOperator(ExpStack *expStack) {
    ExpItem lastStackItem;
    if (seekValueStackValue(expStack, &lastStackItem) == INTERNAL_ERROR)
            return INTERNAL_ERROR; 
    if (lastStackItem.type == ExpItemOperator)
        return true;
    return false;
}

bool isInStackExpression(ExpStack *expStack) {
    ExpItem lastStackItem;
    if (seekValueStackValue(expStack, &lastStackItem) == INTERNAL_ERROR)
            return INTERNAL_ERROR;
    if (lastStackItem.type == ExpItemExpression)
        return true;
    return false;
}

bool isBufferEmpty(ExpStack *expStack) {
    return expStack->used == 0 ? true : false;    
}

DataType getDataTypeOfExpression(ExpExp *expExp) {
    return expExp->dataType;
}

bool isBinaryOperation(OperationType operation) {
    switch (operation) {
    case OperationPar:
    case OperationUnA:
    case OperationUnS:
        return false;
        break;
    default:
        return true;
        break;
    }
}

/** ------------------------ Short-Hands ------------------------ **/

int evaluateTypeOfExpressions(ExpExp *newExpExp) {
    switch (newExpExp->ExpProperties.operation.type) {
        case OperationDiv:
        case OperationSub:
        case OperationMul:
            if (newExpExp->ExpProperties.operation.value.binary.first->dataType == TypeString || newExpExp->ExpProperties.operation.value.binary.second->dataType == TypeString) {
                return SEMANTIC_ERROR_TYPE_EXPRESSION;
            }
        /* FALLTHRU */
        case OperationGth:
        case OperationGEq:
        case OperationLes:
        case OperationLEq:
        case OperationEqu:
        case OperationNEq:
        case OperationAdd:
            if (newExpExp->ExpProperties.operation.value.binary.first->dataType != newExpExp->ExpProperties.operation.value.binary.second->dataType){
                return SEMANTIC_ERROR_TYPE_EXPRESSION;
            } 
        break;
        case OperationUnA:
        case OperationUnS:
            if (newExpExp->ExpProperties.operation.value.unary.first->dataType == TypeString) {
                return SEMANTIC_ERROR_TYPE_EXPRESSION;
            }
        break;
        case OperationPar:
            // default case, no checks
        break;
    }
    return SUCCESS;
}

int rulesEvaluation(ExpStack *expStack, ExpExp *newExpExp) {
    ExpItem seekExpItem;
    ExpItem previousExpItem;
    ExpExp *tmp;
    OperationState state = OperationStateStart;
    bool evaluationFinished = true;
    while (evaluationFinished) {
        if (popFromStack(expStack, &previousExpItem) == INTERNAL_ERROR)
            return INTERNAL_ERROR;
        switch (state) {
            case OperationStateStart:
                switch (previousExpItem.type) {
                    case ExpItemExpression:
                        if ((tmp = malloc(sizeof(ExpExp))) == NULL)
                            return INTERNAL_ERROR;
                        *tmp = previousExpItem.value.ee;
                        state = OperationStateExpression;
                        break;
                    case ExpItemOperator:
                        switch (previousExpItem.value.op) {
                            case OperatorRightBracket:
                                state = StateBracketExpression;
                                break;                        
                            default:
                                return SYNTAX_ERROR;
                                break;
                        }
                        break;
                    default:
                        return SYNTAX_ERROR;
                        break;
                }
                break;
            case StateBracketExpression:
                if (previousExpItem.type == ExpItemExpression) {
                    if ((tmp = malloc(sizeof(ExpExp))) == NULL)
                        return INTERNAL_ERROR;
                    *tmp = previousExpItem.value.ee;
                    newExpExp->type = ExpExpOperation;
                    newExpExp->ExpProperties.operation.type = OperationPar;
                    newExpExp->ExpProperties.operation.value.unary.first = tmp;
                    newExpExp->dataType = newExpExp->ExpProperties.operation.value.binary.first->dataType;
                    state = OperationStateBracketLeft;
                }
                break;
            case OperationStateBracketLeft:
                if (previousExpItem.type == ExpItemOperator) {
                    if (previousExpItem.value.op == OperatorLeftBracket) {
                        if (evaluateTypeOfExpressions(newExpExp) == SEMANTIC_ERROR_TYPE_EXPRESSION) {
                            return SEMANTIC_ERROR_TYPE_EXPRESSION;
                        }
                        evaluationFinished = false;
                    }
                }
                break;
            case OperationStateExpression:
                switch (previousExpItem.type) {
                    case ExpItemOperator:
                        switch (previousExpItem.value.op) {
                        case OperatorAdd:
                            if (seekValueStackValue(expStack, &seekExpItem) == INTERNAL_ERROR) {
                                freeExpExp(newExpExp);
                                return INTERNAL_ERROR;
                            }
                            if (seekExpItem.type == ExpItemAssociativity) {
                                newExpExp->type = ExpExpOperation;
                                newExpExp->ExpProperties.operation.value.unary.first = tmp;
                                newExpExp->ExpProperties.operation.type = OperationUnA;
                                newExpExp->dataType = newExpExp->ExpProperties.operation.value.unary.first->dataType;
                                if (evaluateTypeOfExpressions(newExpExp) == SEMANTIC_ERROR_TYPE_EXPRESSION) {
                                    freeExpExp(newExpExp);
                                    return SEMANTIC_ERROR_TYPE_EXPRESSION;
                                }
                                evaluationFinished = false;
                                break;
                            }
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationAdd;
                            break;
                        case OperatorSubtract:
                            if (seekValueStackValue(expStack, &seekExpItem) == INTERNAL_ERROR) {
                                freeExpExp(newExpExp);
                                return INTERNAL_ERROR;
                            }
                            if (seekExpItem.type == ExpItemAssociativity) {
                                newExpExp->type = ExpExpOperation;
                                newExpExp->ExpProperties.operation.value.unary.first = tmp;
                                newExpExp->dataType = newExpExp->ExpProperties.operation.value.unary.first->dataType;
                                newExpExp->ExpProperties.operation.type = OperationUnS;
                                if (evaluateTypeOfExpressions(newExpExp) == SEMANTIC_ERROR_TYPE_EXPRESSION) {
                                    freeExpExp(newExpExp);
                                    return SEMANTIC_ERROR_TYPE_EXPRESSION;
                                }
                                evaluationFinished = false;
                                break;
                            }
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationSub;
                            break;
                        case OperatorMultiply:
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationMul;
                            break;
                        case OperatorDivide:
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            switch (newExpExp->ExpProperties.operation.value.binary.second->type) {
                                case ExpExpAtom:
                                    switch (newExpExp->ExpProperties.operation.value.binary.second->ExpProperties.atom.type) {
                                    case TermIntegerLiteral:
                                        if (newExpExp->ExpProperties.operation.value.binary.second->ExpProperties.atom.value.i == 0) {
                                            free(newExpExp->ExpProperties.operation.value.binary.second);
                                            return SEMANTIC_ERROR_DIV_ZERO;
                                        }
                                        break;
                                    case TermFloatLiteral:
                                        if (newExpExp->ExpProperties.operation.value.binary.second->ExpProperties.atom.value.d == 0.0) {
                                            free(newExpExp->ExpProperties.operation.value.binary.second);
                                            return SEMANTIC_ERROR_DIV_ZERO;
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    break;
                                case ExpExpOperation:
                                    // TODO: Better handling of zeros
                                    break;
                            }
                            newExpExp->ExpProperties.operation.type = OperationDiv;
                            break;
                        case OperatorIsLessThan:
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationLes;
                            break;
                        case OperatorIsLessEqual:
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationLEq;
                            break;
                        case OperatorIsGreaterThan:
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationGth;
                            break;
                        case OperatorIsGreaterEqual:
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationGEq;
                            break;
                        case OperatorIsEqual:
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationEqu;
                            break;
                        case OperatorNotEqual:
                            newExpExp->ExpProperties.operation.value.binary.second = tmp;
                            newExpExp->ExpProperties.operation.type = OperationNEq;
                            break;
                        default:
                            break;
                        }
                        state = OperationStateCompleteExpression;
                        break;
                    default:
                        return SYNTAX_ERROR;
                        break;
                    }
                break;
            case OperationStateCompleteExpression:
                if (previousExpItem.type == ExpItemExpression) {
                    if ((tmp = malloc(sizeof(ExpExp))) == NULL)
                        return INTERNAL_ERROR;
                    *tmp = previousExpItem.value.ee;
                    newExpExp->type = ExpExpOperation;
                    newExpExp->ExpProperties.operation.value.binary.first = tmp;
                    newExpExp->dataType = newExpExp->ExpProperties.operation.type >= OperationGth ? TypeBool : newExpExp->ExpProperties.operation.value.binary.first->dataType; //Enum is magic is happening.
                    if (evaluateTypeOfExpressions(newExpExp) == SEMANTIC_ERROR_TYPE_EXPRESSION) {
                        freeExpExp(newExpExp);
                        return SEMANTIC_ERROR_TYPE_EXPRESSION;
                    }
                    evaluationFinished = false;
                }
                break;
        }
    }
    ExpItem lastExpItem;
    if (seekValueStackValue(expStack, &lastExpItem) == INTERNAL_ERROR) {
        return INTERNAL_ERROR;
    }
    if (lastExpItem.type == ExpItemAssociativity) {
        if (lastExpItem.value.as == AssociativityLeft) {
            if (popFromStack(expStack, &previousExpItem) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            return SUCCESS;
        }
    }
    return SYNTAX_ERROR;
}

int evaluateExpression(ExpStack *expStack, ExpItem *expItem) {
    int returnCode;
    ExpItem previousExpItem;
    ExpItem newExpression;

    seekValueBehindE(expStack, &previousExpItem);
    /* Handle for unary operation */
    if (isUnaryOperationInStack(expStack)) {
        ExpItem newUnaryOperation = {.type = ExpItemExpression};
        if ((returnCode = rulesEvaluation(expStack, &newUnaryOperation.value.ee)) != SUCCESS) {
            return returnCode;
        }
        if (pushToStack(expStack, newUnaryOperation) == INTERNAL_ERROR)
            return INTERNAL_ERROR;
        seekValueBehindE(expStack, &previousExpItem);

        if (expItem->type == ExpItemEnd) {
            if (expStack->used > 2) {
               if ((returnCode = evaluateExpression(expStack, &endStartOperator)) != SUCCESS)
                    return returnCode;
            }
            return SUCCESS;
        }
    }
    switch (PSATable[previousExpItem.value.op][expItem->value.op]) { // Find value from the table
        case AssociativityLeft: // Waiting for evaluation
            newExpression.type = ExpItemAssociativity;
            newExpression.value.as = PSATable[previousExpItem.value.op][expItem->value.op];
            if (pushToStackBehindE(expStack, newExpression) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            if (pushToStack(expStack, *expItem) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            break;
        case AssociativityRight: // The operation needs to be evaluated
            /* Handle for unary operation */
            if (expItem->value.op == OperatorAdd || expItem->value.op == OperatorSubtract) {
                newExpression.type = ExpItemAssociativity;
                newExpression.value.as = AssociativityLeft;
                if (pushToStackBehindE(expStack, newExpression) == INTERNAL_ERROR)
                    return INTERNAL_ERROR;
                if (pushToStack(expStack, *expItem) == INTERNAL_ERROR)
                    return INTERNAL_ERROR;
                break;
            }
            newExpression.type = ExpItemExpression;
            if ((returnCode = rulesEvaluation(expStack, &newExpression.value.ee)) != SUCCESS)
                return returnCode; 
            if (pushToStack(expStack, newExpression) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            if ((returnCode = evaluateExpression(expStack, expItem)) != SUCCESS) 
                return returnCode;
            break;
        case AssociativityEqual: // Handle for brackets
            newExpression.type = ExpItemExpression;
            if (pushToStack(expStack, *expItem) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            if ((returnCode = rulesEvaluation(expStack, &newExpression.value.ee)) != SUCCESS)
                return returnCode;
            if (pushToStack(expStack, newExpression) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            break;
        case AssociativityError:
            if (previousExpItem.value.op == OperatorEnd && expItem->value.op == OperatorEnd) { // Recursion check
                return SUCCESS;
            }
            return SYNTAX_ERROR;
    }
    return SUCCESS;
}

int checkIfValidToken(Token *token, ExpStack *expStack, ExpItem *expItem) {
    int returnCode;   
    switch (token->type) {
        case TokenAdd:
            expItem->type = ExpItemOperator;    
            expItem->value.op = OperatorAdd;
            break;
        case TokenSubtract:
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorSubtract;
            break;
        case TokenMultiply:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorMultiply;
            break;
        case TokenDivide:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorDivide;
            break;
        case TokenLeftBracket:
            if(isInStackExpression(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorLeftBracket;
            break;
        case TokenRightBracket:
            if(isInStackOperator(expStack))
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorRightBracket;
            break;
        case TokenIsLessThan:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorIsLessThan;
            break;
        case TokenIsLessEqual:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorIsGreaterEqual;
            break;
        case TokenIsGreaterThan:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorIsGreaterThan;
            break;
        case TokenIsGreaterEqual:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorIsGreaterEqual;
            break;
        case TokenIsEqual:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorIsEqual;
            break;
        case TokenNotEqual:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemOperator;
            expItem->value.op = OperatorNotEqual;
            break;
        case TokenIdentifier:
        case TokenWholeNbr:
        case TokenDecimalNbr:
        case TokenStringLiteral:
            if(isInStackExpression(expStack)) 
                return SYNTAX_ERROR;
            expItem->type = ExpItemExpression;
            Term term;
            if ((returnCode = parseTerm(&term, false)) == SUCCESS) {
                expItem->value.ee.dataType = termType(&term);
                expItem->value.ee.type = ExpExpAtom;
                expItem->value.ee.ExpProperties.atom = term;
                if (pushToStack(expStack, *expItem) == INTERNAL_ERROR)
                    return INTERNAL_ERROR;
            } else {
                return returnCode;
            }
            break;
    default:
        return ANALYSIS_END;
        break;
    }
    acceptAny(); // Call next token
    return SUCCESS;
}

int addOperatorAssignToStack(ExpStack *expStack, OperatorAssign assingmentOperation, const SymbolVariable *symbol) {
    int returnCode;
    ExpItem tmpPopedExpItem;
    if (popFromStack(expStack, &tmpPopedExpItem) == INTERNAL_ERROR) // Remove value from stack
        return INTERNAL_ERROR;
    ExpItem newExpItem = { // Create expItem variable to push on stack
        .type = ExpItemExpression,
        .value.ee.type = ExpExpAtom,
        .value.ee.dataType = symbol->type,
        .value.ee.ExpProperties.atom.type = TermVariable,
        .value.ee.ExpProperties.atom.value.v = symbol
        };
    if (pushToStack(expStack, newExpItem) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
    
    ExpItem operatorExpItem = {.type = ExpItemOperator};
    switch (assingmentOperation) {
        case OAssignAdd:
            operatorExpItem.value.op = OperatorAdd;
            break;
        case OAssignSub:
            operatorExpItem.value.op = OperationSub;
            break;
        case OAssignMul:
            operatorExpItem.value.op = OperationMul;
            break;
        case OAssignDiv:
            operatorExpItem.value.op = OperationDiv;
            break;
        default:
            return INTERNAL_ERROR;
            break;
    }
    if ((returnCode = evaluateExpression(expStack, &operatorExpItem)) != SUCCESS) // Evaluate with operation
        return returnCode;
    if (pushToStack(expStack, tmpPopedExpItem) == INTERNAL_ERROR) // Push poped item
        return INTERNAL_ERROR;
    if ((returnCode = evaluateExpression(expStack, &endStartOperator)) != SUCCESS) // Evaluate the process
        return returnCode;
    return SUCCESS;
}

int parseExpression(ExpExp** expression, OperatorAssign assingmentOperation, const SymbolVariable *symbol) {
    /* Stack initialization */
    ExpStack expStack;
    int returnCode;
    if (initExpStack(&expStack, 0) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
    if (pushToStack(&expStack, endStartOperator) == INTERNAL_ERROR) {
        freeExpStack(&expStack);
        return INTERNAL_ERROR;
    }

    /* Main evaluating loop */
    ExpItem expItem;
    int parseStatus;
    do {
        parseStatus = checkIfValidToken(&curTok, &expStack, &expItem);
        if (parseStatus != SUCCESS) {
            break;
        } else if (expItem.type != ExpItemExpression) {
            if ((returnCode = evaluateExpression(&expStack, &expItem)) != SUCCESS) {
                freeExpStack(&expStack);
                return returnCode;
            }
            continue;
        }
    } while (parseStatus == SUCCESS);

    /* Error handling */
    if (parseStatus != ANALYSIS_END) {
        freeExpStack(&expStack);
        return parseStatus;
    }

    /* Validate the Expression until is only E*/
    if (seekValueStackValue(&expStack, &expItem) == INTERNAL_ERROR) { // Update
        freeExpStack(&expStack);
        return INTERNAL_ERROR; 
    }
    if (expItem.type == ExpItemEnd && expStack.used == 1) { // Nothing evaluated
        *expression = NULL;
        freeExpStack(&expStack);
        return NO_EXPRESSION;
    } else if (expItem.type != ExpItemEnd && expStack.used > 2) { // Needs to be processed one more time
        if ((returnCode = evaluateExpression(&expStack, &endStartOperator)) != SUCCESS) {
            freeExpStack(&expStack);
            return returnCode;
        }
        if (seekValueStackValue(&expStack, &expItem) == INTERNAL_ERROR) { // Update
            freeExpStack(&expStack);
            return INTERNAL_ERROR; 
        }
    }

    /* Validate the output*/
    if (expItem.type == ExpItemExpression) {
        if (expStack.used != 2) {
            freeExpStack(&expStack);
            return SYNTAX_ERROR;
        }
        if (assingmentOperation != OAssignEql) { // Assign variable before =
            if ((returnCode = addOperatorAssignToStack(&expStack, assingmentOperation, symbol)) != SUCCESS) {
                freeExpStack(&expStack);
                return returnCode;
            }
            if (seekValueStackValue(&expStack, &expItem) == INTERNAL_ERROR) { // Update
                freeExpStack(&expStack);
                return INTERNAL_ERROR; 
            }
        }
        ExpExp* tmp;
        if ((tmp = malloc(sizeof(ExpExp))) == NULL)
            return INTERNAL_ERROR;
        *tmp = expItem.value.ee;
        *expression = tmp;
    } else {
        freeExpStack(&expStack);
        return SYNTAX_ERROR;
    }
    if (popFromStack(&expStack, &expItem) == INTERNAL_ERROR) // Pop value from stack to keep it allocated
        return INTERNAL_ERROR;
    freeExpStack(&expStack);
    return SUCCESS;
}

