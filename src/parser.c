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
#include "symbol.h"
#include "symtable.h"
#include <stdlib.h>
#include <stdbool.h>
#include "helper.h"

//Shorthand. Advances to the next token without any checks of the current token. If there are issues raised by the nextToken function, returns from the function with the return code returned by nextToken.
#define acceptAny() if ((returnCode = nextToken()) != SUCCESS) return returnCode;

//Short hand. Processes the function of the specified nonterminal and returns its failure code if it finds issues; otherwise the control flow will resume.
#define NTERM(nt) callAndHandleException(nt())

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SYNTAX_ERROR. If the acceptance of the next token fails, returns the error value returned by nextToken.
#define assert(term) if((returnCode =  accept(term)) != SUCCESS) return returnCode;

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SUCCESS, i.e. it instead applies the epsilon rule. If there is a failure with nextToken, reuturs the returned error code.
#define assertOrEpsilon(token) if((returnCode = accept(token)) == SYNTAX_ERROR) return SUCCESS; else if (returnCode != SUCCESS) return returnCode;

#define EOL_Mandatory() assert(TokenEOL); while(peek(TokenEOL)) { acceptAny(); }

#define EOL_Optional() while(peek(TokenEOL)) { acceptAny(); }

Token curTok = {TokenEOF}; //We initialise the current token so that the function nextToken works properly.


#define TEMP_NO_EXPRESSION -1
int parseExpression_Dummy(){
    int returnCode;
    int found = TEMP_NO_EXPRESSION;

    while(true){
        switch (curTok.type){
            case TokenIdentifier:
            case TokenIsEqual:
            case TokenIsGreaterEqual:
            case TokenIsGreaterThan:
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
                found = SUCCESS;
                continue;
            default:
                break;
        }
        break;
    }
    return found;
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

    EOL_Optional();
    NTERM(Prolog);
    EOL_Mandatory();
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
            return SYNTAX_ERROR; //The package isn't named "main".
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
    EOL_Optional(); //TODO: Investigate
    NTERM(Chief);
    
    return SUCCESS;
}

int FunctionDefinition(){
    int returnCode;
    SymbolFunction function;
    function.parameters.count = 0;
    function.parameters.params = NULL;
    function.returnTypes.count = 0;
    function.returnTypes.types = NULL;

    assert(TokenFunc);

    if(peek(TokenIdentifier)){
        if (getFunction(strGetStr(&curTok.atribute.s)) == NULL){
            callAndHandleException(strInit(&function.id));
            callAndHandleException(strCopyString(&function.id,&curTok.atribute.s));
            acceptAny();
        } else return SEMANTIC_ERROR_DEFINITION; // A function with the given name already exists.
    }
    else return SYNTAX_ERROR;

    assert(TokenLeftBracket);
    if((returnCode = FunctionDefinitionParameters_Start(&function)) != SUCCESS){
        strFree(&function.id);
        free(function.parameters.params);
        free(function.returnTypes.types);
        return returnCode;
    }
    assert(TokenRightBracket);
    if((returnCode = FunctionReturnValues(&function)) != SUCCESS){
        strFree(&function.id);
        free(function.parameters.params);
        free(function.returnTypes.types);
        return returnCode;
    }
    
    addFunction(&function);
    
    strFree(&function.id);
    free(function.parameters.params);
    free(function.returnTypes.types);
    
    NTERM(Block); //Todo: attach to the AST generation later.
    return SUCCESS;
}

/**
 * @brief Adds a function parameter to the given function definition. 
 * 
 * @param function The function symbol to which to add the parameter.
 * @param id The name of the parameter.
 * @param type The type of the paramter.
 * @return SUCCESS The function added the paramter to the given function.
 * @return SEMANTIC_ERROR_OTHER A parameter with the given id was already given to the function.
 * @return INTERNAL_ERROR The function encountered an unrecoverable error.
 */
int addFunctionParameter(SymbolFunction* function, const string* id, DataType type){
    int returnCode;
    
    for(int i = 0;i < function->parameters.count;i++)
        if(strCmpString(&function->parameters.params[i].id,id) != 0)
            return SEMANTIC_ERROR_OTHER;
    
    int newCount = ++function->parameters.count;
    if ((function->parameters.params = realloc(function->parameters.params,sizeof(SymbolFunctionParameter)*newCount)) == NULL)
        return INTERNAL_ERROR;
    
    callAndHandleException(strInit(&function->parameters.params[newCount-1].id));
    callAndHandleException(strCopyString(&function->parameters.params[newCount-1].id,id));
    function->parameters.params[newCount-1].type = type;

    return SUCCESS;
}

