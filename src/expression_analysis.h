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
} _Operators;

typedef enum {
  neco
} _VelkyZobacek;

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
} _Rules;

struct RuledValue;

typedef struct {
  RuledValue first;
} UnaryOperation;

typedef struct {
  RuledValue first;
  RuledValue second;
} BinaryOperation;

typedef struct {
  _Rules type; // operation
  union {
    BinaryOperation binary;
    UnaryOperation unary;
  } value;
} Rule;

typedef struct  {
  enum {
    atom,
    operation
  } type;
  union {
    Term atom;
    Rule operation; 
  } ExpProperties;
} RuledValue; // Ecko

typedef struct {
  enum {
    operator, //Enum
    expression, //Ruled value
    zobacek, // Enum
    endSymb // just value
  } type;
  union {
    _Operators op;
    RuledValue ex; // maybe ukazatel, bude to bordel
    _VelkyZobacek zb;
  } value;
} ExpItem;

typedef struct {
  int64_t used;
  int64_t initializedSize;
  ExpItem *values;
} ExpArray; //Todo: Stack

/**
 * @brief This function retuns Expression to the parser and therfore parser can validate the output type
 * 
 * @param expression 
 * @return int 
 */
int parseExpression(RuledValue* expression, _Operators operator, SymbolVariable *symbol);
/**
 * @brief This function validates if recived token fits into already read expression, probably will be depricated as the rules can verify it more easily
 * Also, It is used to correctly parse the IDs
 * 
 * @param token 
 * @param array 
 * @param operator 
 * @return int 
 */
int checkIfValidToken(Token *token, ExpArray *array, RuledValue *operator);
/**
 * @brief 
 * 
 * @param array 
 * @param operator 
 * @return int 
 */
int evaluateExpression(ExpArray *array, RuledValue *operator);
/**
 * @brief Final state machine which checks for rules and then applies them
 * 
 * @param array 
 * @return int 
 */
int rulesEvaluation(ExpArray *array, Rule *generatedRule);
int initExpArray(ExpArray *array, int64_t initialSize);
int pushToArray(ExpArray *array, RuledValue operator);
int seekValueArrayValue(ExpArray *array, int *operator);
int popFromArray(ExpArray *array, RuledValue *returnValue);
void freeArray(ExpArray *array);
bool isInStackOperator(ExpArray *array);
bool isInStackExpressionOrIdentifier(ExpArray *array);
int evaluateExpression(ExpArray *array, RuledValue *operator);
DataType getDataTypeOfExpression(RuledValue *value);

#endif
