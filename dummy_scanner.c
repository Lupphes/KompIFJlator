#include "scanner.h"
#include "token.h"
#include "str.h"
#include <stdio.h>

int i = 0;

Token dummyTokens[] = {
    {TokenPackage},
    {TokenIdentifier},
    {TokenFunc},
    {TokenIdentifier},
    {TokenLeftBracket},
    {TokenRightBracket},
    {TokenLeftCurlyBracket},
    {TokenReturn},
    {TokenWholeNbr},
    {TokenRightCurlyBracket},
    {TokenEOF}
};

int getToken(Token* token){
    if (i == 0){
        strInit(&dummyTokens[1].atribute.s);
        strAddChar(&dummyTokens[1].atribute.s, 'm');
        strAddChar(&dummyTokens[1].atribute.s, 'a');
        strAddChar(&dummyTokens[1].atribute.s, 'i');
        strAddChar(&dummyTokens[1].atribute.s, 'n');
    }
    if (i < sizeof(dummyTokens)/sizeof(Token)){
        *token = dummyTokens[i];
        i++;
        return 0;
    }
    else{
        printf("OH NO.");
        return 0;
    }
}