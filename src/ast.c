/*File name: ast.c --------------------------------------------------*
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

#include "ast.h"
#include "expression_analysis.h"

void freeASTReturn(ASTNodeReturn* x){
    freeExpressionArray(&x->rValues);
}

void freeASTFor(ASTNodeFor* x){
    if (x->condition != NULL){
        freeExpExp(x->condition);
        free(x->condition);
    }
    if (x->incrementAssignment != NULL){
        freeASTAssignment(x->incrementAssignment);
        free(x->incrementAssignment);
    }
    if (x->initAssignment != NULL){
        freeASTAssignment(x->initAssignment);
        free(x->initAssignment);
    }
    if (x->code != NULL){
        freeASTBlock(x->code);
        free(x->code);
    }
}

void freeASTIf(ASTNodeIf* x){
    freeExpExp(x->condition);
    free(x->condition);
    freeASTBlock(x->ifClause);
    free(x->ifClause);
    freeASTBlock(x->elseClause);
    free(x->elseClause);
}

void freeASTFunctionCall(ASTNodeFunctionCall* x){
    freeSymbolVariableArray(&x->lValues);
    freeTermArray(&x->parameters);
}

void freeASTAssignment(ASTNodeAssignment* x){
    freeSymbolVariableArray(&x->lValues);
    freeExpressionArray(&x->rValues);
}

void freeASTStatement(ASTNodeStatement* x){
    switch(x->type){
        case StatementInvalid:
            return;
        case StatementTypeAssignment:
            freeASTAssignment(&x->value.assignment);
            break;
        case StatementTypeFunctionCall:
            freeASTFunctionCall(&x->value.functionCall);
            break;
        case StatementTypeIf:
            freeASTIf(&x->value.ifStatement);
            break;
        case StatementTypeFor:
            freeASTFor(&x->value.forStatement);
            break;
        case StatementTypeReturn:
            freeASTReturn(&x->value.returnStatement);
            break;
    }
}

void freeASTBlock(ASTNodeBlock* x){
    ASTNodeStatement* i = x->firstStatement;
    while (i != NULL){
        ASTNodeStatement* next = i->next;
        freeASTStatement(i);
        free(i);
        i = next;
    }
}

void freeASTFunction(ASTNodeFunction* x){
    freeSymbolVariableArray(&x->variables);
    freeASTBlock(x->code);
    free(x->code);
}

void freeAST(ASTRoot* root){
    ASTNodeFunction* i = root->userFunctions;
    while(i != NULL){
        ASTNodeFunction* next = i->next;
        freeASTFunction(i);
        free(i);
        i = next;
    }
    if (root->mainFunction != NULL){
        freeASTFunction(root->mainFunction);
        free(root->mainFunction);
    }
}