/*File name: parser.c -----------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Viktor Rucký (xrucky01)                                |
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

#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "error.h"
#include <stdbool.h>

//Alias for semantic purposes. The function nextToken can be used to advance to the next token regardless of what the current token is; in other words, it accepts any token.
#define acceptAny() nextToken()

//Short hand. Processes the function of the specified nonterminal and returns its failure code if it finds issues; otherwise the control flow will resume.
#define NTERM(nt) if ((returnCode = (nt()))) return returnCode;

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SYNTAX_ERROR.
#define assert(term) if(!accept(term)) return SYNTAX_ERROR;

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SUCCESS, i.e. it instead applies the epsilon rule.
#define assertOrEpsilon(token) if(!accept(token)) return SUCCESS;

Token curTok = {TokenEOF}; //We initialise the current token so that the function nextToken works properly.

/**
 * @brief Checks whether the currently read token is of the specified type, and if it is, advances to the next token.
 * 
 * @param type The type of the token to accept.
 * @return true The accepted token was the current token.
 * @return false The accepted token isn't the current token.
 */
bool accept(_TokenType type){
    if (curTok.type == type){
        nextToken();
        return true;
    }
    return false;
}

/**
 * @brief Checks whether the currently read token is of the specified type.
 * 
 * @param type The type of the token to expect.
 */
bool peek(_TokenType type){
    return curTok.type == type;
}

int nextToken(){
    if (curTok.type == TokenIdentifier || curTok.type == TokenStringLiteral) //Checks whether we need to free the smart string if the token used it.
        strFree(&curTok.atribute.s);
    return getToken(&curTok);
}

/**
 * @brief Starts the parsing process; essentially could be said to start the compilation.
 * 
 * @return int An error code (or SUCCESS) to be returned by main to the operating system.
 */
int beginParsing(){
    nextToken(); //First read of the token.
    return Start();
}

int Start(){
    int returnCode;

    NTERM(Prolog);
    NTERM(Chief);

    return SUCCESS;
}

int Prolog(){
    assert(TokenPackage);

    if(peek(TokenIdentifier)){
        if (!strCmpConstStr(&curTok.atribute.s,"main")){
            acceptAny();
            return SUCCESS;
        }
        else{
            return SEMANTIC_ERROR_OTHER; //The package isn't named "main".
        }
    }
    else{
        return SYNTAX_ERROR;
    }
}

int Chief(){
    int returnCode;

    if (accept(TokenEOF))
        return SUCCESS;
    
    NTERM(FunctionDefinition);
    NTERM(Chief);
    
    return SUCCESS;
}

int FunctionDefinition(){
    int returnCode;
    
    assert(TokenFunc);

    if(peek(TokenIdentifier)){
        //Todo: Handling of function name assignment to the symtable. I guess?
        acceptAny();
    }
    else return SYNTAX_ERROR;

    assert(TokenLeftBracket);
    NTERM(FunctionDefinitionParameters_Start); //Todo: yes.
    assert(TokenRightBracket);
    NTERM(FunctionReturnValues); //Todo: yes.
    NTERM(Block);
    return SUCCESS;
}

int FunctionDefinitionParameters_Start(){
    int returnCode;
    
    assertOrEpsilon(TokenIdentifier);
    assert(TokenDataType);
    NTERM(FunctionDefinitionParameters_Next);
    return SUCCESS;
}

int FunctionDefinitionParameters_Next(){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    assert(TokenIdentifier);
    assert(TokenDataType);
    NTERM(FunctionDefinitionParameters_Next);
    return SUCCESS;
}

int FunctionReturnValues(){
    int returnCode;

    assertOrEpsilon(TokenLeftBracket);
    NTERM(FunctionReturnValues_First);
    assert(TokenRightBracket);
    return SUCCESS;
}

int FunctionReturnValues_First(){
    int returnCode;

    assertOrEpsilon(TokenDataType);
    NTERM(FunctionReturnValues_Next);
    return SUCCESS;
}

int FunctionReturnValues_Next(){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    assert(TokenDataType);
    NTERM(FunctionReturnValues_Next);
    return SUCCESS;
}

int Block(){
    int returnCode;
    
    //PLACEHOLDER!!!
    assert(TokenLeftCurlyBracket);
    while(!(peek(TokenLeftCurlyBracket) || peek(TokenRightCurlyBracket))) acceptAny();
    if (peek(TokenLeftCurlyBracket)){
        NTERM(Block);
    }
    
    assert(TokenRightCurlyBracket);
    return SUCCESS;
}