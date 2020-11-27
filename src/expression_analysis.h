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
} ExpValue;

typedef struct {
  int64_t used;
  int64_t initializedSize;
  ExpValue* values;
} ExpArray;

int parseExpression(Expression* expression);
int initExpArray(ExpArray *array, int64_t initialSize);
int pushToArray(ExpArray *array, int operator);
int seekValueArrayValue(ExpArray *array, int *operator);
int popFromArray(ExpArray *array, int *returnValue);
void freeArray(ExpArray *array);
bool isInStackOperator(ExpArray *array);
int ruleTranslator(ExpArray *array);
bool isInStackExpressionOrIdentifier(ExpArray *array);
int useRule();
int evaluateExpression(ExpArray *array, int *operator);
int checkIfValidToken(Token *token, ExpArray *array, int *operator);

#endif
