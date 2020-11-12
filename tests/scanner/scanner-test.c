#include "./../../src/scanner.h"
#include "scanner-test.h"

int main(int argc, char const *argv[]) {
    Token token;
    int result;
    result = getToken(&token);
    printf("%s", enumTokenTranslate[token.type]);
    return result;
}

