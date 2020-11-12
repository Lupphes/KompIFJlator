#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "str.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

#define INIT_STRING(TestName,index,value) if(strInit(&TestName[index].atribute.s)) {printf("Fatal string init error!\n"); exit(1);} if(strCopyConstString(&TestName[index].atribute.s,value)) {printf("Fatal string init error!\n"); exit(1);}

#define TEST(number,name,data,out_code) printf("Test " #number " -- " #name "\n"); \
    i = 0; \
    dummyTokens = data; \
    tokenCount = sizeof(data)/sizeof(Token); \
    if ((result = beginParsing()) == out_code) \
    printf("SUCCESS\n\n"); \
    else \
    printf("FAILURE: expected " #out_code "; got %d\n\n",result);

int i          = 0;
int tokenCount = 0;
Token* dummyTokens = NULL;

Token TestInputSane[] = {
    {TokenPackage},
    {TokenIdentifier}, //[1]
    {TokenFunc},
    {TokenIdentifier}, //[3]
    {TokenLeftBracket},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenRightCurlyBracket},
    {TokenEOF}
};

Token TestInputWrongPackageName[] = {
    {TokenPackage},
    {TokenIdentifier}, //[1]
    {TokenFunc},
    {TokenIdentifier}, //[3]
    {TokenLeftBracket},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenRightCurlyBracket},
    {TokenEOF}
};

Token TestNoPackage[] = {
    {TokenIdentifier}, //[0]
    {TokenEOF}
};

Token TestInputBigFunction[] = {
    {TokenPackage},
    {TokenIdentifier}, //[1]
    
    //main function here
    {TokenFunc},
    {TokenIdentifier}, //[3]
    {TokenLeftBracket},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenRightCurlyBracket},
    
    //Some fancy function with many return values and many parameters:
    // func fancy (x int, y float64, z string) (int, int, string, float64) {
    //      return 1, 2, "potato", 12.5
    //  } 
    {TokenFunc},
    {TokenIdentifier}, //[9]
    {TokenLeftBracket},
    {TokenIdentifier}, //[11]
    {TokenDataType, {.t = TypeInt}},
    {TokenComma},
    {TokenIdentifier}, //[14]
    {TokenDataType, {.t=TypeFloat64}},
    {TokenComma},
    {TokenIdentifier}, //[17]
    {TokenDataType, {.t=TypeString}},
    {TokenRightBracket},
    {TokenLeftBracket},
    {TokenDataType, {.t=TypeInt}},
    {TokenComma},
    {TokenDataType, {.t=TypeInt}},
    {TokenComma},
    {TokenDataType, {.t=TypeString}},
    {TokenComma},
    {TokenDataType, {.t=TypeFloat64}},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenReturn},
    {TokenWholeNbr, {.i = 1}},
    {TokenComma},
    {TokenWholeNbr, {.i = 2}},
    {TokenStringLiteral}, //[34]
    {TokenComma},
    {TokenDecimalNbr, {.d=12.5}},
    {TokenRightCurlyBracket},
    {TokenEOF}
};

Token TestInputBigFunction_Error_NoCommaParam[] = {
    {TokenPackage},
    {TokenIdentifier}, //[1]
    
    //main function here
    {TokenFunc},
    {TokenIdentifier}, //[3]
    {TokenLeftBracket},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenRightCurlyBracket},
    
    //Some fancy function with many return values and many parameters:
    // func fancy (x int, y float64, z string) (int, int, string, float64) {
    //      return 1, 2, "potato", 12.5
    //  } 
    {TokenFunc},
    {TokenIdentifier}, //[9]
    {TokenLeftBracket},
    {TokenIdentifier}, //[11]
    {TokenDataType, {.t = TypeInt}},
    {TokenComma},
    {TokenIdentifier}, //[14]
    {TokenDataType, {.t=TypeFloat64}},
    {TokenIdentifier}, //[16]
    {TokenDataType, {.t=TypeString}},
    {TokenRightBracket},
    {TokenLeftBracket},
    {TokenDataType, {.t=TypeInt}},
    {TokenComma},
    {TokenDataType, {.t=TypeInt}},
    {TokenComma},
    {TokenDataType, {.t=TypeString}},
    {TokenComma},
    {TokenDataType, {.t=TypeFloat64}},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenReturn},
    {TokenWholeNbr, {.i = 1}},
    {TokenComma},
    {TokenWholeNbr, {.i = 2}},
    {TokenStringLiteral}, //[33]
    {TokenComma},
    {TokenDecimalNbr, {.d=12.5}},
    {TokenRightCurlyBracket},
    {TokenEOF}
};

