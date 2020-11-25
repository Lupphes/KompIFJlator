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

    OperatorIsLessThan, /** < */
    OperatorIsLessEqual, /** <= */
    OperatorIsGreaterThan, /** > */
    OperatorIsGreaterEqual, /** >= */
    OperatorIsEqual, /** == */
    OperatorNotEqual, /** != */

    OperatorRightBracket, /** ) */
    OperatorLeftBracket, /** ( */
    OperatorIdentifier, /** id */

    OperatorWholeNumber, /** int -- whole numeber */ // EXPATOM
    OperatorDecimal, /** float -- decimal number */
    OperatorStringLiteral, /** string -- text string */

    OperatorLeftAssociative,  /** [ || < */
    OperatorRightAssociative,  /** ] || > */
    OperatorEqualAssociative,  /** = > */
    OperatorError,  /** = > */
    OperatorExpression, /** Exp */
    OperatorEnd /** $ */
} _Operators;

typedef enum {
    RuleAdd, /** E->E+E */
    RuleSub = 34, /** E->E-E */
    RuleMul, /** E->E*E */
    RuleDiv, /** E->E/E */
    RulePar, /** E->(E) */
    RuleVar, /** E->i */
    RuleBth, /** E->E<E */
    RuleBEq, /** E->E<=E */
    RuleLes, /** E->E>E */
    RuleLEq, /** E->E>=E */
    RuleEqu, /** E->E==E */
    RuleNEq /** E->E!=E */
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

typedef struct {
  int *values;
  int64_t used;
  int64_t size;
} Stack;

int initStack(Stack *stack, int64_t initialSize);
int addToStack(Stack *stack, int operation);
int popFromStack(Stack *stack, int *operation);
void freeStack(Stack *stack);
int checkIfValidToken(Token *token, Stack *stack);
int seekValueStack(Stack *stack);
int parseExpression(Expression* expression);

#endif
