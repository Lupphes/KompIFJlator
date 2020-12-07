#include "./../../src/expression_analysis.h"
#include "./../../src/scanner.h"
#include "expression_analysis-test.h"
#include "./../../src/parser_common.h"
#include "./../../src/symtable.h"
#include "./../../src/parser.h"

#include <stdio.h>

int main() {
    int returnCode = 0;
    ExpExp *expression;
    OperatorAssign assingmentOperation = 1;
    string ahoj = {.allocSize = 2,.length = 2,.str = "ah"};
    SymbolVariable symbol = {.id = ahoj,.type = TypeInt,.uid = 1505};
    SymbolVariable *haha = &symbol;
    initVariableTableStack();
    acceptAny();
    int result = parseExpression(&expression, assingmentOperation, haha);
    printf("Return code is: %d\n", result);
    return result;
}