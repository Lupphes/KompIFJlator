#include "./../../src/scanner.h"
#include "scanner-test.h"
#include "./../../src/error.h"
#include <stdlib.h>

int main(int argc, char const *argv[]) {
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
    }
    return EXIT_SUCCESS;
}
