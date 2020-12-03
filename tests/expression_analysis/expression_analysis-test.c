#include "./../../src/expression_analysis.h"
#include "./../../src/scanner.h"
#include "expression_analysis-test.h"

int main() {
    ExpExp expression;
    Operator assingmentOperation = PLAIN_ASSIGEMENT;
    SymbolVariable *symbol = NULL;
    int result = parseExpression(&expression, assingmentOperation, symbol);
    return result;
}