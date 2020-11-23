/*File name: token.h ------------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Ondřej Sloup (xsloup02)                                |
 |            Viktor Rucký (xrucky01)                                |
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
} Attribute;

typedef struct {
    _TokenType type;
    Attribute attribute;
} Token;

#endif