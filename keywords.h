/*File name: keywords.h ---------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Ondřej Sloup (xsloup02)                                |
 |            Viktor Rucký (xrucky01)                                |
 |                                                                   |
 |  _      _     _   __                   __  _        _             |
 | | |    (_)   | | /_/                  /_/ | |      | |            |
 | | |     _  __| | ___   _   _  __   ___   _| |_ __ _| |__  _   _   |
 | | |    | |/ _` |/ _ \ | | | | \ \ / / | | | __/ _` | '_ \| | | |  |
 | | |____| | (_| |  __/ | |_| |  \ V /| |_| | || (_| | | | | |_| |  |
 | |______|_|\__,_|\___|  \__,_|   \_/  \__, |\__\__,_|_| |_|\__,_|  |
 |                                       __/ |                       |
 |                                      |___/                        |
 *-------------------------------------------------------------------*/

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