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

/**
 * @enum ScannerState
 * @brief Describes all tokens which scanner can send.
 *
 */
typedef enum  {
    /* Definition of Tokens */
    /* Strings */
    TokenStringLiteral, /** Read string literal (value is in the attribute 's') */
    /* Conditions */
    TokenNotEqual,  /** Read '!=' */
    TokenIsEqual, /** Read '=' */
    TokenIsGreaterThan, /** Read '>' */
    TokenIsGreaterEqual, /** Read '>=' */
    TokenIsLessThan, /** Read '<' */
    TokenIsLessEqual, /** Read '>=' */
    /* Feature UNARY */
    TokenAddEqual, /** Read '+=' */
    TokenSubtractEqual, /** Read '-=' */
    TokenMultiplyEqual, /** Read '*=' */
    TokenDivideEqual, /** Read '/=' */
    /* Operation */
    TokenAssignment, /** Read '=' */
    TokenVarDefine, /** Read ':=' */
    TokenAdd, /** Read '+' */
    TokenSubtract, /** Read '-' */
    TokenMultiply, /** Read '*' */
    TokenDivide, /** Read '/' */
    /* Numbers */
    TokenWholeNbr, /** Read whole number (value is in the attribute 'i') */
    TokenDecimalNbr, /** Read decimal number (value is in the attribute 'd') */
    /* Symbols */   
    TokenLeftCurlyBracket, /** Read '{' */
    TokenRightCurlyBracket, /** Read '}' */
    TokenLeftBracket, /** Read '(' */
    TokenRightBracket, /** Read ')' */
    TokenComma, /** Read ',' */
    TokenSemicolon, /** Read ';' */
    TokenIdentifier, /** Read string identifier (value is in the attribute 's') */
    TokenEOL, /** Read End of Line – '\n' */
    TokenEOF, /** Read EOL –  End of file */
    /* Keywords */ 
    TokenFor, /** Read keyword 'for' */
    TokenReturn, /** Read keyword 'return' */
    TokenElse, /** Read keyword 'else' */
    TokenFunc, /** Read keyword 'func' */
    TokenIf, /** Read keyword 'if' */
    TokenPackage, /** Read keyword 'package' */
    TokenDataType, /** Read keyword 'float64', 'string' or 'int'. The exact datatype is described in attribute 't' */
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