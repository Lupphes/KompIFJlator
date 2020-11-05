#include <stdio.h>
#define FILE_LENGHT 2
#define FILE_NAME "test.txt"

#define EOL '\0'

FILE * fPtr;


typedef enum {
    GENERAL_ERROR = 1,
    FILE_ERROR
} _TestError;


static const char* enumTokenTranslate[] = {
    "TokenEndOfString", 

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
    "TokenUndefined",   
};


