/*File name: scanner.c ----------------------------------------------*
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
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h> 
#include <string.h>

#include "scanner.h"
#include "keywords.h"
#include "str.h"
#include "error.h"

/*!
 *  Macro function which validates if the funtion result was 0, If not release buffer and exit
 */ 
#define charMacro(func, char) if ((macroError = func(char)) != 0) { \
                                       if (bufferStringInitialised) { \
                                           strFree(&bufferString); \
                                       } \
                                       return macroError; } \

/*!
 *  Macro function which initializes buffer and sets flag.
 */ 
#define initStringMacro(string) if(strInit(string) == STR_ERROR) { return INTERNAL_ERROR; } \
                                bufferStringInitialised = true; \

/*!
 *  End of the line is more readable now
 */                       
#define EOL '\n'

int getCharCheck(int *value) {
    *value = getchar();
    if (feof(stdin))
        *value = EOF;
    else if (ferror(stdin))
        return INTERNAL_ERROR;
    return SUCCESS;
}

int unGetCharCheck(int value) {
    if (value == EOF)
       return SUCCESS;
    value = ungetc(value, stdin);
    if (value == EOF)
       return INTERNAL_ERROR;
    return SUCCESS;
}

int getToken(Token* token) {
    _ScannerState state = StateStart;
    string bufferString;
    bool bufferStringInitialised = false;
    int currChar, savedChar, macroError;
    char hexadecimalValue[3];
    token->type = TokenUndefined;
    while (token->type == TokenUndefined) {
        charMacro(getCharCheck, &currChar);
        switch (state) {
        case StateStart:
            switch (currChar) {
            case ',':
                token->type = TokenComma;
                return SUCCESS;
                break;
            case ';':
                token->type = TokenSemicolon;
                return SUCCESS;
                break;
            case '(':
                token->type = TokenLeftBracket;
                return SUCCESS;
                break;
            case ')':
                token->type = TokenRightBracket;
                return SUCCESS;
                break;
            case '{':
                token->type = TokenLeftCurlyBracket;
                return SUCCESS;
                break;
            case '}':
                token->type = TokenRightCurlyBracket;
                return SUCCESS;
                break;
            case '/':
                state = StateSlash;
                break;
            case '"':
                state = StateStartOfString;
                break;
            case ' ' :
            case '\t' :
                while (currChar == ' ' || currChar == '\t')
                    charMacro(getCharCheck, &currChar);
                charMacro(unGetCharCheck, currChar);
                state = StateStart;
                break;
            case EOL :
                token->type = TokenEOL;
                return SUCCESS;
                break;
            case EOF :
                token->type = TokenEOF;
                return SUCCESS;
                break;
            case '=':
                state = StateEquals;
                break;
            case ':':
                state = StateColon;
                break;
            case '!':
                state = StateExclamationMark;
                break;
            case '>':
                state = StateIsGreaterThan;
                break;
            case '<':
                state = StateIsLessThan;
                break;
            case '*':
                state = StateMultiplyEqual;
                break;
            case '+':
                state = StateAddEqual;
                break;
            case '-':
                state = StateSubtractEqual;
                break;
            case '0':
                savedChar = currChar;
                state = StateZero;
                break;
            default:
                if (isdigit(currChar)) { // Zero is already handled
                    savedChar = currChar;
                    initStringMacro(&bufferString)
                    if (strAddChar(&bufferString, savedChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    state = StateWholeNbr;
                } else if (isalnum(currChar) || currChar == '_') {
                    savedChar = currChar;
                    state = StateIdentifier;
                } else {
                    return LEXICAL_ERROR;
                }
                break;
            }
            break;
        case StateAddEqual:
            switch (currChar) {
            case '=':
                token->type = TokenAddEqual;
                return SUCCESS;
                break;
            default:
                charMacro(unGetCharCheck, currChar);
                token->type = TokenAdd;
                return SUCCESS;
                break;
            }
        break;
        case StateSubtractEqual:
            switch (currChar) {
            case '=':
                token->type = TokenSubtractEqual;
                return SUCCESS;
                break;
            default:
                charMacro(unGetCharCheck, currChar);
                token->type = TokenSubtract;
                return SUCCESS;
                break;
            }
        break;
        case StateMultiplyEqual:
            switch (currChar) {
            case '=':
                token->type = TokenMultiplyEqual;
                return SUCCESS;
                break;
            default:
                charMacro(unGetCharCheck, currChar);
                token->type = TokenMultiply;
                return SUCCESS;
                break;
            }
        break;
        case StateSlash:
            switch (currChar) {
            case '/':
                do {
                    charMacro(getCharCheck, &currChar);
                } while (currChar != EOL && currChar != EOF);
                charMacro(unGetCharCheck, currChar);
                state = StateStart;
                break;
            case '*':
                charMacro(getCharCheck, &currChar);
                int nextChar;
                charMacro(getCharCheck, &nextChar);
                while (!(currChar == '*' && nextChar == '/')) {
                    currChar = nextChar;
                    charMacro(getCharCheck, &nextChar);
                    if (nextChar == EOF)
                        return LEXICAL_ERROR;
                    else if (nextChar == EOL)
                        token->type = TokenEOL;
                }
                if (token->type == TokenEOL)
                    return SUCCESS;
                else
                    state = StateStart;
                break;
            case '=':
                token->type = TokenDivideEqual;
                return SUCCESS;
                break;
            break;
            default:
                charMacro(unGetCharCheck, currChar);
                token->type = TokenDivide;
                return SUCCESS;
                break;
            }
            break;
        case StateStartOfString:
            initStringMacro(&bufferString)
            while(currChar != '"') {
                if (currChar == '\\') {
                    charMacro(getCharCheck, &currChar);
                    switch (currChar) {
                    case '"':
                        if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                            strFree(&bufferString);
                            return INTERNAL_ERROR;
                        }
                        break;
                    case 't':
                        if (strAddChar(&bufferString, '\t') == STR_ERROR) {
                            strFree(&bufferString);
                            return INTERNAL_ERROR;
                        }
                        break;
                    case 'n':
                        if (strAddChar(&bufferString, '\n') == STR_ERROR) { // not tested
                            strFree(&bufferString);
                            return INTERNAL_ERROR;
                        }
                        break;
                    case '\\':
                        if (strAddChar(&bufferString, '\\') == STR_ERROR) {
                            strFree(&bufferString);
                            return INTERNAL_ERROR;
                        }
                        break;
                    case 'x':
                        charMacro(getCharCheck, &currChar);
                        charMacro(getCharCheck, &savedChar);
                        if (!(isxdigit(currChar) && isxdigit(savedChar))){
                            strFree(&bufferString);
                            return LEXICAL_ERROR;
                        }
                        hexadecimalValue[0] = currChar;
                        hexadecimalValue[1] = savedChar;
                        hexadecimalValue[2] = '\0';
                        long number = strtol(hexadecimalValue, NULL, 16);
                        if (strAddChar(&bufferString, (char)number) == STR_ERROR) {
                            strFree(&bufferString);
                            return INTERNAL_ERROR;
                        }
                        break;
                    default:
                        strFree(&bufferString);
                        return LEXICAL_ERROR;
                        break;
                    }
                    charMacro(getCharCheck, &currChar); // test this
                }
                else if (currChar > 31) { // " -- Handled in previous branch
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    charMacro(getCharCheck, &currChar);
                    continue;
                } else if (currChar == EOL || currChar == EOF) {
                    strFree(&bufferString);
                    return LEXICAL_ERROR;
                } 
                else {
                    strFree(&bufferString);
                    return LEXICAL_ERROR;    
                }
            }
            token->type = TokenStringLiteral;
            initStringMacro(&token->attribute.s)
            if (strCopyString(&token->attribute.s, &bufferString) == STR_ERROR) {
                strFree(&bufferString);
                return INTERNAL_ERROR;
            }
            strFree(&bufferString);
            return SUCCESS;
            break;
        case StateEquals:
            switch (currChar) {
            case '=':
                token->type = TokenIsEqual;
                return SUCCESS;
                break;
            default:
                charMacro(unGetCharCheck, currChar);
                token->type = TokenAssignment;
                return SUCCESS;
                break;
            }
        case StateColon:
            switch (currChar) {
            case '=':
                token->type = TokenVarDefine;
                return SUCCESS;
            default:
                return LEXICAL_ERROR;
                break;
            }
        case StateExclamationMark:
            switch (currChar) {
            case '=':
                token->type = TokenNotEqual;
                return SUCCESS;
                break;
            default:
                return LEXICAL_ERROR;
                break;
            }
        case StateIsGreaterThan:
            switch (currChar) {
            case '=':
                token->type = TokenIsGreaterEqual;
                return SUCCESS;
                break;
            default:
                charMacro(unGetCharCheck, currChar);
                token->type = TokenIsGreaterThan;
                return SUCCESS;
                break;
            }
        case StateIsLessThan:
            switch (currChar) {
            case '=':
                token->type = TokenIsLessEqual;
                return SUCCESS;
                break;
            default:
                charMacro(unGetCharCheck, currChar);
                token->type = TokenIsLessThan;
                return SUCCESS;
                break;
            }
        case StateIdentifier:
            initStringMacro(&bufferString);
            if (strAddChar(&bufferString, savedChar) == STR_ERROR) { // makro
                strFree(&bufferString);
                return INTERNAL_ERROR;
            }
            while (isalnum(currChar) || currChar == '_') {
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                charMacro(getCharCheck, &currChar);       
            } 
            charMacro(unGetCharCheck, currChar);
            for (int i = 0; i < sizeof(_keywords)/sizeof(keywordEntry); i++) {
                if(strCmpConstStr(&bufferString, _keywords[i].key) == 0) {
                    *token = _keywords[i].value;
                    strFree(&bufferString);
                    return SUCCESS;
                    break;
                }
            }
            token->type = TokenIdentifier;
            initStringMacro(&token->attribute.s)
            if (strCopyString(&token->attribute.s, &bufferString) == STR_ERROR) {
                strFree(&bufferString);
                return INTERNAL_ERROR;
                
            }
            strFree(&bufferString);
            return SUCCESS;
            break;
        case StateZero:
            initStringMacro(&bufferString)
            if (strAddChar(&bufferString, savedChar) == STR_ERROR) {
                strFree(&bufferString);
                return INTERNAL_ERROR;
            }
            switch (currChar) {
            case '.':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateIncompletedecimalNbr;
                break;
            case 'e':
            case 'E':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateIncompleteUnsignedExpoNbr;
                break;
            case 'x':
            case 'X':
                if (strAddChar(&bufferString, tolower(currChar)) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateIncompleteHexBaseNumber;
                break;
            case 'o':
            case 'O':
                state = StateIncompleteOctalBaseNumber;
                break;
            case 'b':
            case 'B':
                state = StateIncompleteBinaryBaseNumber;
                break;
            default:
                if (isdigit(currChar)) { // Removed condition if handled by parser
                    strFree(&bufferString);
                    return LEXICAL_ERROR;
                }
                charMacro(unGetCharCheck, currChar);
                if (sscanf(strGetStr(&bufferString),"%"SCNd64,&token->attribute.i) == EOF) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                token->type = TokenWholeNbr;
                strFree(&bufferString);
                return SUCCESS;
            }
            break;
        case StateIncompletedecimalNbr:
            state = StateCompleteDecimalNbr;
            if (isdigit(currChar)) {
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateCompleteDecimalNbr;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
                break;
            }
            break;
        case StateCompleteDecimalNbr:
            state = StateCompleteDecimalNbr;
            switch (currChar) {
            case 'e':
            case 'E':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateIncompleteUnsignedExpoNbr;
                break;
            case '_':
                state = StateUnderlineDecimal;
                break;
            default:
                if (isdigit(currChar)) {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    break;
                } else {
                    if (sscanf(strGetStr(&bufferString),"%lf", &token->attribute.d) == EOF) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    token->type = TokenDecimalNbr;
                    strFree(&bufferString);
                    return SUCCESS;
                    break;
                }
            }
            break;
        case StateIncompleteUnsignedExpoNbr:
            switch (currChar) {
            case '+':
            case '-':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateIncompleteSignedExpoNbr;
                break;
            default:
                if (isdigit(currChar)) {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    state = StateCompleteExpoNbr;
                    break;
                } else {
                    strFree(&bufferString);
                    return LEXICAL_ERROR;
                }
                break;
            }
            break;
        case StateIncompleteSignedExpoNbr:
            if (isdigit(currChar)) {
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateCompleteExpoNbr;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
            }
            break;
        case StateCompleteExpoNbr:
            switch (currChar) {
            case '_':
                state = StateUnderlineExpo;
                break;
            default:
                if (isdigit(currChar)) {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    break;
                    state = StateCompleteExpoNbr;
                } else {
                    if (sscanf(strGetStr(&bufferString),"%lf", &token->attribute.d) == EOF) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    token->type = TokenDecimalNbr;
                    strFree(&bufferString);
                    return SUCCESS;
                }
                break;
            }
        break;
        case StateWholeNbr:
            state = StateWholeNbr;
            switch (currChar) {
            case '.':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateIncompletedecimalNbr;
                break;
            case 'e':
            case 'E':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateIncompleteUnsignedExpoNbr;
                break;
            case '_':
                state = StateUnderlineWhole;
                break;
            default:
                if (isdigit(currChar)) {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    break;
                } else {
                    charMacro(unGetCharCheck, currChar);
                    if (sscanf(strGetStr(&bufferString),"%"SCNd64,&token->attribute.i) == EOF) {
                        strFree(&bufferString);
                        return INTERNAL_ERROR;
                    }
                    token->type = TokenWholeNbr;
                    strFree(&bufferString);
                    return SUCCESS;
                }
            break;
            }
        break;
        case StateIncompleteHexBaseNumber:
            switch (currChar) {
                case '_':
                    state = StateUnderlineHex;
                    break;
                default:
                    if (isxdigit(currChar)) {
                        if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                            strFree(&bufferString);
                            return INTERNAL_ERROR;
                        }
                        state = StateCompleteHexBaseNumber;
                    } else {
                        strFree(&bufferString);
                        return LEXICAL_ERROR;
                    }
                break;
            }
        break;
        case StateCompleteHexBaseNumber:
            if (isxdigit(currChar)) {
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateCompleteHexBaseNumber;
            } else if (currChar == '_') {
                state = StateUnderlineHex;
            } else {
                charMacro(unGetCharCheck, currChar);
                errno = 0;
                token->attribute.i = strtoull(strGetStr(&bufferString), NULL, 16);
                if (errno == ERANGE) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                token->type = TokenWholeNbr;
                strFree(&bufferString);
                return SUCCESS;
            }
        break;
        case StateIncompleteOctalBaseNumber:
            switch (currChar) {
                case '_':
                    state = StateUnderlineOctal;
                    break;
                default:
                    if (currChar >= '0' && currChar <= '7') {
                        if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                            strFree(&bufferString);
                            return INTERNAL_ERROR;
                        }
                        state = StateCompleteOctalBaseNumber;
                    } else {
                        strFree(&bufferString);
                        return LEXICAL_ERROR;
                    }
                    break;
            }
        break;
        case StateCompleteOctalBaseNumber:
            if (currChar >= '0' && currChar <= '7') {
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateCompleteOctalBaseNumber;
            } else if (currChar == '_') {
                state = StateUnderlineOctal;
            } else {
                charMacro(unGetCharCheck, currChar);
                errno = 0;
                token->attribute.i = strtoull(strGetStr(&bufferString), NULL, 8);
                if (errno == ERANGE) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                token->type = TokenWholeNbr;
                strFree(&bufferString);
                return SUCCESS;
                break;
            }
        break;
        case StateIncompleteBinaryBaseNumber:
            switch (currChar) {
                case '_':
                    state = StateUnderlineBinary;
                    break;
                default:
                    if (currChar >= '0' && currChar <= '7') {
                        if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                            strFree(&bufferString);
                            return INTERNAL_ERROR;
                        }
                        state = StateCompleteBinaryBaseNumber;
                    } else {
                        strFree(&bufferString);
                        return LEXICAL_ERROR;
                    }
                    break;
            }
        break;
        case StateCompleteBinaryBaseNumber:
            if (currChar == '1' || currChar == '0') {
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return INTERNAL_ERROR;
                }
                state = StateCompleteBinaryBaseNumber;
            } else if (currChar == '_') {
                state = StateUnderlineBinary;
                break;
            } else {
                charMacro(unGetCharCheck, currChar);
                const char* formatedString = strGetStr(&bufferString);
                token->attribute.i = strtoull(formatedString+1, NULL, 2);
                token->type = TokenWholeNbr;
                strFree(&bufferString);
                return SUCCESS;
            }
        break;
        case StateUnderlineHex:
            if (isxdigit(currChar)) {
                charMacro(unGetCharCheck, currChar);
                state = StateCompleteHexBaseNumber;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
            }
        break;
        case StateUnderlineOctal:
            if (currChar >= '0' && currChar <= '7') {
                charMacro(unGetCharCheck, currChar);
                state = StateIncompleteOctalBaseNumber;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
            }
        break;
        case StateUnderlineBinary:
            if (currChar == '1' || currChar == '0') {
                charMacro(unGetCharCheck, currChar);
                state = StateIncompleteBinaryBaseNumber;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
            }
        break;
        case StateUnderlineWhole:
            if (isdigit(currChar)) {
                charMacro(unGetCharCheck, currChar);
                state = StateWholeNbr;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
            }
        break;
        case StateUnderlineDecimal:
            if (isdigit(currChar)) {
                charMacro(unGetCharCheck, currChar);
                state = StateCompleteDecimalNbr;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
            }
        break;
        case StateUnderlineExpo:
            if (isdigit(currChar)) {
                charMacro(unGetCharCheck, currChar);
                state = StateCompleteExpoNbr;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
            }
            break;
        }
    }
    return LEXICAL_ERROR; // This line will never execute. make_iso_compilers_happy
}