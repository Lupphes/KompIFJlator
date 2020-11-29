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


/** ---------------------- Array Functions ---------------------- **/

int initExpArray(ExpArray *array, int64_t initialSize) {
    array->used = 0;
    array->values = malloc(initialSize * sizeof(ExpValue));
    if (array->values == NULL)
        return INTERNAL_ERROR;
    array->initializedSize = initialSize;
    return SUCCESS;
}

int pushToArray(ExpArray *array, int operator) {
    if (array->used == array->initializedSize) {
        array->initializedSize += 1;
        array->values = realloc(array->values, array->initializedSize * sizeof(ExpValue));
        if (array->values == NULL)
            return INTERNAL_ERROR;
    }
    array->values[array->used].value = operator;
    array->used += 1;
    return SUCCESS;
}

int pushToArrayBehindEorID(ExpArray *array, int operator) {
    if (array->used == array->initializedSize) {
        array->initializedSize += 1;
        array->values = realloc(array->values, array->initializedSize * sizeof(ExpValue));
        if (array->values == NULL)
            return INTERNAL_ERROR;
    }
    if (array->values[array->used - 1].value == OperatorExpression || array->values[array->used - 1].value == OperatorIdentifier) {
        for (int i = array->used - 1; i >= 0; i--) {      
            if (array->values[i].value == OperatorExpression || array->values[i].value == OperatorIdentifier) {
                for (int j = i; j < array->used; j++) {
                    array->values[j+1].value = array->values[j].value;
                }
                array->values[array->used-1].value = operator;
                array->used += 1;
                return SUCCESS;
            }
        }
    }
    array->values[array->used].value = operator;
    array->used += 1;
    return SUCCESS;
}

int seekValueBehindE(ExpArray *array, int *operator) {
    if (seekValueArrayValue(array, operator) == INTERNAL_ERROR)
        return INTERNAL_ERROR; 
    if (*operator == OperatorExpression) {
        *operator = array->values[array->used - 2].value;
    }
    return SUCCESS;
}

int seekValueArrayValue(ExpArray *array, int *operator) {
    if (array->used != 0) {
        *operator = array->values[array->used - 1].value;
        return SUCCESS;
    }    
    return INTERNAL_ERROR; 
}

int popFromArray(ExpArray *array, int *returnValue) {
    if (array->values == NULL)
        return INTERNAL_ERROR;
    if (seekValueArrayValue(array, returnValue) == INTERNAL_ERROR)
        return INTERNAL_ERROR; 
    array->initializedSize -= 1;
    array->used -= 1;
    array->values = realloc(array->values, array->initializedSize * sizeof(ExpValue));
    return SUCCESS;
}

void freeArray(ExpArray *array) {
    free(array->values);
    array->values = NULL;
    array->used = 0;
    array->initializedSize = 0;
}

void printArray(ExpArray *array) {
    for (int i = 0; i < array->used; i++) {
        printf("%s,", enumOperatorTranslate[array->values[i].value]);
    }
    printf("\n");
}

/** ---------------------- Array Functions ---------------------- **/

