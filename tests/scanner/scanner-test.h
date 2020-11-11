#ifndef SCANNER_TEST_H
#define SCANNER_TEST_H
#include <stdio.h>

static const char* enumTokenTranslate[] = {
    /* Definition of Tokens */
    /* Strings */
    "TokenStringLiteral", 
    /* Conditions */
    "TokenNotEqual", 
    "TokenIsEqual", 
    "TokenIsGreaterThan",
    "TokenIsGreaterEqual", 
    "TokenIsLessThan", 
    "TokenIsLessEqual", 
    /* Operation */
    "TokenAssignment",
    "TokenVarDefine", 
    "TokenAdd",
    "TokenSubtract", 
    "TokenMultiply", 
    "TokenDivide", 
    /* Numbers */
    "TokenWholeNbr",
    "TokenDecimalNbr", 
    /* Symbols */
    "TokenLeftCurlyBracket", 
    "TokenRightCurlyBracket",
    "TokenRightBracket", 
    "TokenLeftBracket", 
    "TokenComma", 
    "TokenSemicolon", 
    "TokenIdentifier",
    "TokenEOL", 
    "TokenEOF", 
    /* Key words */ 
    "TokenFor", 
    "TokenReturn", 
    "TokenElse",
    "TokenFunc", 
    "TokenIf", 
    "TokenPackage", 
    "TokenDataType",  // Float64, String, Int
    "TokenUndefined"
};

#endif /* SCANNER_TEST_H */