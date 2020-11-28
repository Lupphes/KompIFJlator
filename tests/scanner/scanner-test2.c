#include "./../../src/scanner.h"
#include "scanner-test.h"
#include "./../../src/error.h"
#include <stdlib.h>

int main() {
    Token token;
    int result;
    while (token.type != TokenEOF)
    {
        if((result = getToken(&token)) == SUCCESS) {
            printf("%s\n", enumTokenTranslate[token.type]);
        } else {
            printf("%d\n", result);
            return result;
        }
        if (token.type == 25 || token.type == 0) { // Laze free
            strFree(&token.attribute.s); 
        }
    }
    return EXIT_SUCCESS;
}
