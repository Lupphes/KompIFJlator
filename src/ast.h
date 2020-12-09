/*File name: ast.h --------------------------------------------------*
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

#ifndef AST_H
#define AST_H
#include "symbol.h"
#include "helper.h"
#include "expression_analysis.h"

struct _ASTNodeBlock;

typedef enum {
    StatementInvalid = 0,
    StatementTypeAssignment,
    StatementTypeFunctionCall,
    StatementTypeIf,
    StatementTypeFor,
    StatementTypeReturn
} ASTStatementType;

typedef struct {
     ExpressionArray rValues;
}   ASTNodeReturn;

typedef struct {
    SymbolVariableArray lValues;
    ExpressionArray rValues;
} ASTNodeAssignment;

typedef struct {
    ASTNodeAssignment* initAssignment;
    ExpExp* condition;
    ASTNodeAssignment* incrementAssignment;
    struct _ASTNodeBlock* code;
} ASTNodeFor;

typedef struct {
    ExpExp* condition;
    struct _ASTNodeBlock* ifClause;
    struct _ASTNodeBlock* elseClause;
} ASTNodeIf;

typedef struct _ASTNodeFunctionCall {
    SymbolVariableArray lValues;
    TermArray parameters;
    const SymbolFunction* function;
} ASTNodeFunctionCall;

typedef struct _ASTNodeStatement{
    ASTStatementType type;
    union {
        ASTNodeAssignment assignment;
        ASTNodeFunctionCall functionCall;
        ASTNodeIf ifStatement;
        ASTNodeFor forStatement;
        ASTNodeReturn returnStatement;
    } value;
    struct _ASTNodeStatement* next;
} ASTNodeStatement;

typedef struct _ASTNodeBlock {
    ASTNodeStatement* firstStatement;
} ASTNodeBlock;

typedef struct _ASTNodeFunction {
    const SymbolFunction* function;
    SymbolVariableArray variables;
    ASTNodeBlock* code;
    struct _ASTNodeFunction* next;
} ASTNodeFunction;

typedef struct{
    ASTNodeFunction* userFunctions;
    ASTNodeFunction* mainFunction;
} ASTRoot;


void freeASTReturn(ASTNodeReturn* x);
void freeASTFor(ASTNodeFor* x);
void freeASTIf(ASTNodeIf* x);
void freeASTFunctionCall(ASTNodeFunctionCall* x);
void freeASTAssignment(ASTNodeAssignment* x);
void freeASTStatement(ASTNodeStatement* x);
void freeASTBlock(ASTNodeBlock* x);
void freeASTFunction(ASTNodeFunction* x);
void freeAST(ASTRoot* root);

#endif