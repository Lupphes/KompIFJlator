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
Token cacheTok = {TokenEOF};
bool historicCurTok = false;

DubiousFunctionCallArray dubiousFunctionCalls;

/**
 * @brief Starts the parsing process; essentially could be said to start the compilation.
 * 
 * @return int An error code (or SUCCESS) to be returned by main to the operating system.
 */
int beginParsing(){
    int returnCode = SUCCESS;
    initDubiousFunctionCallArray(&dubiousFunctionCalls);
    callAndHandleException_clean(initFunctionTable());
    callAndHandleException_clean(initVariableTableStack());
    callAndHandleException_clean(nextToken()) //First read of the token.

    callAndHandleException_clean(Start());
    
    //Checking for existence and proper form of main function.
    const SymbolFunction* mainFunction = getFunction("main");
    if (mainFunction == NULL)
        returnAndClean(SEMANTIC_ERROR_DEFINITION);
    if (mainFunction->parameters.count != 0 || mainFunction->returnTypes.count != 0)
        returnAndClean(SEMANTIC_ERROR_TYPE_FUNCTION);
    
    //Evaluating dubious function calls
    for (int i = 0; i < countInDubiousFunctionCallArray(&dubiousFunctionCalls);i++){
        const SymbolFunction* function = getFunction(strGetStr(&dubiousFunctionCalls.arr[i].functionName));
        if (function == NULL)
            returnAndClean(SEMANTIC_ERROR_DEFINITION);
        callAndHandleException_clean(validateFunctionCall(function,dubiousFunctionCalls.arr[i].lValues,dubiousFunctionCalls.arr[i].functionParameters));
    }
    
    //TODO: Start code generation here.

    CLEAN_UP:
    freeDubiousFunctionCallArray(&dubiousFunctionCalls);
    freeFunctionTable();
    freeVariableTableStack();
    freeCurTok();
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
 * @brief Sets curTok to the next token. If prevToken hasn't been called, this asks the scanner for the next token and frees the memory used by the PREVIOUS token. If prevToken has been called, this restores the state to before prevToken was called.
 * 
 * @retval SUCCESS The operation completed successfully.
 * @retval LEXICAL_ERROR The scanner encountered a lexical error while getting the next token.
 * @retval INTERNAL_ERROR There was an unrecoverable error with memory operations.
 */
int nextToken(){
    if (!historicCurTok){
        if (cacheTok.type == TokenIdentifier || cacheTok.type == TokenStringLiteral) //Checks whether we need to free the smart string if the token used it.
            strFree(&cacheTok.attribute.s);
        cacheTok = curTok;
        return getToken(&curTok);
    }
    else {
        Token tmp = cacheTok;
        cacheTok = curTok;
        curTok = tmp;
        historicCurTok = false;
        return SUCCESS;
    }
}

/**
 * @brief "Goes back one token." Sets the currentToken to its previous value. This function can be used to only go one step back; multiple successive calls result in failure.
 * 
 * @retval SUCCESS The operation completed successfully.
 * @retval INTERNAL_ERROR An attempt was made to go back more than one token in the past. This feature is not supported.
 */
int prevToken(){
    if (historicCurTok)
        return INTERNAL_ERROR;
    else{
        Token tmp = cacheTok;
        cacheTok = curTok;
        curTok = tmp;
        historicCurTok = true;
        return SUCCESS;
    }
}

/**
 * @brief Frees the memory used by the variables used for handling the current token, i.e. curTok and cacheTok.
 * 
 */
void freeCurTok(){
    if (cacheTok.type == TokenIdentifier || cacheTok.type == TokenStringLiteral)
        strFree(&cacheTok.attribute.s);
    if (curTok.type == TokenIdentifier || curTok.type == TokenStringLiteral)
        strFree(&curTok.attribute.s);
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
    if (autoAdvance){
        acceptAny();
    }
    return SUCCESS;
}

/**
 * @brief Returns the data type of a term.
 * 
 * @param term The term to get its data type of.
 * @return DataType The data type of the given term.
 */
DataType termType(Term* term){
    if (term->type != TermVariable)
        return term->type; //The TermType enum has equivalent enum values for all types except TermVariable.
    else
        return term->value.v->type;
}

/**
 * @brief Frees the memory used by a term.
 * 
 * @param term The term to free its memory.
 */
void freeTerm(Term* term){
    if (term->type == TermStringLiteral)
        strFree(&term->value.s);
}