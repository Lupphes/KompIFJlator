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

/**
 * @enum ScannerState
 * @brief Describes all states in which scanner can be.
 *
 */
typedef enum {
    StateStart, /** Initial state of the scanner */
    StateStartOfString, /** Start of string literal */
    StateSlash, /** The scanner loaded '/' */
    StateColon, /** The scanner loaded ':' */
    StateIdentifier, /** The scanner is reading an identifier */
    /* Conditions */
    StateEquals, /** The scanner loaded '=' */
    StateExclamationMark,  /** The scanner loaded '!' */
    StateIsGreaterThan, /** The scanner loaded '>' */
    StateIsLessThan, /** The scanner loaded '<' */
    /* Feature UNARY */
    StateAddEqual, /** The scanner loaded '+' */
    StateSubtractEqual, /** The scanner loaded '-' */
    StateMultiplyEqual, /** The scanner loaded '*' */
    /* Feature BASE */
    StateCompleteHexBaseNumber, /** The scanner found valid hex number */
    StateCompleteOctalBaseNumber, /** The scanner found valid octal number */
    StateCompleteBinaryBaseNumber, /** The scanner found valid binary number */
    StateIncompleteHexBaseNumber,  /** The scanner found number with prefix '0x' or 0X */
    StateIncompleteOctalBaseNumber, /** The scanner found number with prefix '0o' */
    StateIncompleteBinaryBaseNumber, /** The scanner found number with prefix '0b' or '0B' */
    StateUnderlineHex, /** The scanner is processing underline to a hex number */
    StateUnderlineOctal, /** The scanner is processing underline to a octal number */
    StateUnderlineBinary, /** The scanner is processing underline to a binary number */
    StateUnderlineDecimal, /** The scanner is processing underline to a decimal number */
    StateUnderlineWhole, /** The scanner is processing underline to a whole number */
    StateUnderlineExpo, /** The scanner is processing underline to a exponential number */
    /* Numbers */
    StateWholeNbr, /** The scanner loaded whole number, but not zero */
    StateZero,  /** The scanner loaded zero */
    StateIncompletedecimalNbr, /** The scanner loaded '.' therefore the number will be decimal */
    StateCompleteDecimalNbr, /** The scanner loaded a returnable decimal number */
    StateIncompleteUnsignedExpoNbr, /** The scanner loaded 'E'. Waiting for next number */
    StateIncompleteSignedExpoNbr, /** The scanner loaded '-' '+'. Waiting for next number */
    StateCompleteExpoNbr /** The scanner loaded a returnable exponential number */
} ScannerState;

/**
 * @brief Checks if getchar() function was successfully called
 * 
 * @param[out] value The character (int) value to which the character will be returned
 * @return int SUCCESS Returned if getchar() did not fail
 * @return int INTERNAL_ERROR Returned if there was a getchar() error
 * @warning This function returns EOF not as an error, but as a feature
 * 
 * @author Ondřej Sloup xsloup02
 * @version 1.0
 * @date 17.11.2020
 */
int getCharCheck(int *value);

/**
 * @brief Checks if ungetc() function was successfully called
 * 
 * @param[in] value The character (int) which should be returned to stdin
 * @return int SUCCESS Returned if ungetc() did not fail
 * @return int INTERNAL_ERROR Returned if there was an ungetc() error
 * @warning Unlike ungetc(), unGetCharCheck() returns characters only to stdin
 * 
 * @author Ondřej Sloup xsloup02
 * @version 1.0
 * @date 17.11.2020
 */
int unGetCharCheck(int value);

/**
 * @brief Returns a token from stdin when called
 * @details The main function of the scanner, which analyzes the input from stdin
 * and returns the corresponding token.
 * 
 * @pre Initialize token variable before calling this function
 * @param[out] token.type Token type is returned here. Additional values
 * are returned into attributes.
 * @param[out] token->attribute.i A returned value of token if the token is 'TokenWholeNbr'
 * @param[out] token->attribute.d A returned value of token if the token is 'TokenDecimalNbr'.
 * @param[out] token->attribute.s A returned value of token if the token is 'TokenIdentifier',
 * 'TokenStringLiteral'.
 * @param[out] token->attribute.t A returned value of token if the token is 'TokenDataType',
 * which identify the dataType. Possible values are 'TypeInt', 'TypeFloat64', 'TypeString'.
 * @return int SUCCESS Returned if getToken() successfully returned the value
 * @return int LEXICAL_ERROR Returned if getToken() found a lexical error
 * @return int INTERNAL_ERROR Returned if getToken() ended unexpectedly
 * 
 * @author Ondřej Sloup xsloup02
 * @version 1.0
 * @date 17.11.2020
 */
int getToken(Token* token);

#endif