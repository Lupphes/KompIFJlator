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
    /* Start */
    StateStart,
    /* String */
    StateStartOfString,
    /* Comments */
    StateSlash,
    StateColon,
    /* White spaces */
    /* Identifier */
    StateIdentifier,
    /* Conditions */
    StateEquals,
    StateExclamationMark,
    StateIsGreaterThan,
    StateIsLessThan,
    /* Feature UNARY */
    StateAddEqual,
    StateSubtractEqual,
    StateMultiplyEqual,
    /* Feature BASE */
    StateCompleteHexBaseNumber,
    StateCompleteOctalBaseNumber,
    StateCompleteBinaryBaseNumber,
    StateIncompleteHexBaseNumber,
    StateIncompleteOctalBaseNumber,
    StateIncompleteBinaryBaseNumber,
    StateUnderlineHex,
    StateUnderlineOctal,
    StateUnderlineBinary,
    /* Numbers */
    StateWholeNbr,
    StateZero,
    // StateCompleteBaseNbr, // Feature
    StateIncompletedecimalNbr,
    StateCompleteDecimalNbr,
    StateIncompleteUnsignedExpoNbr,
    StateIncompleteSignedExpoNbr,
    StateCompleteExpoNbr
} ScannerState;

int getCharCheck(int *value);
int unGetCharCheck(int value);
int getToken(Token* token);
_TokenType processIdentifier(); // Compare strings and return tokens

#endif