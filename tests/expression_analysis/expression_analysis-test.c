#include "./../../src/expression_analysis.h"
#include "./../../src/scanner.h"
#include "expression_analysis-test.h"

int main(int argc, char const *argv[]) {
    Expression expression;
    int result = parseExpression(&expression);
    return result;
}

