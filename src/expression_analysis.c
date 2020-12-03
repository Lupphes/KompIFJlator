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

#define ANALYSIS_END -1
ExpItem endStartOperator = {.type = ExpItemEnd };
ExpItem newExpression = {.type = ExpItemExpression};
ExpItem assocExpItem = {.type = ExpItemAssociativity};


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

int pushToArrayBehindEorID(ExpStack *expStack, ExpItem expItem) {
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
    if (seekValueStackValue(expStack, expItem) == INTERNAL_ERROR)
        return INTERNAL_ERROR; 
    if (expItem->type == ExpItemExpression) {
        *expItem = expStack->values[expStack->used - 2];
    }
    return SUCCESS;
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

void freeExpStack(ExpStack *expStack) {
    free(expStack->values);
    expStack->values = NULL;
    expStack->used = 0;
    expStack->initializedSize = 0;
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
                // printf("%s,", enumOperationTranslate[expStack->values[i].value.ee.ExpProperties.operation.type]);
                printf("—");
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

/** ---------------------- Stack Functions ---------------------- **/

/** ------------------------ Short-Hands ------------------------ **/

bool isInStackOperator(ExpStack *expStack) {
    ExpItem lastStackItem;
    seekValueStackValue(expStack, &lastStackItem);
    if (lastStackItem.type == ExpItemOperator)
        return true;
    return false;
}

bool isInStackExpression(ExpStack *expStack) {
    ExpItem lastStackItem;
    seekValueStackValue(expStack, &lastStackItem);
    if (lastStackItem.type == ExpItemExpression)
        return true;
    return false;
}

bool isBufferEmpty(ExpStack *array) {
    return array->used == 0 ? true : false;    
}

/** ------------------------ Short-Hands ------------------------ **/

int evaluateTypeOfExpressions(Operation *assignedOperation) {
      
    if (termType(&assignedOperation->value.binary.first->ExpProperties.atom) == TypeBool || termType(&assignedOperation->value.binary.second->ExpProperties.atom) == TypeBool) {
        return SEMANTIC_ERROR_TYPE_EXPRESION;
    } 
    switch (assignedOperation->type) {
        case OperationDiv:
        // TODO: Divide 0 check
        // intentionally not breaking
        case OperationSub:
        case OperationMul:
        case OperationGth:
        case OperationGEq:
        case OperationLes:
        case OperationLEq:
        case OperationEqu:
        case OperationNEq:
            if (termType(&assignedOperation->value.binary.first->ExpProperties.atom) == TypeString || termType(&assignedOperation->value.binary.first->ExpProperties.atom) == TypeString) {
                return SEMANTIC_ERROR_TYPE_EXPRESION;
            } 
        // intentionally not breaking
        case OperationAdd:
            if (termType(&assignedOperation->value.binary.first->ExpProperties.atom) != termType(&assignedOperation->value.binary.second->ExpProperties.atom)) {
                return SEMANTIC_ERROR_TYPE_EXPRESION;
            }
        default:
            break;
        break;
    }
    return SUCCESS;
}


int rulesEvaluation(ExpStack *expStack, Operation *assignedOperation) {
    ExpItem previousExpItem;
    OperationState state = OperationStateStart;
    assignedOperation->type = OperationUnk;
    while (assignedOperation->type == OperationUnk) {
        popFromStack(expStack, &previousExpItem);
        switch (state) {
            case OperationStateStart:
                switch (previousExpItem.type) {
                    case ExpItemExpression:
                        assignedOperation->value.binary.second = &previousExpItem.value.ee;
                        state = OperationStateExpression;
                        break;
                    case ExpItemOperator:
                        switch (previousExpItem.value.op) {
                            case OperatorIdentifier:
                                assignedOperation->value.unary.first = &previousExpItem.value.ee;
                                assignedOperation->type = OperationVar;
                                // TODO: Locate values from symtable
                                break;
                            case OperatorLeftBracket:
                                state = StateBracketExpression;
                                break;                        
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case StateBracketExpression:
                if (previousExpItem.type == ExpItemExpression) {
                    assignedOperation->value.unary.first = &previousExpItem.value.ee;
                    state = OperationStateBracketRight;
                }
                break;
            case OperationStateBracketRight:
                if (previousExpItem.type == ExpItemOperator) {
                    if (previousExpItem.value.op == OperatorRightBracket) {
                        assignedOperation->type = OperationPar;
                        //TODO: Return expression bracket
                    }
                }
                break;
            case OperationStateExpression:
                switch (previousExpItem.type) {
                    case ExpItemOperator:
                        switch (previousExpItem.value.op) {
                        case OperatorAdd:
                            assignedOperation->type = OperationAdd;
                            break;
                        case OperatorSubtract:
                            assignedOperation->type = OperationSub;
                            break;
                        case OperatorMultiply:
                            assignedOperation->type = OperationMul;
                            break;
                        case OperatorDivide:
                            assignedOperation->type = OperationDiv;
                            break;
                        case OperatorIsLessThan:
                            assignedOperation->type = OperationLes;
                            break;
                        case OperatorIsLessEqual:
                            assignedOperation->type = OperationLEq;
                            break;
                        case OperatorIsGreaterThan:
                            assignedOperation->type = OperationGth;
                            break;
                        case OperatorIsGreaterEqual:
                            assignedOperation->type = OperationGEq;
                            break;
                        case OperatorIsEqual:
                            assignedOperation->type = OperationEqu;
                            break;
                        case OperatorNotEqual:
                            assignedOperation->type = OperationNEq;
                            break;
                        default:
                            break;
                        }
                        state = OperationStateCompleteExpression;
                        break;
                    default:
                        break;
                    }
                break;
            case OperationStateCompleteExpression:
                if (previousExpItem.type == ExpItemExpression) {
                    assignedOperation->value.binary.first = &previousExpItem.value.ee;
                }
                break;
        }
    }
    ExpItem lastExpItem;
    if (seekValueStackValue(expStack, &lastExpItem) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
    if(lastExpItem.type == ExpItemAssociativity) {
        if (&lastExpItem.value == AssociativityLeft) {
            popFromStack(expStack, &previousExpItem);
            return SUCCESS;
        }
    }
    return SYNTAX_ERROR;
}

int evaluateExpression(ExpStack *expStack, ExpItem *expItem) {
    Operation assignedOperation;
    ExpItem previousExpItem;
    int returnCode;
    int foundValue;

    printStack(expStack); // DEBUG
    seekValueBehindE(expStack, &previousExpItem);
    switch (previousExpItem.type) {
    case ExpItemOperator:
        foundValue = previousExpItem.value.op;
        break;
    case ExpItemEnd:
        foundValue = OperatorEnd;
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
    switch (PSATable[foundValue][expItem->value.op]) {
        case AssociativityLeft:
            assocExpItem.value.as = PSATable[foundValue][expItem->value.op];
            pushToArrayBehindEorID(expStack, assocExpItem);
            pushToStack(expStack, *expItem);

            printStack(expStack); // DEBUG
            break;
        case AssociativityRight:
            if ((returnCode = rulesEvaluation(expStack, &assignedOperation)) != SUCCESS) {
                return returnCode;
            } 
            evaluateTypeOfExpressions(&assignedOperation);
            newExpression.value.ee.type = ExpExpOperation;
            newExpression.value.ee.ExpProperties.operation = assignedOperation;
            pushToStack(expStack, newExpression);
            printStack(expStack); // DEBUG
            evaluateExpression(expStack, expItem);
            printStack(expStack); // DEBUG
            break;
        case AssociativityEqual:
            pushToStack(expStack, *expItem);
            if ((returnCode = rulesEvaluation(expStack, &assignedOperation)) != SUCCESS) {
                return returnCode;
            }
            evaluateTypeOfExpressions(&assignedOperation); 
            newExpression.value.ee.type = ExpExpOperation;
            newExpression.value.ee.ExpProperties.operation = assignedOperation;
            pushToStack(expStack, newExpression);
            printStack(expStack); // DEBUG
            break;
        case AssociativityError:
            return SYNTAX_ERROR;
    }
    return SUCCESS;
}

int checkIfValidToken(Token *token, ExpStack *expStack, ExpItem *expItem) {
    int returnCode;
    acceptAny();
    expItem->type = ExpItemOperator;
    switch (token->type) {
        case TokenAdd:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorAdd;
            break;
        case TokenSubtract:
            expItem->value.op = OperatorSubtract;
            break;
        case TokenMultiply:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorMultiply;
            break;
        case TokenDivide:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorDivide;
            break;
        case TokenLeftBracket:
            if(isInStackExpression(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorLeftBracket;
            break;
        case TokenRightBracket:
            expItem->value.op = OperatorRightBracket;
            break;
        case TokenIsLessThan:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorIsLessThan;
            break;
        case TokenIsLessEqual:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorIsGreaterEqual;
            break;
        case TokenIsGreaterThan:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorIsGreaterThan;
            break;
        case TokenIsGreaterEqual:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorIsGreaterEqual;
            break;
        case TokenIsEqual:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
            expItem->value.op = OperatorIsEqual;
            break;
        case TokenNotEqual:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
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
            if(parseTerm(&term, false) == SUCCESS) {
                expItem->value.ee.type = ExpExpAtom;
                expItem->value.ee.ExpProperties.atom = term;
            } else {
                return returnCode;
            }
            break;
    default:
        return ANALYSIS_END;
        break;
    }
    return SUCCESS;
}

int parseExpression(ExpExp** expression, Operator assingmentOperation, const SymbolVariable *symbol) {
    /* Stack initialization */
    ExpStack expStack;
    initExpStack(&expStack, 0);
    pushToStack(&expStack, endStartOperator);
    printStack(&expStack); // DEBUG
    ExpItem expItem;
    int parseStatus;
    parseStatus = checkIfValidToken(&curTok, &expStack, &expItem);
    while (parseStatus == SUCCESS) {
        if (evaluateExpression(&expStack, &expItem) == SYNTAX_ERROR) 
            return SYNTAX_ERROR;
        parseStatus = checkIfValidToken(&curTok, &expStack, &expItem);
    }
    if (parseStatus != ANALYSIS_END)
        return parseStatus;
    /* Validate the Expression until is only E*/
    if (evaluateExpression(&expStack, &endStartOperator) == SYNTAX_ERROR) 
        return SYNTAX_ERROR;

    printStack(&expStack); // DEBUG
    freeExpStack(&expStack);
    return SUCCESS;
}

