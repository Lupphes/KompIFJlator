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

#define PLAIN_ASSIGEMENT OAssignEql /** Default assignment */

/**
 * @brief Operator enum identifies operators
 * Values from this enum are used in PSATable to find associativity
 * @author Ondřej Sloup
 * 
 */
typedef enum {
    OperatorAdd = 0, 		/** + */
    OperatorSubtract, 		/** - */
    OperatorMultiply, 		/** * */
    OperatorDivide, 		/** / */
    OperatorLeftBracket,	/** ( */
    OperatorRightBracket, 	/** ) */
    OperatorIsLessThan, 	/** < */
    OperatorIsLessEqual, 	/** <= */
    OperatorIsGreaterThan, 	/** > */
    OperatorIsGreaterEqual, /** >= */
    OperatorIsEqual, 		/** == */
    OperatorNotEqual, 		/** != */
    OperatorEnd, 			/** $ */
    OperatorNothing, 		/** Operator nothing */
    OperatorError  			/** Error state */
} Operator;

/**
 * @brief OperatorAssign enum identifies the type of assignment
 * Default assignment is "OAssignEql" (0), which is defined as "PLAIN_ASSIGEMENT"
 * @author Ondřej Sloup
 * 
 */
typedef enum {
  	OAssignEql = 0,	/** = */
  	OAssignAdd,		/** += */
  	OAssignSub,		/** -= */
  	OAssignMul, 	/** *= */
  	OAssignDiv		/** /= */
} OperatorAssign;

/**
 * @brief Associativity enum identifies the associativity
 * @author Ondřej Sloup
 * 
 */
typedef enum {
    AssociativityLeft,  	/** [ || < */
    AssociativityRight,		/** ] || > */
    AssociativityEqual, 	/** = > */
	AssociativityError		/** Error state */
} Associativity;

/**
 * @brief OperationType enum identifies operation (rules) by what the expression has been analyzed
 * @author Ondřej Sloup
 * 
 */
typedef enum {
    OperationAdd, 	/** E->E+E */
    OperationSub, 	/** E->E-E */
    OperationMul, 	/** E->E*E */
    OperationDiv, 	/** E->E/E */
    OperationPar, 	/** E->(E) */
    OperationGth, 	/** E->E>E */
    OperationGEq, 	/** E->E>=E */
    OperationLes, 	/** E->E<E */
    OperationLEq, 	/** E->E<=E */
    OperationEqu, 	/** E->E==E */
    OperationNEq, 	/** E->E!=E */
    OperationUnA, 	/** OP-E->OPE */
    OperationUnS 	/** OP+E->OPE */
} OperationType;

/**
 * @brief OperationState enum is used in final state machine which analyzes and identifies OperationType
 * @author Ondřej Sloup
 * 
 */
typedef enum {
	OperationStateStart,
	StateBracketExpression,
	OperationStateBracketLeft,
	OperationStateExpression,
	OperationStateCompleteExpression
} OperationState;

struct _ExpExp;

/**
 * @brief This structure describes unary operand
 * @author Ondřej Sloup
 * @author Viktor Rucký
 * 
 */
typedef struct {
  	struct _ExpExp *first; /** Definition of expression */
} OperandUnary;

/**
 * @brief This structure describes binary operand
 * @author Ondřej Sloup
 * @author Viktor Rucký
 * 
 */
typedef struct {
  	struct _ExpExp *first; /** Definition of first expression */
  	struct _ExpExp *second; /** Definition of second expression */
} OperandsBinary;

/**
 * @brief This structure describes unary operand
 * @author Ondřej Sloup
 * @author Viktor Rucký
 * 
 */
typedef struct {
		OperationType type; /** Defines operation which was used */
	union { /** Stored values for operation */
	  	OperandsBinary binary; /** Store value If OperationType is binary */
	  	OperandUnary unary; /** Store value If OperationType is unary */
	} value; 
} Operation;

/**
 * @brief This 
 * @author Ondřej Sloup
 * @author Viktor Rucký
 * 
 */
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
} ExpExp;

