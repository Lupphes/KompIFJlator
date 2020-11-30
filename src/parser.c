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

#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "parser_common.h"
#include "token.h"
#include "scanner.h"
#include "error.h"
#include "symbol.h"
#include "symtable.h"
#include "helper.h"
#include "term.h"

//Short hand. Processes the function of the specified nonterminal and returns its failure code if it finds issues; otherwise the control flow will resume.
#define NTERM(nt) callAndHandleException(nt())

//Variant of assert that executes the clean-up function instead of returning straight away in case of failure.
#define NTERM_clean(nt) callAndHandleException_clean(nt())

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
    int returnCode = SUCCESS;
    SymbolFunction function;
    function.parameters.count = 0;
    function.parameters.params = NULL;
    function.returnTypes.count = 0;
    function.returnTypes.types = NULL;
    callAndHandleException(strInit(&function.id));

    //Syntactical and semantic checks.

    assert_clean(TokenFunc);

    if(peek(TokenIdentifier)){
        if (getFunction(strGetStr(&curTok.attribute.s)) == NULL){
            callAndHandleException_clean(strCopyString(&function.id,&curTok.attribute.s));
            acceptAny();
        } else return SEMANTIC_ERROR_DEFINITION; // A function with the given name already exists.
    }
    else return SYNTAX_ERROR;

    assert_clean(TokenLeftBracket);
    callAndHandleException_clean(FunctionDefinitionParameters_Start(&function));
    assert_clean(TokenRightBracket);
    callAndHandleException_clean(FunctionReturnValues(&function));
    
    //Symtable management

    callAndHandleException_clean(addFunction(&function));
    
    callAndHandleException_clean(enterNewStackFrame());
    for (int i = 0; i < function.parameters.count;i++){
        SymbolVariable var = {.id = function.parameters.params[i].id, .type = function.parameters.params[i].type};
        callAndHandleException_clean(addVariable(&var));
    }

    callAndHandleException_clean(Block(false)); //Todo: attach to the AST generation later.
    
    leaveStackFrame();

    CLEAN_UP:
    strFree(&function.id);
    for (int i = 0; i < function.parameters.count;i++)
        strFree(&function.parameters.params[i].id);
    free(function.parameters.params);
    free(function.returnTypes.types);
    return returnCode;
}

/**
 * @brief Adds a function parameter to the given function definition. 
 * 
 * @param function The function symbol to which to add the parameter.
 * @param id The name of the parameter.
 * @param type The type of the paramter.
 * @return SUCCESS The function added the paramter to the given function.
 * @return SEMANTIC_ERROR_DEFINITION A parameter with the given id was already given to the function.
 * @return INTERNAL_ERROR The function encountered an unrecoverable error.
 */
int addFunctionParameter(SymbolFunction* function, const string* id, DataType type){
    int returnCode;
    
    for(int i = 0;i < function->parameters.count;i++)
        if(strCmpString(&function->parameters.params[i].id,id) == 0)
            return SEMANTIC_ERROR_DEFINITION;
    
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
 * @return int SEMANTIC_ERROR_DEFINITION If the function already has a parameter with the given id.
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
        acceptAny();
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
    acceptAny();
    callAndHandleException(FunctionReturnValues_Next(function));

    return SUCCESS;
}

int FunctionReturnValues_Next(SymbolFunction* function){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    if(!peek(TokenDataType))
        return SYNTAX_ERROR;
    
    callAndHandleException(addFunctionReturnType(function, curTok.attribute.t));
    acceptAny();
    callAndHandleException(FunctionReturnValues_Next(function));

    return SUCCESS;
}

