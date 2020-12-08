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
                // printf("Test,");
                break;
            case ExpExpOperation:
                // printf("%s%s%s,", enumAtomTermTranslate[expStack->values[i].value.ee.ExpProperties.operation.value.binary.first->ExpProperties.atom.type], enumOperationTranslate[expStack->values[i].value.ee.ExpProperties.operation.type],  enumAtomTermTranslate[expStack->values[i].value.ee.ExpProperties.operation.value.binary.second->ExpProperties.atom.type]);
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


void freeExpExp(ExpExp *expExp) {
    switch(expExp->type){
        case ExpExpAtom:
            freeTerm(&expExp->ExpProperties.atom);
            break;
        case ExpExpOperation:
            if(expExp->ExpProperties.operation.type == OperationPar){ //TODO: add checks for other unary operations
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

int prepareRightBracketForStack(ExpStack *expStack, ExpItem *expItem) {
    if(isInStackOperator(expStack))
        return SYNTAX_ERROR;
    expItem->type = ExpItemOperator;
    expItem->value.op = OperatorRightBracket;
    return SUCCESS;
}

int prepareLeftBracketForStack(ExpStack *expStack, ExpItem *expItem) {
    if(isInStackExpression(expStack)) 
        return SYNTAX_ERROR;
    expItem->type = ExpItemOperator;
    expItem->value.op = OperatorLeftBracket;
    return SUCCESS;
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
        case OperationPar:
            // default case, no checks
        break;
    }
    return SUCCESS;
}


int rulesEvaluation(ExpStack *expStack, ExpExp *newExpExp) {
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
                        newExpExp->ExpProperties.operation.value.binary.second = tmp;
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
                            newExpExp->ExpProperties.operation.type = OperationAdd;
                            break;
                        case OperatorSubtract:
                            newExpExp->ExpProperties.operation.type = OperationSub;
                            break;
                        case OperatorMultiply:
                            newExpExp->ExpProperties.operation.type = OperationMul;
                            break;
                        case OperatorDivide:
                            switch (newExpExp->ExpProperties.operation.value.binary.second->type) {
                                case ExpExpAtom:
                                    switch (newExpExp->ExpProperties.operation.value.binary.second->ExpProperties.atom.type) {
                                    case TermIntegerLiteral:
                                        if (newExpExp->ExpProperties.operation.value.binary.second->ExpProperties.atom.value.i == 0)
                                            return SEMANTIC_ERROR_DIV_ZERO;
                                        break;
                                    case TermFloatLiteral:
                                        if (newExpExp->ExpProperties.operation.value.binary.second->ExpProperties.atom.value.d == 0.0)
                                            return SEMANTIC_ERROR_DIV_ZERO;
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
                            newExpExp->ExpProperties.operation.type = OperationLes;
                            break;
                        case OperatorIsLessEqual:
                            newExpExp->ExpProperties.operation.type = OperationLEq;
                            break;
                        case OperatorIsGreaterThan:
                            newExpExp->ExpProperties.operation.type = OperationGth;
                            break;
                        case OperatorIsGreaterEqual:
                            newExpExp->ExpProperties.operation.type = OperationGEq;
                            break;
                        case OperatorIsEqual:
                            newExpExp->ExpProperties.operation.type = OperationEqu;
                            break;
                        case OperatorNotEqual:
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
    ExpItem previousExpItem;
    ExpItem newExpression;
    int returnCode;
    int foundValue;

    printStack(expStack); // DEBUG
    seekValueBehindE(expStack, &previousExpItem);
    switch (previousExpItem.type) {
    case ExpItemOperator:
        foundValue = previousExpItem.value.op;
        break;
    case ExpItemEnd:
        foundValue = previousExpItem.value.op;
        // Infrmation already there
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
    switch (PSATable[foundValue][expItem->value.op]) {
        case AssociativityLeft:
            newExpression.type = ExpItemAssociativity;
            newExpression.value.as = PSATable[foundValue][expItem->value.op];
            printStack(expStack); // DEBUG
            if (pushToStackBehindE(expStack, newExpression) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            printStack(expStack); // DEBUG
            if (pushToStack(expStack, *expItem) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            printStack(expStack); // DEBUG
            break;
        case AssociativityRight:
            newExpression.type = ExpItemExpression;
            if ((returnCode = rulesEvaluation(expStack, &newExpression.value.ee)) != SUCCESS) {
                return returnCode;
            } 
            if (pushToStack(expStack, newExpression) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            printStack(expStack); // DEBUG
            evaluateExpression(expStack, expItem);
            printStack(expStack); // DEBUG
            break;
        case AssociativityEqual:
            newExpression.type = ExpItemExpression;
            if (pushToStack(expStack, *expItem) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            if ((returnCode = rulesEvaluation(expStack, &newExpression.value.ee)) != SUCCESS) {
                return returnCode;
            }
            if (pushToStack(expStack, newExpression) == INTERNAL_ERROR)
                return INTERNAL_ERROR;
            printStack(expStack); // DEBUG
            break;
        case AssociativityError:
            return SYNTAX_ERROR;
    }
    return SUCCESS;
}

int checkIfValidToken(Token *token, ExpStack *expStack, ExpItem *expItem) {
    int returnCode;   
    switch (token->type) {
        case TokenAdd:
            if(isInStackOperator(expStack)) 
                return SYNTAX_ERROR;
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
            if (prepareLeftBracketForStack(expStack, expItem) == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            break;
        case TokenRightBracket:
            if (prepareRightBracketForStack(expStack, expItem) == SYNTAX_ERROR)
                return SYNTAX_ERROR;
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
                printStack(expStack); // DEBUG
            } else {
                return returnCode;
            }
            break;
    default:
        return ANALYSIS_END;
        break;
    }
    acceptAny();
    return SUCCESS;
}

int addOperatorAssignToStack(ExpStack *expStack, OperatorAssign assingmentOperation, const SymbolVariable *symbol) {
    int returnCode;
    ExpItem tempPop;
    if (popFromStack(expStack, &tempPop) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
    printStack(expStack); // DEBUG
    ExpItem atomExpItem;
    atomExpItem.type = ExpItemExpression;
    atomExpItem.value.ee.type = ExpExpAtom;
    atomExpItem.value.ee.dataType = symbol->type;
    atomExpItem.value.ee.ExpProperties.atom.type = TermVariable;
    atomExpItem.value.ee.ExpProperties.atom.value.v = symbol;
    if (pushToStack(expStack, atomExpItem) == INTERNAL_ERROR) {
        return INTERNAL_ERROR;
    }
    printStack(expStack); // DEBUG
    
    ExpItem operatorExpItem;
    operatorExpItem.type = ExpItemOperator;    
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
    if ((returnCode = evaluateExpression(expStack, &operatorExpItem)) != SUCCESS) {
            return returnCode;
    }
    printStack(expStack); // DEBUG
    if (pushToStack(expStack, tempPop) == INTERNAL_ERROR) {
        return INTERNAL_ERROR;
    }
    printStack(expStack); // DEBUG
    if ((returnCode = evaluateExpression(expStack, &endStartOperator)) != SUCCESS) {
            return returnCode;
    }
    printStack(expStack); // DEBUG
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
    if (parseStatus != ANALYSIS_END) {
        freeExpStack(&expStack);
        return parseStatus;
    }
    /* Validate the Expression until is only E*/
    if (seekValueStackValue(&expStack, &expItem) == INTERNAL_ERROR) {
        freeExpStack(&expStack);
        return INTERNAL_ERROR; 
    }
    if (expItem.type == ExpItemEnd && expStack.used == 1) {
        *expression = NULL;
        freeExpStack(&expStack);
        return NO_EXPRESSION;
    } else if (expItem.type != ExpItemEnd && expStack.used > 2) {
        if ((returnCode = evaluateExpression(&expStack, &endStartOperator)) != SUCCESS) { 
            freeExpStack(&expStack);
            return returnCode;
        }
        if (seekValueStackValue(&expStack, &expItem) == INTERNAL_ERROR) {
            freeExpStack(&expStack);
            return INTERNAL_ERROR; 
        }
    }
    if (expItem.type == ExpItemExpression) {
        if (expStack.used != 2) {
            freeExpStack(&expStack);
            return SYNTAX_ERROR;
        }
        if (assingmentOperation != OAssignEql) {
            if ((returnCode = addOperatorAssignToStack(&expStack, assingmentOperation, symbol)) != SUCCESS) {
                freeExpStack(&expStack);
                return returnCode;
            }
            if (seekValueStackValue(&expStack, &expItem) == INTERNAL_ERROR) {
                freeExpStack(&expStack);
                return INTERNAL_ERROR; 
            }
            printStack(&expStack); // DEBUG
        }
        ExpExp* tmp;
        if ((tmp = malloc(sizeof(ExpExp))) == NULL)
            return INTERNAL_ERROR;
        *tmp = expItem.value.ee;
        *expression = tmp;
    } else {
        freeExpStack(&expStack);
        return INTERNAL_ERROR;
    }
    printStack(&expStack); // DEBUG
    if (popFromStack(&expStack, &expItem) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
    freeExpStack(&expStack);
    return SUCCESS;
}

