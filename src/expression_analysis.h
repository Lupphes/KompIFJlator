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
 * @brief This structure describes operation and its type. It also includes the the values of atoms/previous operations
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
 * @brief This struct defines expression, its datatype and value
 * @author Ondřej Sloup
 * @author Viktor Rucký
 * 
 */
typedef struct _ExpExp {
  	DataType dataType;  /** Defines data type of expression */
  	enum { /** Identifies type of the expression */
  	  	ExpExpAtom, /** Expression is atom */
  	  	ExpExpOperation /** Expression is operator */
  	} type;
  	union { /** Stores values of the expression */
  	  	Term atom; /** Term values */
  	  	Operation operation; /** Operation values */
  	} ExpProperties;
} ExpExp;

/**
 * @brief Items in ExpStack which describes operations
 * @author Ondřej Sloup
 * @author Viktor Rucký
 * 
 */
typedef struct {
  	enum { /** Defines type of the item */
  	  	ExpItemOperator, /** Item is defined by operator */
  	  	ExpItemExpression, /** Item is defined by expression */
  	  	ExpItemAssociativity, /** Item is defined by associativity */
  	  	ExpItemEnd /** Item is end operator */
  	} type;
  	union { /** Defines value of the item */
  	  	Operator op; /** Operator values */
  	  	ExpExp ee; /** Expression values */
  	  	Associativity as; /** Associativity values */
  	} value;
} ExpItem;

/**
 * @brief Stores ExpItems and holds whole structure
 * @author Ondřej Sloup
 * @author Viktor Rucký
 * 
 */
typedef struct {
	int64_t used; /** Used values in stack */
	int64_t initializedSize; /** Initilized values in stack */
	ExpItem *values; /** Stores ExpItems values */
} ExpStack;

/** ---------------------- Stack Functions ---------------------- **/

/**
 * @brief Initializes ExpStack
 * 
 * @param expStack Stack which will be initialized
 * @param initialSize Initial size to which it should be initialized
 * @return int SUCCESS Stack successfully initialized
 * @return int INTERNAL_ERROR Malloc (memory error)
 * 
 */
int initExpStack(ExpStack *expStack, int64_t initialSize);

/**
 * @brief Pushes value to the stack
 * 
 * @param expStack Stack where the value will be pushed
 * @param stackItem Value to be pushed
 * @return int SUCCESS Value successfully pushed
 * @return int INTERNAL_ERROR Malloc (memory error)
 * 
 */
int pushToStack(ExpStack *expStack, ExpItem stackItem);

/**
 * @brief Pushes value to stack behind. If it encouters expression, it is pushed behind it. Right to left process.
 * 
 * @param expStack Stack where the value will be pushed
 * @param expItem Value to be pushed
 * @return int SUCCESS Value successfully pushed
 * @return int INTERNAL_ERROR Malloc (memory error)
 * 
 */
int pushToStackBehindE(ExpStack *expStack, ExpItem expItem);

/**
 * @brief Looks for first value in stack. If it encouters expression, it returns value behind it. Right to left process.
 * 
 * @param expStack Stack in which the function will look for the value
 * @param expItem Found value
 * @return int SUCCESS Found value
 * 
 */
int seekValueBehindE(ExpStack *expStack, ExpItem *expItem);

/**
 * @brief Looks for specific unary pattern "[,-,E" or "[,+,E".
 * 
 * @param expStack Stack in which the function will look for the value
 * @return true Unary patern found
 * @return false Unary patern not found
 * 
 */
bool isUnaryOperationInStack(ExpStack *expStack);

/**
 * @brief Looks for first value in stack
 * 
 * @param expStack Stack in which the function will look for the value
 * @param expItem Found value
 * @return int SUCCESS Found value
 * @return int INTERNAL_ERROR Empty stack (should not happen)
 * 
 */
int seekValueStackValue(ExpStack *expStack, ExpItem *expItem);

/**
 * @brief Pops value from stack and returns it via expItem
 * 
 * @param expStack Stack in which the function will look for the value. Right to left process.
 * @param expItem Returned poped value
 * @return int SUCCESS Popped successfully
 * @return int INTERNAL_ERROR Error encoutered
 * 
 */
int popFromStack(ExpStack *expStack, ExpItem *expItem);

/**
 * @brief Debug function which is not used in final product, but it was used in tests.
 * 
 * @param expStack Stack to be printed
 */
void printStack(ExpStack *expStack);

/**
 * @brief Frees whole stack and all its allocated values
 * 
 * @param expStack Stack to be freed
 */
void freeExpStack(ExpStack *expStack);

/**
 * @brief Frees expression and all its allocated values
 * 
 * @param expExp ExpExp to be freed
 * 
 * @author Viktor Rucký
 */
void freeExpExp(ExpExp *expExp);

/** ---------------------- Stack Functions ---------------------- **/

/** ------------------------ Short-Hands ------------------------ **/

/**
 * @brief Finds if first value on stack is an operator
 * 
 * @param expStack Stack to be processed
 * @return true First value from right is an operator
 * @return false First value from right is not an operator
 */