int Block(bool newStackFrame){
    int returnCode;
    
    if (newStackFrame)
        callAndHandleException(enterNewStackFrame());

    assert(TokenLeftCurlyBracket);
    EOL_Mandatory();
    NTERM(Statement);
    EOL_Optional(); //TODO: Investigate.
    assert(TokenRightCurlyBracket);

    if (newStackFrame)
        leaveStackFrame();

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
    
    const int numberOfDubiousFunctionCallsBeforeApplyingThisRule = countInDubiousFunctionCallArray(&dubiousFunctionCalls);
    string firstID;
    SymbolVariableArray* lValues = malloc(sizeof(SymbolVariableArray));
    if (lValues == NULL)
        return INTERNAL_ERROR;
    initSymbolVariableArray(lValues);
    callAndHandleException_clean(strInit(&firstID));
    const SymbolVariable* firstVariable;

    if(peek(TokenIdentifier)){ //Unnecessary check since we already know that we are dealing with an identifier as the current token, but for good measure I check here as well.
        callAndHandleException_clean(strCopyString(&firstID, &curTok.attribute.s));
        acceptAny();
    } else returnAndClean(SYNTAX_ERROR);

    switch(curTok.type){
        case TokenLeftBracket:
            if (getVariable(strGetStr(&firstID)) != NULL)
                returnAndClean(SEMANTIC_ERROR_OTHER);
            callAndHandleException_clean(FunctionCall_rule(lValues,getFunction(strGetStr(&firstID)),&firstID));
            returnAndClean(SUCCESS);
        case TokenVarDefine:
            callAndHandleException_clean(VariableDefinition(&firstID));
            returnAndClean(SUCCESS);
        case TokenAssignment:
        case TokenComma:
            firstVariable = getVariable(strGetStr(&firstID));
            if (firstVariable == NULL) //No variable with the given name exists in the current context.
                returnAndClean(SEMANTIC_ERROR_DEFINITION);
            callAndHandleException_clean(addToSymbolVariableArray(lValues,firstVariable));
            callAndHandleException_clean(Assignment(lValues));
            returnAndClean(SUCCESS);
        default:
            returnAndClean(SYNTAX_ERROR);
    }

    CLEAN_UP:
    strFree(&firstID);
    if (numberOfDubiousFunctionCallsBeforeApplyingThisRule == countInDubiousFunctionCallArray(&dubiousFunctionCalls)){ //If we added a dubious function call, we need a reference to its lvalues later, so we can't free them now.
        freeSymbolVariableArray(lValues);
        free(lValues);
    }
    return returnCode;

}

int Assignment(SymbolVariableArray* lValues){
    int returnCode = SUCCESS;
    string functionCandidate;
    callAndHandleException(strInit(&functionCandidate));

    callAndHandleException_clean(VariableList_Next(lValues));
    assert_clean(TokenAssignment);

    if (peek(TokenIdentifier)){
        callAndHandleException_clean(strCopyString(&functionCandidate, &curTok.attribute.s));
        acceptAny();
        if(peek(TokenLeftBracket)){ //We are dealing with a function call with assignment now.
            if (getVariable(strGetStr(&functionCandidate)) != NULL)
                returnAndClean(SEMANTIC_ERROR_OTHER);
            const SymbolFunction* function = getFunction(strGetStr(&functionCandidate));
            callAndHandleException_clean(FunctionCall_rule(lValues, function,&functionCandidate)); //TODO: AST handling somewhere.
        }
        else{
            callAndHandleException_clean(AssignmentOfExpressions(lValues));
        }
    }
    else{
        callAndHandleException_clean(AssignmentOfExpressions(lValues));
    }
    
    CLEAN_UP:
    strFree(&functionCandidate);
    return returnCode;
}

int AssignmentOfExpressions(const SymbolVariableArray* lValues){
    int returnCode = SUCCESS;
    
    TermArray expressionList; //TODO: Waiting for expression team.
    initTermArray(&expressionList);

    callAndHandleException_clean(ExpressionList_Start(&expressionList));

    if (countInTermArray(&expressionList) == 0)
        returnAndClean(SYNTAX_ERROR);
    if (countInSymbolVariableArray(lValues) != countInTermArray(&expressionList))
        returnAndClean(SEMANTIC_ERROR_OTHER);
    for(int i = 0;i < countInSymbolVariableArray(lValues);i++)
        if(lValues->arr[i]->type != TypeBlackHole && lValues->arr[i]->type != termType(expressionList.arr[i]))
            returnAndClean(SEMANTIC_ERROR_OTHER);

    CLEAN_UP:
    freeTermArray(&expressionList);
    return returnCode;
}

int ExpressionList_Start(TermArray* expressionList){
    int returnCode = SUCCESS;
    Term* expression = malloc(sizeof(Term));
    if (expression == NULL)
        return INTERNAL_ERROR;

    if ((returnCode = parseTerm(expression,true)) != SUCCESS && returnCode != SYNTAX_ERROR) //TODO: Adjust once work on expression parser is begun.
        return returnCode; //If there is an error while parsing the expression
    else if (returnCode == SYNTAX_ERROR)
        return SUCCESS; //Epsilon rule.
    
    callAndHandleException_clean(addToTermArray(expressionList,expression));
    callAndHandleException(ExpressionList_Next(expressionList));

    return returnCode;
    CLEAN_UP:
    freeTerm(expression);
    free(expression);
    return returnCode;
}

int ExpressionList_Next(TermArray* expressionList){
    int returnCode = SUCCESS;
    Term* expression = malloc(sizeof(Term));
    if (expression == NULL)
        return INTERNAL_ERROR;

    assertOrEpsilon_clean(TokenComma);
    
    callAndHandleException_clean(parseTerm(expression,true));
    callAndHandleException_clean(addToTermArray(expressionList,expression));
    callAndHandleException(ExpressionList_Next(expressionList));

    return returnCode;
    CLEAN_UP:
    freeTerm(expression);
    free(expression);
    return returnCode;
}

