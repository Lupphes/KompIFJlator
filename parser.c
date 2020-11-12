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

//Shorthand. Advances to the next token without any checks of the current token. If there are issues raised by the nextToken function, returns from the function with the return code returned by nextToken.
#define acceptAny() if ((returnCode = nextToken()) != SUCCESS) return returnCode;

//Short hand. Processes the function of the specified nonterminal and returns its failure code if it finds issues; otherwise the control flow will resume.
#define NTERM(nt) if ((returnCode = (nt()))) return returnCode;

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SYNTAX_ERROR. If the acceptance of the next token fails, returns the error value returned by nextToken.
#define assert(term) if((returnCode =  accept(term)) != SUCCESS) return returnCode;

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SUCCESS, i.e. it instead applies the epsilon rule. If there is a failure with nextToken, reuturs the returned error code.
#define assertOrEpsilon(token) if((returnCode = accept(token)) == SYNTAX_ERROR) return SUCCESS; else if (returnCode != SUCCESS) return returnCode;

#define EOL_Mandatory assert(TokenEOL); while(peek(TokenEOL)) { acceptAny(); }

#define EOL_Optional while(peek(TokenEOL)) { acceptAny(); }

Token curTok = {TokenEOF}; //We initialise the current token so that the function nextToken works properly.


int parseExpression_Dummy(){
    int returnCode;

    while(true){
        switch (curTok.type){
            case TokenIdentifier:
            case TokenIsEqual:
            case TokenIsGreaterEqual:
            case TokenIsLessEqual:
            case TokenIsLessThan:
            case TokenWholeNbr:
            case TokenDecimalNbr:
            case TokenStringLiteral:
            case TokenNotEqual:
            case TokenAdd:
            case TokenSubtract:
            case TokenMultiply:
            case TokenDivide:
            case TokenLeftBracket:
            case TokenRightBracket:
                acceptAny();
                continue;
            default:
                break;
        }
        break;
    }
    return SUCCESS;
}

/**
 * @brief Checks whether the currently read token is of the specified type, and if it is, advances to the next token.
 * 
 * @param type The type of the token to accept.
 * @return int 0 (SUCCESS)  Returned if the token is accepted and nextToken did not fail
 * @return int SYNTAX_ERROR Returned if the token is not acccepted.
 * @return int other values If nextToken fails, the error code returned by nextToken is returned.
 */
int accept(_TokenType type){
    if (curTok.type == type){
        return nextToken();
    }
    return SYNTAX_ERROR;
}

/**
 * @brief Checks whether the currently read token is of the specified type.
 * 
 * @param type The type of the token to expect.
 */
bool peek(_TokenType type){
    return curTok.type == type;
}

/**
 * @brief Asks the scanner for the next token and stores it in the curTok variable. The function FREES the string stored in curTok if it needs to.
 * 
 * @return int The return code of the getToken function.
 */
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
    int returnCode;
    if ((returnCode = nextToken()) != SUCCESS) //First read of the token.
        return returnCode;
    return Start();
}

int Start(){
    int returnCode;

    NTERM(Prolog);
    NTERM(Chief);

    return SUCCESS;
}

int Prolog(){
    int returnCode;
    
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

    if (accept(TokenEOF) == SUCCESS)
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
        default:
            return SUCCESS;
            break;
    }
    
    return SUCCESS; //The code will never get here, but the compiler complains about a missing return with a non-void function.
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

    if(!parseExpression_Dummy())
        return SUCCESS; //Epsilon rule; might change. xD TODO
    NTERM(ExpressionList_Next);

    return SUCCESS;
}

int ExpressionList_Next(){
    int returnCode;

    assertOrEpsilon(TokenComma);
    NTERM(parseExpression_Dummy);
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
    NTERM(parseExpression_Dummy);
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
    int returnCode;
    
    switch(curTok.type){
        case TokenWholeNbr:
        case TokenDecimalNbr:
        case TokenStringLiteral:
        case TokenIdentifier:
            acceptAny();
            return SUCCESS;
            break;
        default:
            return SYNTAX_ERROR;
    }
}

int If(){
    int returnCode;
    
    assert(TokenIf);
    NTERM(parseExpression_Dummy);
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
    NTERM(parseExpression_Dummy);
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