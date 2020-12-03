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

#define PLAIN_ASSIGEMENT OperatorNotScreaming

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

    OperationNegate,
    OperatorNotScreaming,
    OperatorError  /** = > */
} Operator;

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
    OperationVar, /** E->i */
    OperationGth, /** E->E>E */
    OperationGEq, /** E->E>=E */
    OperationLes, /** E->E<E */
    OperationLEq, /** E->E<=E */
    OperationEqu, /** E->E==E */
    OperationNEq, /** E->E!=E */
	OperationUnk,
    OperationErr
} OperationType;

typedef enum {
	OperationStateStart,
	StateBracketExpression,
	OperationStateBracketRight,
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

/**
 * @brief This function retuns Expression to the parser and therfore parser can validate the output type
 * 
 * @param expression 
 * @return int 
 */
int parseExpression(ExpExp* expression, Operator operator, SymbolVariable *symbol);
/**
 * @brief This function validates if recived token fits into already read expression, probably will be depricated as the rules can verify it more easily
 * Also, It is used to correctly parse the IDs
 * 
 * @param token 
 * @param array 
 * @param operator 
 * @return int 
 */
int checkIfValidToken(Token *token, ExpStack *array, ExpItem *operator);
/**
 * @brief 
 * 
 * @param array 
 * @param operator 
 * @return int 
 */
int evaluateExpression(ExpStack *expStack, ExpItem *expItem);
/**
 * @brief Final state machine which checks for rules and then applies them
 * 
 * @param array 
 * @return int 
 */
int rulesEvaluation(ExpStack *expStack, Operation *assignetOperation);
int initExpStack(ExpStack *expStack, int64_t initialSize);
int pushToStack(ExpStack *expStack, ExpItem stackItem);
int seekValueStackValue(ExpStack *expStack, ExpItem *expItem);
int popFromStack(ExpStack *expStack, ExpItem *expItem);
void freeExpStack(ExpStack *expStack);
void printStack(ExpStack *expStack);
bool isInStackOperator(ExpStack *expStack);
bool isInStackExpression(ExpStack *expStack);
DataType getDataTypeOfExpression(ExpExp *value);
int evaluateTypeOfExpressions(Operation *assignedOperation);

#endif
