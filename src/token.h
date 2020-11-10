#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>
#include "str.h"

typedef enum  {
    /* Definition of Tokens */
    /* Strings */
    TokenStringLiteral,
    /* Conditions */
    TokenNotEqual,
    TokenIsEqual,
    TokenIsGreaterThan,
    TokenIsGreaterEqual,
    TokenIsLessThan,
    TokenIsLessEqual,
    /* Operation */
    TokenEquals,
    TokenVarDefine,
    TokenAdd,
    TokenSubtract,
    TokenMultiply,
    TokenDivide,
    /* Numbers */
    TokenWholeNbr,
    TokenDecimalNbr,
    /* Symbols */   
    TokenLeftCurlyBracket,
    TokenRightCurlyBracket,
    TokenRightBracket,
    TokenLeftBracket,
    TokenComma,
    TokenSemicolon,
    TokenIdentifier,
    TokenEOL, // Needs to be discussed
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