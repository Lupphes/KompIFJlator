#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>
#include "str.h"

typedef enum  {
    /* Definition of Tokens */
    /* Strings */
    TokenStringLiteral,
    /* Conditions */
    TokenFeatureExclamationMark,
    TokenNotEqual,
    TokenIsEqual,
    TokenIsGreaterThan,
    TokenIsGreaterEqual,
    TokenIsLessThan,
    TokenIsLessEqual,
    TokenVarDefine, // prase
    /* Operation */
    TokenAdd,
    TokenSubtract,
    TokenMultiply,
    /* Feature or/and */
    TokenAndSecond,
    TokenOrSecond,
    /* Numbers */
    TokenWholeNbr,
    TokenDecimalNbr,
    /* Else */   
    TokenBrackets,
    TokenWhiteSpace,
    TokenLeftCurlyBracket,
    TokenRightCurlyBracket,
    TokenRightBracket,
    TokenLeftBracket,
    TokenComma,
    TokenSemicolon,
    TokenIdentifier,
    TokenEOL,
    TokenEOF,
    /* Key words */ 
    TokenFor,
    TokenReturn,
    TokenElse,
    TokenFunc,
    TokenIf,
    TokenPackage,
    TokenDataType, // Float64, String, Int
    TokenUndefined
} _TokenType;

typedef enum {
  TypeInt,
  TypeFloat64,
  TypeString
} DataType;

typedef union {
  int64_t i;
  double d;
  string s;
  DataType t;
} Atribute;

typedef struct {
    _TokenType type;
    Atribute atribute;
} Token;

#endif