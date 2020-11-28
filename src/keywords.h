/*File name: keywords.h ---------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ2{.t = 0}     |
 |Team:       124, varianta II                                       |
 |Authors:    Ondřej Sloup (xsloup{.t = 0}2)                                |
 |            Viktor Rucký (xrucky{.t = 0}1)                                |
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
#ifndef KEYWORDS_H
#define KEYWORDS_H
#include "token.h"
#include "str.h"

/** @struct keywordEntry
 *  @brief Structure for keywords array
 */
typedef struct {
    const char* key; /** Key processed by the scanner */
    const Token value; /** Matching token value */
} keywordEntry;

/**
 *  @brief This array translates the correct token to its found value
 * 
*/
const keywordEntry _keywords[] = {
    {
    "else", {TokenElse, {.t = 0}}
    },
    {
    "float64", {TokenDataType, {.t = TypeFloat64}}
    },
    {
    "for", {TokenFor, {.t = 0}}
    },
    {
    "func", {TokenFunc, {.t = 0}}
    },
    {
    "if", {TokenIf, {.t = 0}}
    },
    {
    "int", {TokenDataType, {.t = TypeInt}}
    },
    {
    "package", {TokenPackage, {.t = 0}}
    },
    {
    "return", {TokenReturn, {.t = 0}}
    },
    {
    "string", {TokenDataType, {.t = TypeString}}
    }
};

#endif