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
    StatementTypeAssignment,
    StatementTypeFunctionCall,
    StatementTypeIf,
    StatementTypeFor,
    StatementTypeReturn
} ASTStatementType;

typedef struct {
    const ExpressionArray rValues;
}   ASTNodeReturn;

typedef struct {
    const SymbolVariableArray lValues;
    const ExpressionArray rValues;
} ASTNodeAssignment;

typedef struct {
    const SymbolVariable* initVariable;
    const ASTNodeAssignment* initAssignment;
    const ExpExp* condition;
    const ASTNodeAssignment* incrementAssignment;
    const struct _AstNodeBlock* code;
} ASTNodeFor;

typedef struct {
    const ExpExp* condition;
    const struct _ASTNodeBlock* ifClause;
    const struct _ASTNodeBlock* elseClause;
} ASTNodeIf;

typedef struct {
    const SymbolVariableArray lValues;
    const SymbolFunction* function;
} ASTNodeFunctionCall;

typedef struct _ASTNodeStatement{
    const ASTStatementType type;
    union {
        const ASTNodeAssignment assignment;
        const ASTNodeFunctionCall functionCall;
        const ASTNodeIf ifStatement;
        const ASTNodeFor forStatement;
        const ASTNodeReturn returnStatement;
    } value;
    const struct _ASTNodeStatement* next;
} ASTNodeStatement;

typedef struct _ASTNodeBlock {
    const SymbolVariableArray variables;
    const ASTNodeStatement* firstStatement;
} ASTNodeBlock;

typedef struct {
    const SymbolFunction* function;
    const ASTNodeBlock* code;
} ASTNodeFunction;

typedef struct{
    ASTNodeFunction* userFunctions; //Later I'll probably use a helper.h array for this.
    ASTNodeFunction* mainFunction;
} ASTRoot;

#endif