Token TestInputBigFunction_Error_NoCommaReturn[] = {
    {TokenPackage},
    {TokenIdentifier}, //[1]
    
    //main function here
    {TokenFunc},
    {TokenIdentifier}, //[3]
    {TokenLeftBracket},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenRightCurlyBracket},
    
    //Some fancy function with many return values and many parameters:
    // func fancy (x int, y float64, z string) (int, int, string, float64) {
    //      return 1, 2, "potato", 12.5
    //  } 
    {TokenFunc},
    {TokenIdentifier}, //[9]
    {TokenLeftBracket},
    {TokenIdentifier}, //[11]
    {TokenDataType, {.t = TypeInt}},
    {TokenComma},
    {TokenIdentifier}, //[14]
    {TokenDataType, {.t=TypeFloat64}},
    {TokenComma},
    {TokenIdentifier}, //[17]
    {TokenDataType, {.t=TypeString}},
    {TokenRightBracket},
    {TokenLeftBracket},
    {TokenDataType, {.t=TypeInt}},
    {TokenDataType, {.t=TypeInt}},
    {TokenComma},
    {TokenDataType, {.t=TypeString}},
    {TokenComma},
    {TokenDataType, {.t=TypeFloat64}},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenReturn},
    {TokenWholeNbr, {.i = 1}},
    {TokenComma},
    {TokenWholeNbr, {.i = 2}},
    {TokenStringLiteral}, //[33]
    {TokenComma},
    {TokenDecimalNbr, {.d =12.5}},
    {TokenRightCurlyBracket},
    {TokenEOF}
};

void prepAttributes(){
    INIT_STRING(TestInputSane,1,"main");
    INIT_STRING(TestInputSane,3,"main");

    INIT_STRING(TestInputWrongPackageName,1,"msin");
    INIT_STRING(TestInputWrongPackageName,3,"main");

    INIT_STRING(TestNoPackage,0,"main");

    INIT_STRING(TestInputBigFunction,1,"main");
    INIT_STRING(TestInputBigFunction,3,"main");
    INIT_STRING(TestInputBigFunction,9,"fancy");
    INIT_STRING(TestInputBigFunction,11,"x");
    INIT_STRING(TestInputBigFunction,14,"y");
    INIT_STRING(TestInputBigFunction,17,"z");
    INIT_STRING(TestInputBigFunction,34,"potato");

    INIT_STRING(TestInputBigFunction_Error_NoCommaParam,1,"main");
    INIT_STRING(TestInputBigFunction_Error_NoCommaParam,3,"main");
    INIT_STRING(TestInputBigFunction_Error_NoCommaParam,9,"fancy");
    INIT_STRING(TestInputBigFunction_Error_NoCommaParam,11,"x");
    INIT_STRING(TestInputBigFunction_Error_NoCommaParam,14,"y");
    INIT_STRING(TestInputBigFunction_Error_NoCommaParam,16,"z");
    INIT_STRING(TestInputBigFunction_Error_NoCommaParam,33,"potato");

    INIT_STRING(TestInputBigFunction_Error_NoCommaReturn,1,"main");
    INIT_STRING(TestInputBigFunction_Error_NoCommaReturn,3,"main");
    INIT_STRING(TestInputBigFunction_Error_NoCommaReturn,9,"fancy");
    INIT_STRING(TestInputBigFunction_Error_NoCommaReturn,11,"x");
    INIT_STRING(TestInputBigFunction_Error_NoCommaReturn,14,"y");
    INIT_STRING(TestInputBigFunction_Error_NoCommaReturn,17,"z");
    INIT_STRING(TestInputBigFunction_Error_NoCommaReturn,33,"potato")


}

int getToken(Token* token){
    if (i < tokenCount){
        *token = dummyTokens[i];
    }
    else if (i > tokenCount){
        printf("OH NO.\n");
    }
    i++;
    return 0;
}

int main(){
    int result;
    prepAttributes();
    printf("=====TEST=====\n\n");
    TEST(1,simple main function, TestInputSane, 0);
    result = result;
    TEST(2,package name not main,TestInputWrongPackageName,SEMANTIC_ERROR_OTHER);
    result = result;
    TEST(3,no package token in prologue,TestNoPackage,SYNTAX_ERROR);
    result = result;
    TEST(4,parsing a "big" function with many parameters and return values,TestInputBigFunction,0);
    result = result;
    TEST(5,ditto but with a missing comma in the parameter list,TestInputBigFunction_Error_NoCommaParam,SYNTAX_ERROR);
    result = result;
    TEST(6,ditto but with a missing comma in the return values list,TestInputBigFunction_Error_NoCommaReturn,SYNTAX_ERROR);
    
    return 0;
}