#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>
#include "str.h"
#include "data_type.h"

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
    TokenAssignment,
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
    TokenLeftBracket,
    TokenRightBracket,
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