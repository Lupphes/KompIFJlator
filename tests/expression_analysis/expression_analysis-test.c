#include "./../../src/expression_analysis.h"
#include "./../../src/scanner.h"
#include "expression_analysis-test.h"
#include "./../../src/parser_common.h"
#include "./../../src/symtable.h"

#include <stdio.h>

int main() {
    int returnCode = 0;
    ExpExp *expression;
    Operator assingmentOperation = PLAIN_ASSIGEMENT;
    SymbolVariable *symbol = NULL;
    initVariableTableStack();
    acceptAny();
    int result = parseExpression(&expression, assingmentOperation, symbol);
    printf("Return code is: %d\n", result);
    return result;
}