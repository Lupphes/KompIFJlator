/*File name: expression_analysis.h -----------------------------------*
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

#ifndef EXPRESSION_ANALYSIS_H
#define EXPRESSION_ANALYSIS_H
#include <stdbool.h> 
#include <stdint.h>
#include <stdlib.h>

#include "str.h"
#include "token.h"
#include "term.h"





typedef enum {
    OperatorAdd = 0, /** + */
    OperatorSubtract, /** - */
    OperatorMultiply, /** * */
    OperatorDivide, /** / */

    OperatorLeftBracket, /** ( */
    OperatorRightBracket, /** ) */
    OperatorIdentifier, /** id */

    OperatorIsLessThan, /** < */
    OperatorIsLessEqual, /** <= */
    OperatorIsGreaterThan, /** > */
    OperatorIsGreaterEqual, /** >= */
    OperatorIsEqual, /** == */
    OperatorNotEqual, /** != */
    OperatorEnd, /** $ */
    
    /* this need redo */
    OperatorWholeNumber = 15, /** int -- whole numeber */ // EXPATOM
    OperatorDecimal, /** float -- decimal number */
    OperatorStringLiteral, /** string -- text string */
    OperationNegate,
    

    OperatorLeftAssociative,  /** [ || < */
    OperatorRightAssociative,  /** ] || > */
    OperatorEqualAssociative,  /** = > */
    OperatorStart, /** ^ */
    OperatorExpression, /** Exp */
    OperatorError  /** = > */
} _Operators;

typedef enum {
    RuleAdd, /** E->E+E */
    RuleSub = 34, /** E->E-E */
    RuleMul, /** E->E*E */
    RuleDiv, /** E->E/E */
    RulePar, /** E->(E) */
    RuleVar, /** E->i */
    RuleGth, /** E->E<E */
    RuleGEq, /** E->E<=E */
    RuleLes, /** E->E>E */
    RuleLEq, /** E->E>=E */
    RuleEqu, /** E->E==E */
    RuleNEq, /** E->E!=E */
    RuleBra, /** E->-(E) */
    RuleNe2, /** E->--E */
    RuleErr
} _Rules;


typedef enum {
  ExpressionTypeInt,
  ExpressionTypeFloat64,
  ExpressionTypeString,
  ExpressionTypeBool,
  ExpressionTypeNotDefined
} _ExpressionType;

typedef struct {
    _ExpressionType type;
} Expression;

typedef struct  {
  int value;
  union {
    Term literal;
    // TODO: Asi tady ještě budu něco potrebovat
  } ExpProperties;
} ExpValue;

typedef struct {
  int64_t used;
  int64_t initializedSize;
  ExpValue *values;
} ExpArray;

typedef struct {
  ExpValue first;
} UnaryOperation;

typedef struct {
  ExpValue first;
  ExpValue second;
} BinaryOperation;

typedef struct {
  int ruleType;
  union {
    BinaryOperation binary;
    UnaryOperation unary;
  } Operation;
  ExpValue newExp;
} Rule;

/**
 * @brief This is debug array and will not be included in final project
 * It is used in print array for normalizing the outputs
 * 
 */
static const char* enumOperatorTranslate[] = {
    "+", /** + */
    "-", /** - */
    "*", /** * */
    "/", /** / */

    "(", /** ( */
    ")", /** ) */
    "id", /** id */

    "<", /** < */
    "<=", /** <= */
    ">", /** > */
    ">=", /** >= */
    "==", /** == */
    "!=", /** != */
    "$", /** $ */
    "lalala",
    /* this need redo */
    "OperatorWholeNumber", /** int -- whole numeber */ // EXPATOM
    "OperatorDecimal", /** float -- decimal number */
    "OperatorStringLiteral", /** string -- text string */
    "OperationNegate",
    

    "<",  /** [ || < */
    ">",  /** ] || > */
    "=",  /** = > */
    "OperatorStart", /** ^ */
    "E", /** Exp */
    "OperatorError"  /** = > */
 };
/**
 * @brief This function retuns Expression to the parser and therfore parser can validate the output type
 * 
 * @param expression 
 * @return int 
 */
int parseExpression(Expression* expression);
/**
 * @brief This function validates if recived token fits into already read expression, probably will be depricated as the rules can verify it more easily
 * Also, It is used to correctly parse the IDs
 * 
 * @param token 
 * @param array 
 * @param operator 
 * @return int 
 */
int checkIfValidToken(Token *token, ExpArray *array, ExpValue *operator);
/**
 * @brief 
 * 
 * @param array 
 * @param operator 
 * @return int 
 */
int evaluateExpression(ExpArray *array, ExpValue *operator);
/**
 * @brief Final state machine which checks for rules and then applies them
 * 
 * @param array 
 * @return int 
 */
int rulesEvaluation(ExpArray *array, Rule *generatedRule);
int initExpArray(ExpArray *array, int64_t initialSize);
int pushToArray(ExpArray *array, ExpValue operator);
int seekValueArrayValue(ExpArray *array, int *operator);
int popFromArray(ExpArray *array, ExpValue *returnValue);
void freeArray(ExpArray *array);
bool isInStackOperator(ExpArray *array);
bool isInStackExpressionOrIdentifier(ExpArray *array);
int evaluateExpression(ExpArray *array, ExpValue *operator);


#endif
