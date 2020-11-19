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

#ifndef LEX_ANALYSIS_H
#define LEX_ANALYSIS_H
#include <stdbool.h> 

typedef enum {
    OperatorAdd = 0, /** + */
    OperatorSubtract, /** - */
    OperatorMultiple, /** * */
    OperatorDivide, /** / */

    OperatorLessThan, /** < */
    OperatorLessEqual, /** <= */
    OperatorGreaterThan, /** > */
    OperatorGreaterEqual, /** >= */
    OperatorIsEqual, /** == */
    OperatorIsNotEqual, /** != */

    OperatorRightBracket, /** ) */
    OperatorLeftBracket, /** ( */
    OperatorId, /** id */

    OperatorWholeNumeber, /** int -- whole numeber */
    OperatorDecimal, /** float -- decimal number */
    OperatorString, /** string -- text string */

    OperatorLeftAssociative,  /** [ */
    OperatorRightAssociative,  /** ] */
    OperatorExpression, /** Exp */
    OperatorEnd /** $ */
} _Operators;

typedef enum {
    RuleAdd, /** E->E+E */
    RuleSub, /** E->E-E */
    RuleMul, /** E->E*E */
    RuleDiv, /** E->E/E */
    RulePar, /** E->(E) */
    RuleVar, /** E->i */
    RuleBth, /** E->E<E */
    RuleBEq, /** E->E<=E */
    RuleLes, /** E->E>E */
    RuleLEq, /** E->E>=E */
    RuleEqu, /** E->E==E */
    RuleNEq /* E->E!=E */
} _Rules;


typedef enum {
  TypeInt,
  TypeFloat64,
  TypeString,
  TypeBool,
  TypeNotDefined
} _ExpressionType;

typedef union {
  int64_t i;
  double d;
  string s;
  bool b;
} _ExpressionValue;

typedef struct {
    _ExpressionType type;
    _ExpressionValue value;
} Expression;

typedef struct {
  int *values;
  int64_t used;
  int64_t size;
} Stack;

int initStack(Stack *stack, int64_t initialSize);
int addToStack(Stack *stack, int operation);
int getLastValueStack(Stack *stack);
void freeStack(Stack *stack);
int checkIfValidToken(Token *token, Stack *stack);
int parseExpression(Expression* expression);

#endif
