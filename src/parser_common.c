/*File name: parser_common.c ----------------------------------------*
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

#include <stdlib.h>
#include <stdbool.h>
#include "parser_common.h"
#include "parser.h"
#include "symtable.h"
#include "symbol.h"
#include "token.h"
#include "helper.h"
#include "error.h"
#include "scanner.h"

Token curTok = {TokenEOF}; //We initialise the current token so that the function nextToken works properly.

DubiousFunctionCallArray dubiousFunctionCalls;

/**
 * @brief Starts the parsing process; essentially could be said to start the compilation.
 * 
 * @return int An error code (or SUCCESS) to be returned by main to the operating system.
 */
int beginParsing(){
    int returnCode = SUCCESS;
    initDubiousFunctionCallArray(&dubiousFunctionCalls);
    callAndHandleException_clean(nextToken()) //First read of the token.
    
    //callAndHandleException_clean(Start());
    
    for (int i = 0; i < countInDubiousFunctionCallArray(&dubiousFunctionCalls);i++){
        const SymbolFunction* function = getFunction(strGetStr(&dubiousFunctionCalls.arr[i].functionName));
        if (function == NULL)
            returnAndClean(SEMANTIC_ERROR_DEFINITION);
        callAndHandleException_clean(validateFunctionCall(function,dubiousFunctionCalls.arr[i].lValues,dubiousFunctionCalls.arr[i].functionParameters));
    }

    //TODO: Start code generation here.

    CLEAN_UP:
    freeDubiousFunctionCallArray(&dubiousFunctionCalls);
    return returnCode;
        
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
        strFree(&curTok.attribute.s);
    return getToken(&curTok);
}

int validateFunctionCall(const SymbolFunction* function, const SymbolVariableArray* lValues, const TermArray* functionParameters){
    //Paremer count check
    if(countInTermArray(functionParameters) != function->parameters.count){
        return SEMANTIC_ERROR_TYPE_FUNCTION;
    }
    
    //Parameter type check
    for(int i = 0;i < function->parameters.count;i++){
        if(function->parameters.params[i].type != termType(functionParameters->arr[i]))
            return SEMANTIC_ERROR_TYPE_FUNCTION;
    }

    //Return value count check
    if(countInSymbolVariableArray(lValues) != function->returnTypes.count){ //TODO: change on behest of the boss mayble later.
        return SEMANTIC_ERROR_TYPE_FUNCTION;
    }

    //lValues type check
    for(int i=0;i < countInSymbolVariableArray(lValues);i++){
        if(lValues->arr[i]->type != TypeBlackHole && lValues->arr[i]->type != function->returnTypes.types[i])
            return SEMANTIC_ERROR_TYPE_FUNCTION;
    }

    return SUCCESS;
}

/**
 * @brief Tries to parse the current token as a term. Upon success, saves the result to the variable pointed by the "term" parameter.
 * 
 * @param term A pointer to a variable of type Term where the result shall be stored.
 * @param autoAdvance Tells the function whether to automatically advance to the next token if parsing was successful.
 * @retval SUCCESS Parsing was successful and the result has been stored in "term".
 * @retval SYNTAX_ERROR The current token cannot possibly be a term as it is of a wrong type.
 * @retval SEMANTIC_ERROR_DEFINITION The current token is an identifier that corresponds to no valid variable in the current context.
 * @retval LEXICAL_ERROR The "autoAdvance" parameter is set to true and the scanner reported a lexical error while acquiring the next token.
 * @retval INTERNAL_ERROR There was an unrecoverable error with memory allocation.
 */
int parseTerm(Term* term, bool autoAdvance){
    int returnCode;
    
    switch(curTok.type){
        case TokenWholeNbr:
            term->type = TermIntegerLiteral;
            term->value.i = curTok.attribute.i;
            break;
        case TokenDecimalNbr:
            term->type = TermFloatLiteral;
            term->value.d = curTok.attribute.d;
            break;
        case TokenStringLiteral:
            term->type = TermStringLiteral;
            if(strInit(&term->value.s) != SUCCESS){
                strFree(&term->value.s);
                return INTERNAL_ERROR;
            }
            if(strCopyString(&term->value.s,&curTok.attribute.s) != SUCCESS){
                strFree(&term->value.s);
                return INTERNAL_ERROR;
            }
            break;
        case TokenIdentifier:
            term->type = TermVariable;
            const SymbolVariable* variable = getVariable(strGetStr(&curTok.attribute.s));
            if (variable == NULL)
                return SEMANTIC_ERROR_DEFINITION;
            term->value.v = variable;
            break;
        default:
            return SYNTAX_ERROR;
    }
<<<<<<< HEAD
    // acceptAny();
=======
    if (autoAdvance){
        acceptAny();
    }
>>>>>>> origin/master
    return SUCCESS;
}

DataType termType(Term* term){
    if (term->type != TermVariable)
        return term->type; //The TermType enum has equivalent enum values for all types except TermVariable.
    else
        return term->value.v->type;
}
