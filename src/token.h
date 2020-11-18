#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>
#include "str.h"
#include "data_type.h"

/**
 * @enum ScannerState
 * @brief Describes all tokens which scanner can send.
 *
 */
typedef enum  {
    /* Definition of Tokens */
    /* Strings */
    TokenStringLiteral, /** Returned string literal (value is in the attribute 's') */
    /* Conditions */
    TokenNotEqual,  /** Returned '!=' */
    TokenIsEqual, /** Returned '=' */
    TokenIsGreaterThan, /** Returned '>' */
    TokenIsGreaterEqual, /** Returned '>=' */
    TokenIsLessThan, /** Returned '<' */
    TokenIsLessEqual, /** Returned '>=' */
    /* Feature UNARY */
    TokenAddEqual, /** Returned '+=' */
    TokenSubtractEqual, /** Returned '-=' */
    TokenMultiplyEqual, /** Returned '*=' */
    TokenDivideEqual, /** Returned '/=' */
    /* Operation */
    TokenAssignment, /** Returned '=' */
    TokenVarDefine, /** Returned ':=' */
    TokenAdd, /** Returned '+' */
    TokenSubtract, /** Returned '-' */
    TokenMultiply, /** Returned '*' */
    TokenDivide, /** Returned '/' */
    /* Numbers */
    TokenWholeNbr, /** Returned whole number (value is in the attribute 'i') */
    TokenDecimalNbr, /** Returned decimal number (value is in the attribute 'd') */
    /* Symbols */   
    TokenLeftCurlyBracket, /** Returned '{' */
    TokenRightCurlyBracket, /** Returned '}' */
    TokenLeftBracket, /** Returned '(' */
    TokenRightBracket, /** Returned ')' */
    TokenComma, /** Returned ',' */
    TokenSemicolon, /** Returned ';' */
    TokenIdentifier, /** Returned string identifier (value is in the attribute 's') */
    TokenEOL, /** Returned End of Line – '\n' */
    TokenEOF, /** Returned EOL –  End of file */
    /* Keywords */ 
    TokenFor, /** Returned keyword 'for' */
    TokenReturn, /** Returned keyword 'return' */
    TokenElse, /** Returned keyword 'else' */
    TokenFunc, /** Returned keyword 'func' */
    TokenIf, /** Returned keyword 'if' */
    TokenPackage, /** Returned keyword 'package' */
    TokenDataType, /** Returned keyword 'float64', 'string' or 'int'. The exact datatype is described in attribute 't' */
    TokenUndefined /** Undefined token – Error state */
} _TokenType;

/** @struct Attribute
 *  @brief This structure is used to identify which value has the token.
 */
typedef union {
  int64_t i; /** Value of token if it is 'TokenWholeNbr' */
  double d; /** Value of token if it is 'TokenDecimalNbr' */
  string s; /** Value of token if it is 'TokenStringLiteral' or 'TokenIdentifier' */
  DataType t; /** Value of token if it is 'TokenDataType' */
} Attribute;

/** @struct Token
 *  @brief This structure describes the token. It consists of the token type and optional attribute.
 */
typedef struct {
    _TokenType type; /** Identify token type */
    Attribute attribute; /** Attribute for token value */
} Token;

#endif