/**
 * @brief 
 * 
 */
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

/**
 * @brief 
 * 
 */
typedef struct {
	int64_t used;
	int64_t initializedSize;
	ExpItem *values;
} ExpStack;

/** ---------------------- Stack Functions ---------------------- **/

/**
 * @brief 
 * 
 * @param expStack 
 * @param initialSize 
 * @return int 
 */
int initExpStack(ExpStack *expStack, int64_t initialSize);
/**
 * @brief 
 * 
 * @param expStack 
 * @param stackItem 
 * @return int 
 */
int pushToStack(ExpStack *expStack, ExpItem stackItem);
/**
 * @brief 
 * 
 * @param expStack 
 * @param expItem 
 * @return int 
 */
int pushToStackBehindE(ExpStack *expStack, ExpItem expItem);
/**
 * @brief 
 * 
 * @param expStack 
 * @param expItem 
 * @return int 
 */
int seekValueBehindE(ExpStack *expStack, ExpItem *expItem);
/**
 * @brief 
 * 
 * @param expStack 
 * @return true 
 * @return false 
 */
bool isUnaryOperationInStack(ExpStack *expStack);
/**
 * @brief 
 * 
 * @param expStack 
 * @param expItem 
 * @return int 
 */
int seekValueStackValue(ExpStack *expStack, ExpItem *expItem);
/**
 * @brief 
 * 
 * @param expStack 
 * @param expItem 
 * @return int 
 */
int popFromStack(ExpStack *expStack, ExpItem *expItem);
/**
 * @brief 
 * 
 * @param expStack 
 */
void printStack(ExpStack *expStack);
/**
 * @brief 
 * 
 * @param expStack 
 */
void freeExpStack(ExpStack *expStack);
/**
 * @brief 
 * 
 * @param expExp 
 */
void freeExpExp(ExpExp *expExp);

/** ---------------------- Stack Functions ---------------------- **/

/** ------------------------ Short-Hands ------------------------ **/

/**
 * @brief 
 * 
 * @param expStack 
 * @return true 
 * @return false 
 */
bool isInStackOperator(ExpStack *expStack);
/**
 * @brief 
 * 
 * @param expStack 
 * @return true 
 * @return false 
 */
bool isInStackExpression(ExpStack *expStack);
/**
 * @brief 
 * 
 * @param expStack 
 * @return true 
 * @return false 
 */
bool isBufferEmpty(ExpStack *expStack);
/**
 * @brief Get the Data Type Of Expression object
 * 
 * @param expExp 
 * @return DataType 
 */
DataType getDataTypeOfExpression(ExpExp *expExp);
/**
 * @brief 
 * 
 * @param operation 
 * @return true 
 * @return false 
 */
bool isBinaryOperation(OperationType operation);
/** ------------------------ Short-Hands ------------------------ **/

/**
 * @brief 
 * 
 * @param newExpExp 
 * @return int 
 */
int evaluateTypeOfExpressions(ExpExp *newExpExp);
/**
 * @brief 
 * 
 * @param expStack 
 * @param newExpExp 
 * @return int 
 */
int rulesEvaluation(ExpStack *expStack, ExpExp *newExpExp);
/**
 * @brief 
 * 
 * @param expStack 
 * @param expItem 
 * @return int 
 */
int evaluateExpression(ExpStack *expStack, ExpItem *expItem);
/**
 * @brief 
 * 
 * @param token 
 * @param expStack 
 * @param expItem 
 * @return int 
 */
int checkIfValidToken(Token *token, ExpStack *expStack, ExpItem *expItem);
/**
 * @brief 
 * 
 * @param expStack 
 * @param assingmentOperation 
 * @param symbol 
 * @return int 
 */
int addOperatorAssignToStack(ExpStack *expStack, OperatorAssign assingmentOperation, const SymbolVariable *symbol);
/**
 * @brief 
 * 
 * @param expression 
 * @param assingmentOperation 
 * @param symbol 
 * @return int 
 */
int parseExpression(ExpExp** expression, OperatorAssign assingmentOperation, const SymbolVariable *symbol);

#endif
