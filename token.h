#include <str.h>
#include <stdint.h>

typedef enum  {
    /* Definition of Tokens */
    /* Strings */
    TokenEndOfString,
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
    TokenDivide,
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
    /* Key words */ 
    TokenFor,
    TokenReturn,
    TokenElse,
    TokenFloat64,
    TokenFunc,
    TokenIf,
    TokenInt,
    TokenString,
    TokenPackage
} _TokenType;

typedef union {
  int64_t i;
  double d;
  string s;
} Atribute;

typedef struct {
    _TokenType type;
    Atribute atribute;
} Token;
