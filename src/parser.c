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
#include "term.h"

//Shorthand. Advances to the next token without any checks of the current token. If there are issues raised by the nextToken function, returns from the function with the return code returned by nextToken.
#define acceptAny() if ((returnCode = nextToken()) != SUCCESS) return returnCode;

//Short hand. Processes the function of the specified nonterminal and returns its failure code if it finds issues; otherwise the control flow will resume.
#define NTERM(nt) callAndHandleException(nt())

//Variant of assert that executes the clean-up function instead of returning straight away in case of failure.
#define NTERM_clean(nt) callAndHandleException_clean(nt())

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SYNTAX_ERROR. If the acceptance of the next token fails, returns the error value returned by nextToken.
#define assert(term) if((returnCode =  accept(term)) != SUCCESS) return returnCode;

//Variant of assert that executes the clean-up function instead of returning straight away in case of failure.
#define assert_clean(term) if((returnCode =  accept(term)) != SUCCESS) goto CLEAN_UP;

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SUCCESS, i.e. it instead applies the epsilon rule. If there is a failure with nextToken, reuturs the returned error code.
#define assertOrEpsilon(token) if((returnCode = accept(token)) == SYNTAX_ERROR) return SUCCESS; else if (returnCode != SUCCESS) return returnCode;

#define EOL_Mandatory() assert(TokenEOL); while(peek(TokenEOL)) { acceptAny(); }

#define EOL_Optional() while(peek(TokenEOL)) { acceptAny(); }

Token curTok = {TokenEOF}; //We initialise the current token so that the function nextToken works properly.

