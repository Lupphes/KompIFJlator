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
#define NTERM(nt) if (!(returnCode = (nt()))) return returnCode

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SYNTAX_ERROR.
#define assert(term) if(!accept(term)) return SYNTAX_ERROR;

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
    return curTok.type != type;
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
int startParsing(){
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
        if (strCmpConstStr(&curTok.atribute.s,"main")){
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
    
    NTERM(Function);
    NTERM(Chief);
    
    return SUCCESS;
}

int Function(){
    int returnCode;
    
    assert(TokenFunc);

    if(peek(TokenIdentifier)){
        //Todo: Handling of function name assignment to the symtable. I guess?
        acceptAny();
    }
    else return SYNTAX_ERROR;

    assert(TokenLeftBracket);

    NTERM(FunctionDefinitionParameters); //Todo: yes.

    assert(TokenRightBracket);

    NTERM(FunctionReturnValues); //Todo: yes.

    NTERM(Block);
    
}

int FunctionDefinitionParameters(){
    return SUCCESS;
}

int FunctionReturnValues(){
    return SUCCESS;
}

int Block(){
    return SUCCESS;
}