#ifndef SCANNER_TEST_H
#define SCANNER_TEST_H
#include <stdio.h>

static const char* enumTokenTranslate[] = {
    "TokenStringLiteral", 

    "TokenFeatureExclamationMark", 
    "TokenNotEqual", 
    "TokenIsEqual", 
    "TokenIsGreaterThan",
    "TokenIsGreaterEqual", 
    "TokenIsLessThan", 
    "TokenIsLessEqual", 
    "TokenVarDefine", 

    "TokenAdd",
    "TokenSubtract", 
    "TokenMultiply", 
    "TokenDivide", 

    "TokenAndSecond", 
    "TokenOrSecond", 

    "TokenWholeNbr",
    "TokenDecimalNbr", 

    "TokenBrackets", 
    "TokenWhiteSpace", 
    "TokenLeftCurlyBracket", 
    "TokenRightCurlyBracket",
    "TokenRightBracket", 
    "TokenLeftBracket", 
    "TokenComma", 
    "TokenSemicolon", 
    "TokenIdentifier",
    "TokenEOL", 
    "TokenEOF", 
    "TokenEquals",

    "TokenFor", 
    "TokenReturn", 
    "TokenElse",
    "TokenFunc", 
    "TokenIf", 
    "TokenPackage", 
    "TokenDataType", 
    "TokenUndefined"
};

#endif /* SCANNER_TEST_H */