bool isInStackOperator(ExpStack *expStack);

/**
 * @brief Finds if first value on stack is an expression
 * 
 * @param expStack Stack to be processed
 * @return true First value from right is an expression
 * @return false First value from right is not an expression
 */
bool isInStackExpression(ExpStack *expStack);

/**
 * @brief Finds if buffer is empty
 * 
 * @param expStack Stack to be processed
 * @return true Stack is empty
 * @return false Stack is not empty
 */
bool isBufferEmpty(ExpStack *expStack);

/**
 * @brief Get the datatype of expression object
 * 
 * @param expExp Stack to be processed
 * @return DataType Returned data type of expression
 */
DataType getDataTypeOfExpression(ExpExp *expExp);

/**
 * @brief Finds if the operation inputted is binary or unary
 * 
 * @param operation Inputed operation
 * @return true Operation is binary
 * @return false Operation is unary
 */
bool isBinaryOperation(OperationType operation);
/** ------------------------ Short-Hands ------------------------ **/

/**
 * @brief Checks if expression types are correct with IFJ20 guidelines
 * 
 * @param newExpExp Values which is checked
 * @return int SUCCESS Everything was completed successfully
 * @return int SEMANTIC_ERROR_TYPE_EXPRESSION Operation cannot be processed with given types
 * 
 */
int evaluateTypeOfExpressions(ExpExp *newExpExp);

/**
 * @brief The final state machine which analyzes the rules in stack
 * 
 * @param expStack Stack to be processed
 * @param newExpExp New generated expression (operation)
 * @return int SUCCESS Everything was completed successfully
 * @return int INTERNAL_ERROR There was internal (probably memory) error
 * @return int SYNTAX_ERROR The syntax of expression was incorrect
 * @return int SEMANTIC_ERROR_DIV_ZERO Expression was divided by zero
 * @return int SEMANTIC_ERROR_TYPE_EXPRESSION Operation cannot be processed with given types
 * 
 */
int rulesEvaluation(ExpStack *expStack, ExpExp *newExpExp);

/**
 * @brief Evaluates how new expression fits into stacks and assigns associativity
 * 
 * @param expStack Stack to be processed
 * @param expItem New item to be processed
 * @return int SUCCESS Everything was completed successfully
 * @return int INTERNAL_ERROR There was internal (probably memory) error
 * @return int SYNTAX_ERROR The syntax of expression was incorrect
 * @return int SEMANTIC_ERROR_DIV_ZERO Expression was divided by zero
 * @return int SEMANTIC_ERROR_TYPE_EXPRESSION Operation cannot be processed with given types
 * 
 */
int evaluateExpression(ExpStack *expStack, ExpItem *expItem);

/**
 * @brief Translates tokens to operators and expressions. It also validates the order. 
 * 
 * @param token Incomming token which is processed by scanner
 * @param expStack Stack to be processed
 * @param expItem Returned analyzed item
 * @return int SUCCESS Everything was completed successfully
 * @return int INTERNAL_ERROR There was internal (probably memory) error
 * @return int ANALYSIS_END There is nothing left to analyze, but everything was completed successfully
 * @return int SYNTAX_ERROR The syntax of expression was incorrect
 * 
 */
int checkIfValidToken(Token *token, ExpStack *expStack, ExpItem *expItem);

/**
 * @brief This function will process the assingment operation after completed expression evaluation
 * 
 * @param expStack Stack to be processed
 * @param assingmentOperation Assignment operator by which analyzer decides if is needed to add the value to a symbol
 * @param symbol Symbol (variable) which expression analyzer needs to process the assignment operation
 * @return int SUCCESS Everything was completed successfully
 * @return int INTERNAL_ERROR There was internal (probably memory) error
 * @return int SYNTAX_ERROR The syntax of expression was incorrect
 * @return int SEMANTIC_ERROR_DIV_ZERO Expression was divided by zero
 * @return int SEMANTIC_ERROR_TYPE_EXPRESSION Operation cannot be processed with given types
 * 
 */
int addOperatorAssignToStack(ExpStack *expStack, OperatorAssign assingmentOperation, const SymbolVariable *symbol);

/**
 * @brief Main function which is executed by parser
 * 
 * @param expression Expression to where the value is returned
 * @param assingmentOperation Assignment operator by which analyzer decides if is needed to add the value to a symbol
 * @param symbol Symbol (variable) which expression analyzer needs to process the assignment operation
 * @return int SUCCESS Everything was completed successfully
 * @return int INTERNAL_ERROR There was internal (probably memory) error
 * @return int SYNTAX_ERROR The syntax of expression was incorrect
 * @return int SEMANTIC_ERROR_DIV_ZERO Expression was divided by zero
 * @return int SEMANTIC_ERROR_TYPE_EXPRESSION Operation cannot be processed with given types
 * 
 */
int parseExpression(ExpExp** expression, OperatorAssign assingmentOperation, const SymbolVariable *symbol);

#endif
