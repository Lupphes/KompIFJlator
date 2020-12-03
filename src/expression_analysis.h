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
    OperatorNotScreaming,
    OperatorError  /** = > */
} Operators;

typedef enum {
  neco
} Associativity;

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
    RuleErr
} OperationType;

struct _ExpExp;

typedef struct {
  struct _ExpExp *first;
} OperandUnary;

typedef struct {
  struct _ExpExp *first;
  struct _ExpExp *second;
} OperandsBinary;

typedef struct {
  OperationType type; // operation
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
    ExpItemOperator, //Enum
    ExpItemExpression, //Ruled value
    ExpItemAssociativity, // Enum
    ExpItemEnd // just value
  } type;
  union {
    Operators op;
    ExpExp ee; // maybe ukazatel, bude to bordel
    Associativity as;
  } value;
} ExpItem;

typedef struct {
  int64_t used;
  int64_t initializedSize;
  ExpItem *values;
} ExpStack; //Todo: Stack

/**
 * @brief This function retuns Expression to the parser and therfore parser can validate the output type
 * 
 * @param expression 
 * @return int 
 */
int parseExpression(ExpExp** expression, Operator assingmentOperation, const SymbolVariable *symbol);
/**
 * @brief This function validates if recived token fits into already read expression, probably will be depricated as the rules can verify it more easily
 * Also, It is used to correctly parse the IDs
 * 
 * @param token 
 * @param array 
 * @param operator 
 * @return int 
 */
int checkIfValidToken(Token *token, ExpStack *array, ExpExp *operator);
/**
 * @brief 
 * 
 * @param array 
 * @param operator 
 * @return int 
 */
int evaluateExpression(ExpStack *array, ExpExp *operator);
/**
 * @brief Final state machine which checks for rules and then applies them
 * 
 * @param array 
 * @return int 
 */
int rulesEvaluation(ExpStack *array, Operation *generatedRule);
int initExpArray(ExpStack *array, int64_t initialSize);
int pushToArray(ExpStack *array, ExpExp operator);
int seekValueArrayValue(ExpStack *array, int *operator);
int popFromArray(ExpStack *array, ExpExp *returnValue);
void freeArray(ExpStack *array);
bool isInStackOperator(ExpStack *array);
bool isInStackExpressionOrIdentifier(ExpStack *array);
int evaluateExpression(ExpStack *array, ExpExp *operator);
DataType getDataTypeOfExpression(ExpExp *value);
void freeExpExp(ExpExp *expExp);

#endif