int VariableList_Next(SymbolVariableArray* lValues){
    int returnCode;
    
    assertOrEpsilon(TokenComma);
    
    if (peek(TokenIdentifier)){
        const SymbolVariable* foundVariable = getVariable(strGetStr(&curTok.attribute.s));
        if (foundVariable == NULL)
            return SEMANTIC_ERROR_DEFINITION;
        callAndHandleException(addToSymbolVariableArray(lValues,foundVariable));
        acceptAny();
    } else return SYNTAX_ERROR;
    
    callAndHandleException(VariableList_Next(lValues));
    
    return SUCCESS;
}

int VariableDefinition(string* idName){
    int returnCode;
    SymbolVariable newVariable;
    callAndHandleException(strInit(&newVariable.id));

    assert(TokenVarDefine);
    Term term;
    if((returnCode = parseTerm(&term,true)) != SUCCESS){
        strFree(&newVariable.id);
        return returnCode;
    }

    if (strCopyString(&newVariable.id,idName) != SUCCESS){
        strFree(&newVariable.id);
        freeTerm(&term);
        return INTERNAL_ERROR;
    }
    newVariable.type = termType(&term); //Todo: Waiting for expression team.
    
    if((returnCode = addVariable(&newVariable)) != SUCCESS){
        strFree(&newVariable.id);
        freeTerm(&term);
        return returnCode;
    }

    strFree(&newVariable.id);
    freeTerm(&term);

    return SUCCESS;
}

int FunctionCall_rule(SymbolVariableArray* lValues, const SymbolFunction* function, const string* functionName){
    int returnCode = SUCCESS;
    TermArray* functionParameters = malloc(sizeof(TermArray));
    if(functionParameters == NULL)
        return INTERNAL_ERROR;
    
    initTermArray(functionParameters);

    assert_clean(TokenLeftBracket);
    callAndHandleException_clean(TermList(functionParameters));    
    assert_clean(TokenRightBracket);

    if (function != NULL){ //We are dealing with an already defined function.
        callAndHandleException_clean(validateFunctionCall(function,lValues,functionParameters));
    }
    else{ //We cannot determine the semantic correctness of the function call yet; we instead save this occurence for later investigation.
        DubiousFunctionCall functionCall = {.functionParameters=functionParameters, .lValues=lValues};
        callAndHandleException_clean(strInit(&functionCall.functionName));
        callAndHandleException_clean(strCopyString(&functionCall.functionName,functionName));
        callAndHandleException_clean(addToDubiousFunctionCallArray(&dubiousFunctionCalls,&functionCall));
        return SUCCESS; //We don't want to clean the function-parameter array here because we need it later in the dubious-function-call check at the end of parsing.
    }

    CLEAN_UP:
    freeTermArray(functionParameters);
    free(functionParameters);
    return returnCode;
}

int TermList(TermArray* functionParameters){
    int returnCode;
    Term* term = malloc(sizeof(Term));
    if(term == NULL)
        return INTERNAL_ERROR;
    
    if((returnCode = parseTerm(term,true)) == SUCCESS){
        if(addToTermArray(functionParameters,term) != SUCCESS){
            freeTerm(term);
            free(term);
            return INTERNAL_ERROR;
        }
        callAndHandleException(TermListNext(functionParameters));
        return SUCCESS;
    }
    else{
        free(term);
        return returnCode == SYNTAX_ERROR ? SUCCESS : returnCode; //Epsilon rule
    }
}

int TermListNext(TermArray* functionParameters){
    int returnCode;

    assertOrEpsilon(TokenComma);

    Term* term = malloc(sizeof(Term));
    if(term == NULL)
        return INTERNAL_ERROR;

    if((returnCode = parseTerm(term,true)) == SUCCESS){
        addToTermArray(functionParameters,term);
        callAndHandleException(TermListNext(functionParameters));
        return SUCCESS;
    }
    else{
        free(term);
        return returnCode;
    }
}

int If(){
    int returnCode;
    
    assert(TokenIf);
    NTERM(parseExpression_Dummy);
    callAndHandleException(Block(true));
    assert(TokenElse);
    callAndHandleException(Block(true));

    return SUCCESS;
}

int Return(){
    int returnCode;
    TermArray expressions;
    initTermArray(&expressions);

    assert(TokenReturn);
    callAndHandleException(ExpressionList_Start(&expressions));

    freeTermArray(&expressions);
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
    callAndHandleException(Block(true));

    return SUCCESS;

}

int For_Definition(){
    int returnCode;
    
    assertOrEpsilon(TokenIdentifier);
    //NTERM(VariableDefinition);

    return SUCCESS;
}

int For_Assignment(){
    int returnCode;
    
    assertOrEpsilon(TokenIdentifier);
    //NTERM(Assignment);

    return SUCCESS;
}