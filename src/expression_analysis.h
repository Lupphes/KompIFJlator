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

#define PLAIN_ASSIGEMENT OAssignEql

typedef enum {
    OperatorAdd = 0, /** + */
    OperatorSubtract, /** - */
    OperatorMultiply, /** * */
    OperatorDivide, /** / */

    OperatorLeftBracket, /** ( */
    OperatorRightBracket, /** ) */

    OperatorIsLessThan, /** < */
    OperatorIsLessEqual, /** <= */
    OperatorIsGreaterThan, /** > */
    OperatorIsGreaterEqual, /** >= */
    OperatorIsEqual, /** == */
    OperatorNotEqual, /** != */
    OperatorEnd, /** $ */

    OperatorNothing,
    OperatorError  /** = > */
} Operator;

typedef enum {
  OAssignEql = 0, /** = */
  OAssignAdd, /** += */
  OAssignSub, /** -= */
  OAssignMul, /** *= */
  OAssignDiv /** /= */
} OperatorAssign;

typedef enum {
    AssociativityLeft,  /** [ || < */
    AssociativityRight,  /** ] || > */
    AssociativityEqual,  /** = > */
	AssociativityError
} Associativity;

typedef enum {
	OperatorWholeNumber, /** int -- whole numeber */ // EXPATOM
    OperatorDecimal, /** float -- decimal number */
    OperatorStringLiteral /** string -- text string */
} AtomTerm;

typedef enum {
    OperationAdd, /** E->E+E */
    OperationSub, /** E->E-E */
    OperationMul, /** E->E*E */
    OperationDiv, /** E->E/E */
    OperationPar, /** E->(E) */
    OperationGth, /** E->E>E */
    OperationGEq, /** E->E>=E */
    OperationLes, /** E->E<E */
    OperationLEq, /** E->E<=E */
    OperationEqu, /** E->E==E */
    OperationNEq, /** E->E!=E */
    OperationUnA, /** OP-E->OPE */
    OperationUnS /** OP+E->OPE */
} OperationType;

typedef enum {
	OperationStateStart,
	StateBracketExpression,
	OperationStateBracketLeft,
	OperationStateExpression,
	OperationStateCompleteExpression
} OperationState;

struct _ExpExp;

typedef struct {
  struct _ExpExp *first;
} OperandUnary;

typedef struct {
  struct _ExpExp *first;
  struct _ExpExp *second;
} OperandsBinary;

typedef struct {
  OperationType type;
  union {
    OperandsBinary binary;
    OperandUnary unary;
  } value;
} Operation;

typedef struct _ExpExp {
  DataType dataType;
  enum {
    ExpExpAtom,
    ExpExpOperation
  } type;
  union {
    Term atom;
    Operation operation; 
  } ExpProperties;
} ExpExp; // Ecko

typedef struct {
  enum {
    ExpItemOperator,
    ExpItemExpression,
    ExpItemAssociativity,
    ExpItemEnd
  } type;
  union {
    Operator op;
    ExpExp ee;
    Associativity as;
  } value;
} ExpItem;

typedef struct {
  int64_t used;
  int64_t initializedSize;
  ExpItem *values;
} ExpStack;

/** ---------------------- Stack Functions ---------------------- **/
int initExpStack(ExpStack *expStack, int64_t initialSize);
int pushToStack(ExpStack *expStack, ExpItem stackItem);
int pushToStackBehindE(ExpStack *expStack, ExpItem expItem);
int seekValueBehindE(ExpStack *expStack, ExpItem *expItem);
bool isUnaryOperationInStack(ExpStack *expStack);
int seekValueStackValue(ExpStack *expStack, ExpItem *expItem);
int popFromStack(ExpStack *expStack, ExpItem *expItem);
void printStack(ExpStack *expStack);
void freeExpStack(ExpStack *expStack);
void freeExpExp(ExpExp *expExp);

/** ---------------------- Stack Functions ---------------------- **/

/** ------------------------ Short-Hands ------------------------ **/
bool isInStackOperator(ExpStack *expStack);
bool isInStackExpression(ExpStack *expStack);
bool isBufferEmpty(ExpStack *expStack);
DataType getDataTypeOfExpression(ExpExp *expExp);
bool isBinaryOperation(OperationType operation);
/** ------------------------ Short-Hands ------------------------ **/

int evaluateTypeOfExpressions(ExpExp *newExpExp);
int rulesEvaluation(ExpStack *expStack, ExpExp *newExpExp);
int evaluateExpression(ExpStack *expStack, ExpItem *expItem);
int checkIfValidToken(Token *token, ExpStack *expStack, ExpItem *expItem);
int addOperatorAssignToStack(ExpStack *expStack, OperatorAssign assingmentOperation, const SymbolVariable *symbol);
int parseExpression(ExpExp** expression, OperatorAssign assingmentOperation, const SymbolVariable *symbol);

#endif
