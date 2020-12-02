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
ExpValue endStartOperator = {.value = OperatorEnd };
ExpValue newExpression = {.value = OperatorExpression };


/** ---------------------- Array Functions ---------------------- **/

int initExpArray(ExpArray *array, int64_t initialSize) {
    array->used = 0;
    array->values = malloc(initialSize * sizeof(ExpValue));
    if (array->values == NULL)
        return INTERNAL_ERROR;
    array->initializedSize = initialSize;
    return SUCCESS;
}

int pushToArray(ExpArray *array, ExpValue operator) {
    if (array->used == array->initializedSize) {
        array->initializedSize += 1;
        array->values = realloc(array->values, array->initializedSize * sizeof(ExpValue));
        if (array->values == NULL)
            return INTERNAL_ERROR;
    }
    array->values[array->used].value = operator.value;
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

int popFromArray(ExpArray *array, ExpValue *returnValue) {
    if (array->values == NULL)
        return INTERNAL_ERROR;
    if (seekValueArrayValue(array, &returnValue->value) == INTERNAL_ERROR)
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

int evaluateTypeOfExpressions(Rule *generatedRule) {
    generatedRule->newExp = newExpression;   
    switch (generatedRule->ruleType) {
        case RuleMul:
        // TODO: Divide 0 check
        // intentionally not breaking
        case RuleSub:
        case RuleDiv:
        case RulePar:
        case RuleGth:
        case RuleGEq:
        case RuleLes:
        case RuleLEq:
        case RuleEqu:
        case RuleNEq:
            if (generatedRule->Operation.binary.first.ExpProperties.literal.type != generatedRule->Operation.binary.second.ExpProperties.literal.type) {
                return SEMANTIC_ERROR_TYPE_EXPRESION;
            } else if (generatedRule->Operation.binary.first.ExpProperties.literal.type == TermStringLiteral || generatedRule->Operation.binary.second.ExpProperties.literal.type == TermStringLiteral) {
                return SEMANTIC_ERROR_TYPE_EXPRESION;
            }
        break;
        case RuleAdd:
        if (generatedRule->Operation.binary.first.ExpProperties.literal.type != generatedRule->Operation.binary.second.ExpProperties.literal.type) {
            return SEMANTIC_ERROR_TYPE_EXPRESION;
        } 
        generatedRule->newExp.ExpProperties.literal.type = generatedRule->Operation.binary.first.ExpProperties.literal.type;
        break;
    }
    if (generatedRule->Operation.binary.first.ExpProperties.literal.type == TermTypeBool || generatedRule->Operation.binary.second.ExpProperties.literal.type == TermTypeBool) {
        return SEMANTIC_ERROR_TYPE_EXPRESION;
    }
    return SUCCESS;
}


int rulesEvaluation(ExpArray *array, Rule *generatedRule) {
    // Rule generatedRule
    ExpValue returnedValue;
    popFromArray(array, &returnedValue);
    switch (returnedValue.value) {
        case OperatorRightBracket:
            popFromArray(array, &returnedValue);
            if (returnedValue.value == OperatorExpression) {
                (*generatedRule).Operation.unary.first = returnedValue;
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorLeftBracket) {
                    (*generatedRule).ruleType = RulePar;
                    if (seekValueArrayValue(array, &returnedValue.value) == OperatorSubtract) {
                        popFromArray(array, &returnedValue);
                        (*generatedRule).ruleType = RuleBra;
                    }
                }
            }
            break;
        case OperatorSubtract:
            popFromArray(array, &returnedValue);
            if (returnedValue.value == OperatorSubtract) {
                popFromArray(array, &returnedValue);
                if (returnedValue.value== OperatorExpression) {
                    // rule = RuleNe2;
                }
            } else if (returnedValue.value == OperatorLeftBracket) {
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    popFromArray(array, &returnedValue);
                    if (returnedValue.value == OperatorRightBracket) {
                        // rule = RuleBra;
                    }
                }
            }
            break;
        case OperatorExpression:
            (*generatedRule).Operation.binary.second = returnedValue;
            popFromArray(array, &returnedValue);
            switch (returnedValue.value) {
            case OperatorAdd:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleAdd;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorSubtract:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleSub;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorMultiply:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleMul;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorDivide:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleDiv;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorIsGreaterThan:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleGth;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorIsGreaterEqual:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleGEq;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorIsLessThan:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleLes;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorIsLessEqual:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleLEq;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorIsEqual:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleEqu;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            case OperatorNotEqual:
                popFromArray(array, &returnedValue);
                if (returnedValue.value == OperatorExpression) {
                    (*generatedRule).ruleType = RuleNEq;
                    (*generatedRule).Operation.binary.first = returnedValue;
                }
                break;
            default:
                break;
            }
            break;
        case OperatorIdentifier:
            (*generatedRule).Operation.unary.first = returnedValue;
            (*generatedRule).ruleType = RuleNEq;
            break;
        default:
            break;
    }
    int foundValue;
    if (seekValueArrayValue(array, &foundValue) == INTERNAL_ERROR)
        return INTERNAL_ERROR;
    if(foundValue == OperatorLeftAssociative) {
        popFromArray(array, &returnedValue);
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

int evaluateExpression(ExpArray *array, ExpValue *operator) {
    int getLastValeFromStack;
    Rule generatedRules;
    int returnCode;
    
    printArray(array); // DEBUG
    seekValueBehindE(array, &getLastValeFromStack);
    switch (_PSATable[getLastValeFromStack][operator->value]) {
        case OperatorLeftAssociative:
            pushToArrayBehindEorID(array, _PSATable[getLastValeFromStack][operator->value]);
            pushToArray(array, *operator);

            printArray(array); // DEBUG
            break;
        case OperatorRightAssociative:
            if ((returnCode = rulesEvaluation(array, &generatedRules)) != SUCCESS) {
                return returnCode;
            } 
            evaluateTypeOfExpressions(&generatedRules);
            // TODO: generateAST(); 
            pushToArray(array, generatedRules.newExp);
            printArray(array); // DEBUG
            evaluateExpression(array, operator);
            printArray(array); // DEBUG
            break;
        case OperatorEqualAssociative:
            pushToArray(array, *operator);
            if ((returnCode = rulesEvaluation(array, &generatedRules)) != SUCCESS) {
                return returnCode;
            }
            evaluateTypeOfExpressions(&generatedRules); 
            // TODO: generateAST(); 
            pushToArray(array, generatedRules.newExp);
            printArray(array); // DEBUG
            break;
        case OperatorError:
            return SYNTAX_ERROR;
    }
    return SUCCESS;
}

int checkIfValidToken(Token *token, ExpArray *array, ExpValue *operator) {
    int returnCode;
    acceptAny();
    switch (token->type) {
        case TokenAdd:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorAdd;
            break;
        case TokenSubtract:
            operator->value = OperatorSubtract;
            break;
        case TokenMultiply:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorMultiply;
            break;
        case TokenDivide:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorDivide;
            break;
        case TokenLeftBracket:
            if(isInStackExpressionOrIdentifier(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorLeftBracket;
            break;
        case TokenRightBracket:
            operator->value = OperatorRightBracket;
            break;
        case TokenIsLessThan:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorIsLessThan;
            break;
        case TokenIsLessEqual:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorIsGreaterEqual;
            break;
        case TokenIsGreaterThan:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorIsGreaterThan;
            break;
        case TokenIsGreaterEqual:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorIsGreaterEqual;
            break;
        case TokenIsEqual:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorIsEqual;
            break;
        case TokenNotEqual:
            if(isInStackOperator(array)) 
                return SYNTAX_ERROR;
            operator->value = OperatorNotEqual;
            break;
        case TokenIdentifier:
        case TokenWholeNbr:
        case TokenDecimalNbr:
        case TokenStringLiteral:
            if(isInStackExpressionOrIdentifier(array)) 
                return SYNTAX_ERROR;
            Term term;
            if(parseTerm(&term, false) == SUCCESS){
                if ((&term)->type == TermVariable) {
                    // TODO: Remebember variable from symtable
                }
                operator->value = OperatorIdentifier;
                operator->ExpProperties.literal = term;
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
    /* Stack initialization */
    ExpArray expArray;
    initExpArray(&expArray, 0);
    pushToArray(&expArray, endStartOperator);
    printArray(&expArray); // DEBUG
    ExpValue operator;
    int parseStatus;
    parseStatus = checkIfValidToken(&curTok, &expArray, &operator);
    while (parseStatus == SUCCESS) {
        if (evaluateExpression(&expArray, &operator) == SYNTAX_ERROR) 
            return SYNTAX_ERROR;
        parseStatus = checkIfValidToken(&curTok, &expArray, &operator);
    }
    if (parseStatus != ANALYSIS_END)
        return parseStatus;
    /* Validate the Expression until is only E*/
    if (evaluateExpression(&expArray, &endStartOperator) == SYNTAX_ERROR) 
        return SYNTAX_ERROR;

    printArray(&expArray); // DEBUG
    freeArray(&expArray);
    return SUCCESS;
}
