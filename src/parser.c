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
#include "expression_analysis.h"

const SymbolFunction* currentFunction = NULL;
bool currentFunctionContainsReturnStatement;
SymbolVariableArray variablesInCurrentFunction;

//Short hand. Processes the function of the specified nonterminal and returns its failure code if it finds issues; otherwise the control flow will resume.
#define NTERM(nt) callAndHandleException(nt())

//Variant of assert that executes the clean-up function instead of returning straight away in case of failure.
#define NTERM_clean(nt) callAndHandleException_clean(nt())

int Start(ASTRoot* astRoot){
    int returnCode;    

    EOL_Optional();
    NTERM(Prolog);
    EOL_Mandatory();
    callAndHandleException(Chief(astRoot));

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

int Chief(ASTRoot* astRoot){
    int returnCode;

    if (accept(TokenEOF) == SUCCESS)
        return SUCCESS;
    
    callAndHandleException(FunctionDefinition(astRoot));
    EOL_Optional(); //TODO: Investigate
    callAndHandleException(Chief(astRoot));
    
    return SUCCESS;
}

int FunctionDefinition(ASTRoot* astRoot){
    int returnCode = SUCCESS;
    SymbolFunction function;
    function.parameters.count = 0;
    function.parameters.params = NULL;
    function.parameters.variadic = false;
    function.returnTypes.count = 0;
    function.returnTypes.types = NULL;
    function.builtIn = false;
    initSymbolVariableArray(&variablesInCurrentFunction);
    callAndHandleException(strInit(&function.id));
    ASTNodeBlock* astFunctionCodeBlock = NULL;
    ASTNodeFunction* astFunction = NULL;
    astFunction = malloc(sizeof(ASTNodeFunction));
    if (astFunction == NULL)
        returnAndClean(INTERNAL_ERROR);
    astFunctionCodeBlock = malloc(sizeof(ASTNodeBlock));
    if (astFunctionCodeBlock == NULL)
        returnAndClean(INTERNAL_ERROR);

    //Syntactical and semantic checks.

    assert_clean(TokenFunc);

    if(peek(TokenIdentifier)){
        if (getFunction(strGetStr(&curTok.attribute.s)) == NULL){
            callAndHandleException_clean(strCopyString(&function.id,&curTok.attribute.s));
            acceptAny_clean();
        } else returnAndClean(SEMANTIC_ERROR_DEFINITION); // A function with the given name already exists.
    }
    else returnAndClean(SYNTAX_ERROR);

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
        callAndHandleException_clean(addToSymbolVariableArray(&variablesInCurrentFunction,getVariable(strGetStr(&function.parameters.params[i].id))));
    }

    currentFunction = getFunction(strGetStr(&function.id));
    currentFunctionContainsReturnStatement = false;

    callAndHandleException_clean(Block(astFunctionCodeBlock,false));
    
    //Add implicit return to the end of the function.
    ASTNodeStatement* tmp = malloc(sizeof(ASTNodeStatement));
    if (tmp == NULL)
        returnAndClean(INTERNAL_ERROR);
    tmp->next = NULL;
    tmp->type = StatementTypeReturn;
    initExpressionArray(&tmp->value.returnStatement.rValues);
    if (astFunctionCodeBlock->firstStatement != NULL){
        ASTNodeStatement* i;
        for(i = astFunctionCodeBlock->firstStatement;i->next != NULL;i = i->next);
        i->next = tmp;
    } else {
        astFunctionCodeBlock->firstStatement = tmp;
    }

    
    leaveStackFrame();

    if (function.returnTypes.count != 0 && !currentFunctionContainsReturnStatement)
        returnAndClean(SEMANTIC_ERROR_TYPE_FUNCTION);

    //AST generation

    astFunction->function = currentFunction;
    astFunction->variables = variablesInCurrentFunction;
    astFunction->code = astFunctionCodeBlock;

    if (getFunction("main") == currentFunction){
        astFunction->next = NULL;   
        astRoot->mainFunction = astFunction;
    }
    else{
        astFunction->next = astRoot->userFunctions;
        astRoot->userFunctions = astFunction;
    }

    CLEAN_UP:
    strFree(&function.id);
    for (int i = 0; i < function.parameters.count;i++)
        strFree(&function.parameters.params[i].id);
    free(function.parameters.params);
    free(function.returnTypes.types);
    if (returnCode != SUCCESS){ //If this operation was successful, the freeing will be done when the AST is freed later.
        free(astFunction);
        free(astFunctionCodeBlock);
    }
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

    acceptAny_clean();
    if (peek(TokenDataType)){
        callAndHandleException(addFunctionParameter(function,&id,curTok.attribute.t));
        acceptAny_clean();
    } else {
        strFree(&id);
        return SYNTAX_ERROR;
    }
    strFree(&id);

    return SUCCESS;
    CLEAN_UP:
    strFree(&id);
    return returnCode;
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

int Block(ASTNodeBlock* astBlock, bool newStackFrame){
    int returnCode;
    
    astBlock->firstStatement = NULL;

    if (newStackFrame)
        callAndHandleException(enterNewStackFrame());

    assert(TokenLeftCurlyBracket);
    EOL_Mandatory();
    callAndHandleException(Statement(&astBlock->firstStatement));
    EOL_Optional(); //TODO: Investigate.
    assert(TokenRightCurlyBracket);

    if (newStackFrame)
        leaveStackFrame();

    return SUCCESS;
}

int Statement(ASTNodeStatement** astStatement){
    int returnCode;

    ASTNodeStatement* astStatementTMP = malloc(sizeof(ASTNodeStatement));
    if (astStatement == NULL)
        return INTERNAL_ERROR;
    astStatementTMP->next = NULL;

    switch(curTok.type){
        case TokenIf:
            callAndHandleException(If(astStatementTMP))
            break;
        case TokenReturn:
            callAndHandleException(Return(astStatementTMP));
            break;
        case TokenFor:
            callAndHandleException(For(astStatementTMP));
            break;
        case TokenIdentifier:
            callAndHandleException(StatementStartingWithIdentifier(astStatementTMP));
            break;
        default: //Epsilon case
            free(astStatementTMP);
            return SUCCESS;
    }

    EOL_Mandatory();
    callAndHandleException(Statement(&astStatementTMP->next));
    *astStatement = astStatementTMP;
    return SUCCESS;
}

int StatementStartingWithIdentifier(ASTNodeStatement* astStatement){
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
        acceptAny_clean();
    } else returnAndClean(SYNTAX_ERROR);

    switch(curTok.type){
        case TokenLeftBracket:
            if (getVariable(strGetStr(&firstID)) != NULL)
                returnAndClean(SEMANTIC_ERROR_OTHER);
            callAndHandleException_clean(FunctionCall_rule(astStatement,lValues,getFunction(strGetStr(&firstID)),&firstID));
            returnAndClean(SUCCESS);
        case TokenVarDefine:
            astStatement->type = StatementTypeAssignment;
            callAndHandleException_clean(VariableDefinition(&astStatement->value.assignment, &firstID));
            returnAndClean(SUCCESS);
        case TokenAssignment:
        case TokenComma:
            firstVariable = getVariable(strGetStr(&firstID));
            if (firstVariable == NULL) //No variable with the given name exists in the current context.
                returnAndClean(SEMANTIC_ERROR_DEFINITION);
            callAndHandleException_clean(addToSymbolVariableArray(lValues,firstVariable));
            callAndHandleException_clean(Assignment(astStatement,lValues));
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

int Assignment(ASTNodeStatement* astStatement,SymbolVariableArray* lValues){
    int returnCode = SUCCESS;
    string functionCandidate;
    callAndHandleException(strInit(&functionCandidate));

    callAndHandleException_clean(VariableList_Next(lValues));
    assert_clean(TokenAssignment);

    if (peek(TokenIdentifier)){
        callAndHandleException_clean(strCopyString(&functionCandidate, &curTok.attribute.s));
        acceptAny_clean();
        if(peek(TokenLeftBracket)){ //We are dealing with a function call with assignment now.
            if (getVariable(strGetStr(&functionCandidate)) != NULL)
                returnAndClean(SEMANTIC_ERROR_OTHER);
            const SymbolFunction* function = getFunction(strGetStr(&functionCandidate));
            callAndHandleException_clean(FunctionCall_rule(astStatement,lValues, function,&functionCandidate)); //TODO: AST handling somewhere.
        }
        else{
            callAndHandleException_clean(prevToken());
            astStatement->type = StatementTypeAssignment;
            callAndHandleException_clean(AssignmentOfExpressions(&astStatement->value.assignment,lValues));
        }
    }
    else{
        astStatement->type = StatementTypeAssignment;
        callAndHandleException_clean(AssignmentOfExpressions(&astStatement->value.assignment,lValues));
    }
    
    CLEAN_UP:
    strFree(&functionCandidate);
    return returnCode;
}

int AssignmentOfExpressions(ASTNodeAssignment* astAsignment,const SymbolVariableArray* lValues){
    int returnCode = SUCCESS;
    
    ExpressionArray expressionList;
    initExpressionArray(&expressionList);

    callAndHandleException_clean(ExpressionList_Start(&expressionList));

    if (countInExpressionArray(&expressionList) == 0)
        returnAndClean(SYNTAX_ERROR);
    if (countInSymbolVariableArray(lValues) != countInExpressionArray(&expressionList))
        returnAndClean(SEMANTIC_ERROR_OTHER);
    for(int i = 0;i < countInSymbolVariableArray(lValues);i++)
        if(lValues->arr[i]->type != TypeBlackHole && lValues->arr[i]->type != getDataTypeOfExpression(expressionList.arr[i]))
            returnAndClean(SEMANTIC_ERROR_OTHER);

    //Magic we can't make robust and nice in time.
    astAsignment->lValues.arr = malloc(sizeof(SymbolVariable*) * lValues->count);
    if (astAsignment->lValues.arr == NULL)
        returnAndClean(INTERNAL_ERROR);
    memcpy(astAsignment->lValues.arr,lValues->arr,sizeof(SymbolVariable*)*lValues->count);
    astAsignment->lValues.count = lValues->count;
    astAsignment->rValues = expressionList;

    return returnCode;
    CLEAN_UP:
    freeExpressionArray(&expressionList);
    return returnCode;
}

int ExpressionList_Start(ExpressionArray* expressionList){
    int returnCode;
    ExpExp* expression;

    if ((returnCode = parseExpression(&expression,PLAIN_ASSIGEMENT,NULL)) == NO_EXPRESSION){
        return SUCCESS; //Epsilon rule.
    }
    else if (returnCode != SUCCESS){
        return returnCode; //If there is an error while parsing the expression
    }
    
    if (getDataTypeOfExpression(expression) == TypeBlackHole)
        returnAndClean(SEMANTIC_ERROR_DEFINITION);

    callAndHandleException_clean(addToExpressionArray(expressionList,expression));
    callAndHandleException(ExpressionList_Next(expressionList));

    return SUCCESS;
    CLEAN_UP:
    freeExpExp(expression);
    free(expression);
    return returnCode;
}

int ExpressionList_Next(ExpressionArray* expressionList){
    int returnCode;
    ExpExp* expression;

    assertOrEpsilon(TokenComma);
    
    if ((returnCode = parseExpression(&expression,PLAIN_ASSIGEMENT,NULL)) == NO_EXPRESSION){
        return SYNTAX_ERROR;
    }
    else if (returnCode != SUCCESS){
        return returnCode;
    }
    if (getDataTypeOfExpression(expression) == TypeBlackHole)
        returnAndClean(SEMANTIC_ERROR_DEFINITION);
    callAndHandleException_clean(addToExpressionArray(expressionList,expression));
    callAndHandleException(ExpressionList_Next(expressionList));

    return SUCCESS;
    CLEAN_UP:
    freeExpExp(expression);
    free(expression);
    return returnCode == NO_EXPRESSION ? SYNTAX_ERROR : returnCode;
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

int VariableDefinition(ASTNodeAssignment* astAssignmentStatement,string* idName){
    int returnCode;
    SymbolVariable newVariable;
    callAndHandleException(strInit(&newVariable.id));

    assert(TokenVarDefine);
    ExpExp* expression;
    if((returnCode = parseExpression(&expression,PLAIN_ASSIGEMENT,NULL)) != SUCCESS){
        strFree(&newVariable.id);
        return returnCode == NO_EXPRESSION ? SYNTAX_ERROR : returnCode;
    }

    if (getDataTypeOfExpression(expression) == TypeBool || getDataTypeOfExpression(expression) == TypeBlackHole){
        freeExpExp(expression);
        free(expression);
        strFree(&newVariable.id);
        return SEMANTIC_ERROR_TYPE_EXPRESSION;
    }

    if (strCopyString(&newVariable.id,idName) != SUCCESS){
        strFree(&newVariable.id);
        freeExpExp(expression);
        free(expression);
        return INTERNAL_ERROR;
    }
    newVariable.type = getDataTypeOfExpression(expression);
    
    if((returnCode = addVariable(&newVariable)) != SUCCESS){
        strFree(&newVariable.id);
        freeExpExp(expression);
        free(expression);
        return returnCode;
    }
    const SymbolVariable* symtableVariableEntry = getVariable(strGetStr(idName));
    if ((returnCode = addToSymbolVariableArray(&variablesInCurrentFunction,symtableVariableEntry)) != SUCCESS){
        strFree(&newVariable.id);
        freeExpExp(expression);
        free(expression);
        return returnCode;
    }

    strFree(&newVariable.id);
    
    initSymbolVariableArray(&astAssignmentStatement->lValues);
    initExpressionArray(&astAssignmentStatement->rValues);
    if (addToSymbolVariableArray(&astAssignmentStatement->lValues,symtableVariableEntry) != SUCCESS || addToExpressionArray(&astAssignmentStatement->rValues,expression) != SUCCESS){
        freeSymbolVariableArray(&astAssignmentStatement->lValues);
        freeExpressionArray(&astAssignmentStatement->rValues);
        freeExpExp(expression);
        free(expression);
        return INTERNAL_ERROR;
    }

    return SUCCESS;
}

int FunctionCall_rule(ASTNodeStatement* astStatement, SymbolVariableArray* lValues, const SymbolFunction* function, const string* functionName){
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

    astStatement->type = StatementTypeFunctionCall;
    astStatement->value.functionCall.function = function;
    astStatement->value.functionCall.parameters = *functionParameters;
    astStatement->value.functionCall.lValues.arr = malloc(sizeof(SymbolVariable*) * lValues->count);
    if (astStatement->value.functionCall.lValues.arr == NULL)
        returnAndClean(INTERNAL_ERROR);
    memcpy(astStatement->value.functionCall.lValues.arr,lValues->arr,sizeof(SymbolVariable*)*lValues->count);
    astStatement->value.functionCall.lValues.count = lValues->count;

    free(functionParameters);
    return SUCCESS;
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
        return returnCode;
    }
}

int If(ASTNodeStatement* astIfStatement){
    int returnCode;
    ExpExp* expression = NULL;
    ASTNodeBlock* ifClause   = NULL;
    ASTNodeBlock* elseClause = NULL;
    ifClause = malloc(sizeof(ASTNodeBlock));
    elseClause = malloc(sizeof(ASTNodeBlock));
    if (ifClause == NULL || elseClause == NULL)
        returnAndClean(INTERNAL_ERROR);

    assert(TokenIf);
    if ((returnCode = parseExpression(&expression,PLAIN_ASSIGEMENT,NULL)) == NO_EXPRESSION)
        return SYNTAX_ERROR;
    else if (returnCode != SUCCESS)
        return returnCode;
    if (getDataTypeOfExpression(expression) != TypeBool)
        returnAndClean(SEMANTIC_ERROR_TYPE_EXPRESSION);
    callAndHandleException_clean(Block(ifClause,true));
    assert_clean(TokenElse);
    callAndHandleException_clean(Block(elseClause,true));

    astIfStatement->type = StatementTypeIf;
    astIfStatement->value.ifStatement.condition = expression;
    astIfStatement->value.ifStatement.ifClause = ifClause;
    astIfStatement->value.ifStatement.elseClause = elseClause;

    return SUCCESS;
    CLEAN_UP:
    if (expression != NULL) freeExpExp(expression);
    free(expression);
    free(ifClause);
    free(elseClause);
    return returnCode;
}

int Return(ASTNodeStatement* astStatement){
    int returnCode;
    ExpressionArray expressions;
    initExpressionArray(&expressions);

    currentFunctionContainsReturnStatement = true;

    assert(TokenReturn);
    callAndHandleException_clean(ExpressionList_Start(&expressions));

    if (countInExpressionArray(&expressions) != currentFunction->returnTypes.count)
        returnAndClean(SEMANTIC_ERROR_TYPE_FUNCTION);
    for (int i = 0; i < currentFunction->returnTypes.count;i++)
        if (getDataTypeOfExpression(expressions.arr[i]) != currentFunction->returnTypes.types[i])
            returnAndClean(SEMANTIC_ERROR_TYPE_FUNCTION);

    astStatement->type = StatementTypeReturn;
    astStatement->value.returnStatement.rValues = expressions;
    return SUCCESS;
    CLEAN_UP:
    freeExpressionArray(&expressions);
    return returnCode;
}

int For(ASTNodeStatement* astStatement){
    int returnCode;
    ExpExp* expression;

    astStatement->type = StatementTypeFor;
    astStatement->value.forStatement.code = NULL;
    astStatement->value.forStatement.condition = NULL;
    astStatement->value.forStatement.incrementAssignment = NULL;
    astStatement->value.forStatement.initAssignment = NULL;

    callAndHandleException(enterNewStackFrame());

    assert(TokenFor);
    callAndHandleException(For_Definition(&astStatement->value.forStatement));
    assert(TokenSemicolon);
    if((returnCode = parseExpression(&expression,PLAIN_ASSIGEMENT,NULL)) == NO_EXPRESSION)
        return SYNTAX_ERROR;
    else if (returnCode != SUCCESS)
        return returnCode;
    if (getDataTypeOfExpression(expression) != TypeBool)
        returnAndClean(SEMANTIC_ERROR_TYPE_EXPRESSION);
    astStatement->value.forStatement.condition = expression;
    assert_clean(TokenSemicolon);
    callAndHandleException_clean(For_Assignment(&astStatement->value.forStatement));
    ASTNodeBlock* tmp = malloc(sizeof(ASTNodeBlock));
    astStatement->value.forStatement.code = tmp;
    callAndHandleException_clean(Block(astStatement->value.forStatement.code,true));

    leaveStackFrame();

    return SUCCESS;
    CLEAN_UP:
    return returnCode;

}

int For_Definition(ASTNodeFor* astForStatement){
    int returnCode = SUCCESS;
    string id;

    if(!peek(TokenIdentifier)){
        astForStatement->initAssignment = NULL;
        return SUCCESS; //Epsilon rule
    }

    callAndHandleException_clean(strInit(&id));
    callAndHandleException_clean(strCopyString(&id,&curTok.attribute.s));

    acceptAny_clean();

    ASTNodeAssignment* tmp = malloc(sizeof(ASTNodeAssignment));
    if (tmp == NULL)
        returnAndClean(INTERNAL_ERROR);

    callAndHandleException_clean(VariableDefinition(tmp,&id));
    astForStatement->initAssignment = tmp;

    CLEAN_UP:
    strFree(&id);
    return returnCode;
}

int For_Assignment(ASTNodeFor* astForStatement){
    int returnCode = SUCCESS;
    SymbolVariableArray lValues;
    const SymbolVariable* firstVariable;
    initSymbolVariableArray(&lValues);

    if (!peek(TokenIdentifier)){ //Epsilon rule
        astForStatement->incrementAssignment = NULL;
        return SUCCESS;
    }
    firstVariable = getVariable(strGetStr(&curTok.attribute.s));
    if (firstVariable == NULL)
        return SEMANTIC_ERROR_DEFINITION;
    acceptAny();
    callAndHandleException(addToSymbolVariableArray(&lValues,firstVariable));
    callAndHandleException_clean(VariableList_Next(&lValues));
    assert_clean(TokenAssignment);
    ASTNodeAssignment* tmp = malloc(sizeof(ASTNodeAssignment));
    if (tmp == NULL)
        returnAndClean(INTERNAL_ERROR);
    callAndHandleException_clean(AssignmentOfExpressions(tmp,&lValues));
    astForStatement->incrementAssignment = tmp;

    CLEAN_UP:
    freeSymbolVariableArray(&lValues);
    return returnCode;
}