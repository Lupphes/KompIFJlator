#include "./../../src/scanner.h"
#include "scanner-test.h"

int main() {
    Token token;
    int result;
    result = getToken(&token);
    printf("%s", enumTokenTranslate[token.type]);
    if (token.type == 25 || token.type == 0) { // Laze free
        strFree(&token.attribute.s); 
    }
    return result;
}

