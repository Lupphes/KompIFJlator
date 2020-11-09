#include "./../../src/scanner.h"
#include "scanner-test.h"

int main(int argc, char const *argv[]) {
    Token token;
    int result;
    while (token.type != TokenEOF)
    {
        if((result = getToken(&token)) == 0) {
            printf("%s\n", enumTokenTranslate[token.type]);
        } else {
            printf("%d\n", result);
            return result;
        }
    }
    return 0;
}
