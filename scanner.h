/*File name: scanner.h ----------------------------------------------*
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
#ifndef SCANNER_H
#define SCANNER_H
#include <stdint.h>
#include "str.h"
#include "token.h"


typedef enum {
    SUCCESS = 0,
    LEXICAL_ERROR,
    INPUT_ERROR,
    UNEXPECTED_EOF_ERROR,
    MEMORY_ERROR = 99
} ErrorGetToken;

typedef enum {
    /* Start */
    StateStart,
    /* String */
    StateStartOfString,
    /* Comments */
    StateSlash,
    /* White spaces */
    /* Identifier */
    StateIdentifier,
    /* Conditions */
    StateEquals,
    StateExclamationMark, // Feature
    StateIsGreaterThan,
    StateIsLessThan,
    /* Conditional operators -- feature */
    StateAndFirst,
    // StateAndSecond,
    StateOrFirst,
    // StateOrSecond, 
    /* Numbers */
    StateWholeNbr,
    StateZero,
    // StateCompleteBaseNbr, // Feature
    StateIncompletedecimalNbr,
    StateCompleteDecimalNbr,
    StateUnsignedExpoNbr,
    StateCompleteUnsignedExpoNbr,
    StateSemiCompleteExpoNbr,
    StateCompleteExpoNbr
} ScannerState;

int getCharCheck(int *value);
int unGetCharCheck(int value);
int getToken(Token* token);
_TokenType processIdentifier(); // Compare strings and return tokens

#endif