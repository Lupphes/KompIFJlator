#include "./../../src/expression_analysis.h"
#include "./../../src/scanner.h"
#include "expression_analysis-test.h"

#include <stdio.h>

int main() {
    ExpExp expression;
    Operator assingmentOperation = PLAIN_ASSIGEMENT;
    SymbolVariable *symbol = NULL;
    int result = parseExpression(&expression, assingmentOperation, symbol);
    printf("Return code is: %d\n", result);
    return result;
}