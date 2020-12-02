#include "./../../src/expression_analysis.h"
#include "./../../src/scanner.h"
#include "expression_analysis-test.h"

int main(int argc, char const *argv[]) {
    ExpExp expression;
    Operators assingmentOperation = PLAIN_ASSIGEMENT;
    SymbolVariable *symbol = NULL;
    int result = parseExpression(&expression, assingmentOperation, symbol);
    return result;
}