#include "scanner.h"
#include "scanner-test.h"

int main(int argc, char const *argv[]) {
    Token token;
    int result;
    while (token.type != TokenEOF)
    {
        if((result = getToken(&token)) == 0) {
            printf("%s", token);
        } else {
            printf("%d", result);
            return result;
        }
    }
    return 0;
}