/**
 * @brief Processes a single function parameter in a function definition. This functions EXPECTS that a TokenIdentifier is present as the current token.
 * 
 * @param function 
 * @return int SUCCESS If everthing was succesful.
 * @return int SYNTAX_ERROR If the second token wasn't of type TokenDataType
 * @return int SEMANTIC_ERROR_OTHER If the function already has a parameter with the given id.
 * @return int INTERNAL_ERROR There was fatal problem with memory allocation.
 */
int processFunctionDefinitionParameter(SymbolFunction* function){
    int returnCode;

    string id;
    callAndHandleException(strInit(&id));
    if(strCopyString(&id,&curTok.atribute.s) != SUCCESS){
        strFree(&id);
        return INTERNAL_ERROR;
    }

    acceptAny();
    if (peek(TokenDataType)){
        callAndHandleException(addFunctionParameter(function,&id,curTok.atribute.t));
    } else {
        strFree(&id);
        return SYNTAX_ERROR;
    }
    strFree(&id);

    return SUCCESS;
}

int FunctionDefinitionParameters_Start(SymbolFunction* function){
    int returnCode;
    
    if (!peek(TokenIdentifier)) //Epsilon rule
        return SUCCESS;
    
    callAndHandleException(processFunctionDefinitionParameter(function));
    callAndHandleException(FunctionDefinitionParameters_Next(function));
    return SUCCESS;
}

int FunctionDefinitionParameters_Next(SymbolFunction* function){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    
    if(!peek(TokenIdentifier))
        return SYNTAX_ERROR;

    callAndHandleException(processFunctionDefinitionParameter(function));
    callAndHandleException(FunctionDefinitionParameters_Next(function));
    return SUCCESS;
}

int FunctionReturnValues(SymbolFunction* function){
    int returnCode;

    assertOrEpsilon(TokenLeftBracket);
    callAndHandleException(FunctionReturnValues_First(function));
    assert(TokenRightBracket);
    return SUCCESS;
}

/**
 * @brief Adds a return type to the specified to the given function definition.
 * 
 * @param function The function to which to add the new return type.
 * @param type The return type to add.
 * @return int SUCCESS If everything was succesful.
 * @return int INTERNAL_ERROR If there was a fatal memory allocation error.
 */
int addFunctionReturnType(SymbolFunction* function, DataType type){
    int newCount = ++function->returnTypes.count;

    if ((function->returnTypes.types = realloc(function->returnTypes.types,sizeof(DataType)*newCount)) == NULL)
        return INTERNAL_ERROR;
    
    function->returnTypes.types[newCount-1] = type;

    return SUCCESS;
}

int FunctionReturnValues_First(SymbolFunction* function){
    int returnCode;

    if(!peek(TokenDataType)) //Epsilon rule
        return SUCCESS;

    callAndHandleException(addFunctionReturnType(function,curTok.atribute.t));
    callAndHandleException(FunctionReturnValues_Next(function));

    return SUCCESS;
}

int FunctionReturnValues_Next(SymbolFunction* function){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    if(!peek(TokenDataType))
        return SYNTAX_ERROR;
    
    callAndHandleException(addFunctionReturnType(function, curTok.atribute.t));
    callAndHandleException(FunctionReturnValues_Next(function));

    return SUCCESS;
}

int Block(){
    int returnCode;
    
    assert(TokenLeftCurlyBracket);
    EOL_Mandatory();
    NTERM(Statement);
    EOL_Optional(); //TODO: Investigate.
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
        default: //Epsilon case
            return SUCCESS;
    }

    EOL_Mandatory();
    NTERM(Statement);
    
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

    if ((returnCode = parseExpression_Dummy()) != SUCCESS && returnCode != TEMP_NO_EXPRESSION) //TODO: Adjust once work on expression parser is begun.
        return returnCode; //If there is an error while parsing the expression
    else if (returnCode == TEMP_NO_EXPRESSION)
        return SUCCESS; //Epsilon rule.
        
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
    if(Term() == SUCCESS){
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