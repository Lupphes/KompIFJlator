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
#include "dummyExpressionParser.h"
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
    
    assert(TokenLeftCurlyBracket);
    NTERM(Statement);
    assert(TokenRightCurlyBracket);

    return SUCCESS;
}

int Statement(){
    int returnCode;

    switch(curTok.type){
        case TokenIf:
            NTERM(If)
            break;
        case TokenReturn:
            NTERM(Return);
            break;
        case TokenFor:
            NTERM(For);
            break;
        case TokenIdentifier:
            NTERM(StatementStartingWithIdentifier);
            break;
    }

    return SUCCESS;
}

int StatementStartingWithIdentifier(){
    int returnCode;
    
    assert(TokenIdentifier); //TODO: DEFINITELY TODO. THIS WILL BE ANNOYING AF.

    switch(curTok.type){
        case TokenLeftBracket:
            NTERM(FunctionCall);
            return SUCCESS;
        case TokenVarDefine:
            NTERM(VariableDefinition);
            return SUCCESS;
        case TokenAssignment:
        case TokenComma:
            NTERM(Assignment);
            return SUCCESS;
        default:
            return SYNTAX_ERROR;
    }
}

int Assignment(){
    int returnCode;

    NTERM(IDList_Next);
    assert(TokenAssignment);

    if (peek(TokenIdentifier)){
        acceptAny();
        if(peek(TokenLeftBracket)){
            //Some unget token shit here probably, or update to a LL(2) grammar. TODO. :RikoCreepy:
            NTERM(FunctionCall);
        }
        else{
            //Some unget token shit here too probably, or update to a LL(2) grammar. TODO. :RikoCreepy:
            NTERM(ExpressionList_Start);
        }
    }
    else{
        NTERM(ExpressionList_Start);
    }

    return SUCCESS;
}

int ExpressionList_Start(){
    int returnCode;

    if(!parseExpression())
        return SUCCESS; //Epsilon rule; might change. xD TODO
    NTERM(ExpressionList_Next);

    return SUCCESS;
}

int ExpressionList_Next(){
    int returnCode;

    assertOrEpsilon(TokenComma);
    NTERM(parseExpression);
    NTERM(ExpressionList_Next);

    return SUCCESS;
}

int IDList_Next(){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    assert(TokenIdentifier);
    NTERM(IDList_Next);
    
    return SUCCESS;
}

int VariableDefinition(){
    int returnCode;
    
    assert(TokenVarDefine);
    NTERM(parseExpression);
    return SUCCESS;
}

int FunctionCall(){
    int returnCode;
    
    assert(TokenLeftBracket);
    NTERM(TermList);
    assert(TokenRightBracket);
    
    return SUCCESS;
}

int TermList(){
    int returnCode;
    if(Term()){
        NTERM(TermListNext);
        return SUCCESS;
    }
    return SUCCESS;
}

int TermListNext(){
    int returnCode;

    assertOrEpsilon(TokenComma); 
    NTERM(Term); //Handling for allowed extraneous comma here in the future maybe.
    NTERM(TermListNext);

    return SUCCESS;
}

int Term(){
    switch(curTok.type){
        case TokenWholeNbr:
        case TokenDecimalNbr:
        case TokenStringLiteral:
        case TokenIdentifier:
        acceptAny();
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

int If(){
    int returnCode;
    
    assert(TokenIf);
    NTERM(parseExpression);
    NTERM(Block);
    assert(TokenElse);
    NTERM(Block);

    return SUCCESS;
}

int Return(){
    int returnCode;

    assert(TokenReturn);
    NTERM(ExpressionList_Start);

    return SUCCESS;
}

int For(){
    int returnCode;
    
    assert(TokenFor);
    NTERM(For_Definition);
    assert(TokenSemicolon);
    NTERM(parseExpression);
    assert(TokenSemicolon);
    NTERM(For_Assignment);
    NTERM(Block);

    return SUCCESS;

}

int For_Definition(){
    int returnCode;
    
    assertOrEpsilon(TokenIdentifier);
    NTERM(VariableDefinition);

    return SUCCESS;
}

int For_Assignment(){
    int returnCode;
    
    assertOrEpsilon(TokenIdentifier);
    NTERM(Assignment);

    return SUCCESS;
}