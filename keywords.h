#include <token.h>

typedef struct {
    char* key;
    _TokenType value;
} keywordEntry;


const keywordEntry keywords[] = {
    {
    "else", TokenElse
    },
    {
    "float64", TokenFloat64
    },
    {
    "for", TokenFor
    },
    {
    "func", TokenFunc
    },
    {
    "if", TokenIf
    },
    {
    "int", TokenInt
    },
    {
    "package", TokenPackage
    }    
};