DubiousFunctionCallArray dubiousFunctionCalls;

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
        strFree(&curTok.attribute.s);
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
    initDubiousFunctionCallArray(&dubiousFunctionCalls);
    returnCode = Start();
    
    for (int i = 0; i < countInDubiousFunctionCallArray(&dubiousFunctionCalls);i++)
        callAndHandleException(validateFunctionCall())
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
        if (!strCmpConstStr(&curTok.attribute.s,"main")){
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
        if (getFunction(strGetStr(&curTok.attribute.s)) == NULL){
            callAndHandleException(strInit(&function.id));
            callAndHandleException(strCopyString(&function.id,&curTok.attribute.s));
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
    if(strCopyString(&id,&curTok.attribute.s) != SUCCESS){
        strFree(&id);
        return INTERNAL_ERROR;
    }

    acceptAny();
    if (peek(TokenDataType)){
        callAndHandleException(addFunctionParameter(function,&id,curTok.attribute.t));
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

    callAndHandleException(addFunctionReturnType(function,curTok.attribute.t));
    callAndHandleException(FunctionReturnValues_Next(function));

    return SUCCESS;
}

int FunctionReturnValues_Next(SymbolFunction* function){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    if(!peek(TokenDataType))
        return SYNTAX_ERROR;
    
    callAndHandleException(addFunctionReturnType(function, curTok.attribute.t));
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
    int returnCode = SUCCESS;
    
    string firstID;
    SymbolVariableArray lValues;
    callAndHandleException(strInit(&firstID));
    initSymbolVariableArray(&lValues);

    if(peek(TokenIdentifier)){ //Unnecessary check since we already know that we are dealing with an identifier as the current token, but for good measure I check here as well.
        callAndHandleException_clean(strCopyString(&firstID, &curTok.attribute.s));
        acceptAny();
    } else returnAndClean(SYNTAX_ERROR);

    switch(curTok.type){
        case TokenLeftBracket:
            NTERM(FunctionCall_rule);
            return SUCCESS;
        case TokenVarDefine:
            callAndHandleException_clean(VariableDefinition(&firstID));
            returnAndClean(SUCCESS);
        case TokenAssignment:
        case TokenComma:
            SymbolVariable* firstVariable = getVariable(strGetStr(&firstID));
            if (firstVariable == NULL) //No variable with the given name exists in the current context.
                returnAndClean(SEMANTIC_ERROR_DEFINITION);
            callAndHandleException_clean(addToSymbolVariableArray(&lValues,firstVariable));
            callAndHandleException_clean(Assignment(&lValues));
            returnAndClean(SUCCESS);
        default:
            return SYNTAX_ERROR;
    }

    CLEAN_UP:
    strFree(&firstID);
    freeSymbolVariableArray(&lValues);
    return returnCode;

}

int Assignment(SymbolVariableArray* lValues){
    int returnCode = SUCCESS;
    string functionCandidate;
    callAndHandleException(strInit(&functionCandidate));

    callAndHandleException_clean(IDList_Next(lValues));
    assert_clean(TokenAssignment);

    if (peek(TokenIdentifier)){
        callAndHandleException_clean(strCopyString(&functionCandidate, &curTok.attribute.s));
        acceptAny();
        if(peek(TokenLeftBracket)){ //We are dealing with a function call with assignment now.
            SymbolFunction* function = getFunction(strGetStr(&functionCandidate));
            callAndHandleException_clean(FunctionCall_rule(lValues, function,&functionCandidate)); //TODO: AST handling somewhere.
        }
        else{
            NTERM(ExpressionList_Start);
        }
    }
    else{
        NTERM(ExpressionList_Start);
    }
    
    CLEAN_UP:
    strFree(&functionCandidate);
    return returnCode;
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

int IDList_Next(SymbolVariableArray* lValues){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    
    if (peek(TokenIdentifier)){
        SymbolVariable* foundVariable = getVariable(strGetStr(&curTok.attribute.s));
        if (foundVariable == NULL)
            return SEMANTIC_ERROR_DEFINITION;
        addToSymbolVariableArray(lValues,foundVariable);
    } else return SYNTAX_ERROR;
    
    callAndHandleException(IDList_Next(lValues));
    
    return SUCCESS;
}

int VariableDefinition(string* idName){
    int returnCode;
    SymbolVariable newVariable;

    assert(TokenVarDefine);
    NTERM(parseExpression_Dummy); //TODO: Waiting for expression team.
    
    callAndHandleException(strInit(&newVariable.id));
    if (strCopyString(&newVariable.id,idName) != SUCCESS){
        strFree(&newVariable.id);
        return INTERNAL_ERROR;
    }
    newVariable.type = TypeInt; //Todo: Waiting for expression team.
    //Todo: Add the initial assignment to the AST later.

    return SUCCESS;
}

int validateFunctionCall(SymbolFunction* function, SymbolVariableArray* lValues, TermArray* functionParameters){
    //Paremer count check
    if(countInTermArray(&functionParameters) != function->parameters.count){
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

int FunctionCall_rule(SymbolVariableArray* lValues, SymbolFunction* function, string* functionName){
    int returnCode = SUCCESS;
    TermArray* functionParameters = malloc(sizeof(TermArray));
    if(functionParameters == NULL)
        return INTERNAL_ERROR;
    
    initTermArray(functionParameters);

    assert_clean(TokenLeftBracket);
    callAndHandleException_clean(TermList(functionParameters));    
    assert_clean(TokenRightBracket);

    if (function != NULL){ //We are dealing with an already definied function.
        callAndHandleException_clean(validateFunctionCall(function,lValues,functionParameters));
    }
    else{ //We cannot determine the semantic correctness of the function call yet; we instead save this occurence for later investigation.
        DubiousFunctionCall functionCall = {function,functionParameters,lValues};
        callAndHandleException_clean(addToDubiousFunctionCallArray(&dubiousFunctionCalls,functionCall));
        return SUCCESS; //We don't want to clean the function-parameter array here because we need it later in the dubious-function-call check at the end of parsing.
    }

    CLEAN_UP:
    freeTermArray(functionParameters);
    free(functionParameters);
    return returnCode;
}

int TermList(TermArray* functionParameters){
    int returnCode;
    Term* term = malloc(sizeof(term));
    if(term == NULL)
        return INTERNAL_ERROR;
    
    if(Term_rule(term) == SUCCESS){
        addToTermArray(functionParameters,term);
        callAndHandleException(TermListNext(functionParameters));
        return SUCCESS;
    }
    else{
        free(term);
        return SUCCESS; //Epsilon rule
    }
}

int TermListNext(TermArray* functionParameters){
    int returnCode;

    assertOrEpsilon(TokenComma);

    Term* term = malloc(sizeof(term));
    if(term == NULL)
        return INTERNAL_ERROR;

    if((returnCode = Term_rule(term)) == SUCCESS){
        addToTermArray(functionParameters,term);
        callAndHandleException(TermListNext(functionParameters));
        return SUCCESS;
    }
    else{
        free(term);
        return returnCode;
    }
}

DataType termType(Term* term){
    if (term->type != TermVariable)
        return term->type; //The TermType enum has equivalent enum values for all types except TermVariable.
    else
        return term->value.v->type;
}

int Term_rule(Term* term){ //Name collision with the "Term" data type
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
            SymbolVariable* variable = getVariable(strGetStr(&curTok.attribute.s));
            if (variable == NULL)
                return SEMANTIC_ERROR_DEFINITION;
            term->value.v = variable;
            break;
        default:
            return SYNTAX_ERROR;
    }
    acceptAny();
    return SUCCESS;
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