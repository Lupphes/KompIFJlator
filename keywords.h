#include <token.h>

typedef struct {
    const char* key;
    const Token value;
} keywordEntry;


const keywordEntry keywords[] = {
    {
    "else", {TokenElse}
    },
    {
    "float64", {TokenDataType, {.t = TypeFloat64}}
    },
    {
    "for", {TokenFor}
    },
    {
    "func", {TokenFunc}
    },
    {
    "if", {TokenIf}
    },
    {
    "int", {TokenDataType, {.t = TypeInt}}
    },
    {
    "package", {TokenPackage}
    },
    {
    "string", {TokenDataType, {.t = TypeString}}
    }    
};