bool isInStackOperator(ExpArray *array) {
    int lastValue;
    if (seekValueArrayValue(array, &lastValue) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
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

bool isInStackExpressionOrIdentifier(ExpArray *array) {
    int lastValue;
    if (seekValueArrayValue(array, &lastValue) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
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

bool isBufferEmpty(ExpArray *array) {
    return array->used == 0 ? true : false;    
}

int rulesEvaluation(ExpArray *array) {
    int rule = RuleErr;
    int returnedValue;
    popFromArray(array, &returnedValue);
    switch (returnedValue) {
    case OperatorRightBracket:
        popFromArray(array, &returnedValue);
        if (returnedValue == OperatorExpression) {
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorLeftBracket) {
                rule = RulePar;
                if (seekValueArrayValue(array, &returnedValue) == OperatorSubtract) {
                    popFromArray(array, &returnedValue);
                    rule = RuleBra;
                }
            }
        }
        break;
    case OperatorSubtract:
        popFromArray(array, &returnedValue);
        if (returnedValue == OperatorSubtract) {
            popFromArray(array, &returnedValue);
            if (returnedValue== OperatorExpression)
                rule = RuleNe2;
        } else if (returnedValue == OperatorLeftBracket) {
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression) {
                popFromArray(array, &returnedValue);
                if (returnedValue == OperatorRightBracket)
                    rule = RuleBra;
            }
        }
        break;
    case OperatorExpression:
        popFromArray(array, &returnedValue);
        switch (returnedValue) {
        case OperatorAdd:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleAdd;
            break;
        case OperatorSubtract:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleSub;
            break;
        case OperatorMultiply:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleMul;
            break;
        case OperatorDivide:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleDiv;
            break;
        case OperatorIsGreaterThan:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleGth;
            break;
        case OperatorIsGreaterEqual:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleGEq;
            break;
        case OperatorIsLessThan:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleLes;
            break;
        case OperatorIsLessEqual:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleLEq;
            break;
            popFromArray(array, &returnedValue);
        case OperatorIsEqual:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleEqu;
            break;
        case OperatorNotEqual:
            popFromArray(array, &returnedValue);
            if (returnedValue == OperatorExpression)
                rule = RuleNEq;
        default:
            break;
        }
        break;
    case OperatorIdentifier:
        rule = RuleVar;
        break;
    default:
        break;
    }
    int foundValue;
    if (seekValueArrayValue(array, &foundValue) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
    if(foundValue == OperatorLeftAssociative) {
        popFromArray(array, &returnedValue);
        return rule;
    }
    return RuleErr;
}

int evaluateExpression(ExpArray *array, int *operator) {
    int getLastValeFromStack;
    int identifiedRule;
    printArray(array); // DEBUG
    seekValueBehindE(array, &getLastValeFromStack);
    switch (_PSATable[getLastValeFromStack][*operator]) {
        case OperatorLeftAssociative:
            pushToArrayBehindEorID(array, _PSATable[getLastValeFromStack][*operator]);
            pushToArray(array, *operator);

            printArray(array); // DEBUG
            break;
        case OperatorRightAssociative:
            identifiedRule = rulesEvaluation(array);
            pushToArray(array, OperatorExpression);
            printArray(array); // DEBUG
            evaluateExpression(array, operator);
            printArray(array); // DEBUG
            break;
        case OperatorEqualAssociative:
            pushToArray(array, *operator);
            identifiedRule = rulesEvaluation(array);
            pushToArray(array, OperatorExpression);
            printArray(array); // DEBUG
            break;
    }
    return SUCCESS;
}

int checkIfValidToken(Token *token, ExpArray *array, int *operator) {
    int returnCode;
    acceptAny();
    switch (token->type) {
        case TokenAdd:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            *operator = OperatorAdd;
            break;
        case TokenSubtract:
            *operator = OperatorSubtract;
            break;
        case TokenMultiply:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            *operator = OperatorMultiply;
            break;
        case TokenDivide:
            if(isInStackOperator(array)) 
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
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsLessThan;
            break;
        case TokenIsLessEqual:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsGreaterEqual;
            break;
        case TokenIsGreaterThan:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsGreaterThan;
            break;
        case TokenIsGreaterEqual:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsGreaterEqual;
            break;
        case TokenIsEqual:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            *operator = OperatorIsEqual;
            break;
        case TokenNotEqual:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            *operator = OperatorNotEqual;
            break;
        case TokenIdentifier:
        case TokenWholeNbr:
        case TokenDecimalNbr:
        case TokenStringLiteral:
            if(isInStackExpressionOrIdentifier(array)) 
                return SYNTAX_ERROR;
            Term* term = malloc(sizeof(term));
            if(parseTerm(term, false) == SUCCESS){
                switch (term->type) {
                case TermIntegerLiteral:
                    // *operator = OperatorWholeNumber;
                    break;
                case TermFloatLiteral:
                    // *operator = OperatorDecimal;
                    break;
                case TermStringLiteral:
                    // *operator = OperatorStringLiteral;
                    break;
                case TermVariable:
                    // *operator = OperatorIdentifier;
                    break;
                }
                *operator = OperatorIdentifier;
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

int parseExpression(Expression* expression) {
    /* Stack init */
    ExpArray expArray;
    initExpArray(&expArray, 0);
    pushToArray(&expArray, OperatorEnd);
    printArray(&expArray);

    int operator;
    int parseStatus;
    while (parseStatus != ANALYSIS_END) {
        parseStatus = checkIfValidToken(&curTok, &expArray, &operator);
        evaluateExpression(&expArray, &operator);
    }
    operator = OperatorEnd; 
    evaluateExpression(&expArray, &operator);
    printArray(&expArray); // DEBUG
    // TODO: generateAST(); 

    freeArray(&expArray);
    return SUCCESS;
}
