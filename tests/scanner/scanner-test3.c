#include "./../../src/scanner.h"
#include "scanner-test.h"

static const char* enumDataTypeTranslate[] = {
  "TypeInt",
  "TypeFloat64",
  "TypeString"
};

int main(int argc, char const *argv[]) {
    Token token;
    int result;
    result = getToken(&token);
    if (token.type == TokenWholeNbr) {
        printf("%.9ld", token.attribute.i);
    } else if (token.type == TokenDecimalNbr) {
        printf("%.9lf", token.attribute.d);
    } else if (token.type == TokenStringLiteral) {
        printf("%s", strGetStr(&token.attribute.s));
        strFree(&token.attribute.s);
    } else if (token.type == TokenIdentifier) {
        printf("%s", strGetStr(&token.attribute.s));
        strFree(&token.attribute.s);
    } else if (token.type == TokenDataType) {
        printf("%s", enumDataTypeTranslate[token.attribute.t]);
    } else {
        printf("%s", enumTokenTranslate[token.type]);
    }
    
    return result;
}

