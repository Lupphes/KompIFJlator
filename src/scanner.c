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

#include "scanner.h"
#include "keywords.h"
#include "str.h"
#define charMacro(func, char) if ((macroError = func(char)) != 0) { \
                                       if (&bufferString != NULL) { \
                                           strFree(&bufferString); \
                                       } \
                                       return macroError; }

#define EOL '\n'

int getCharCheck(int *value) {
    *value = getchar();
    if (feof(stdin)) {
        *value = EOF;
    }
    else if (ferror(stdin)) {
        return INPUT_ERROR;
    }
    return SUCCESS;
}

int unGetCharCheck(int value) {
    value = ungetc(value, stdin);
    return SUCCESS;
}

int getToken(Token* token) {
    ScannerState state = StateStart;
    string bufferString;
    int currChar, previousChar, macroError;
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
                while (currChar == EOL)
                    charMacro(getCharCheck, &currChar)
                charMacro(unGetCharCheck, currChar);
                state = StateStart;
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
                token->type = TokenMultiply;
                return SUCCESS;
                break;
            case '+':
                token->type = TokenAdd;
                return SUCCESS;
                break;
            case '-':
                token->type = TokenSubtract;
                return SUCCESS;
                break;
            case '0':
                previousChar = currChar;
                state = StateZero;
                break;
            default:
                if (isdigit(currChar)) {
                    previousChar = currChar;
                    state = StateWholeNbr;
                    if(strInit(&bufferString) == STR_ERROR)
                        return MEMORY_ERROR;
                    if (strAddChar(&bufferString, previousChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return MEMORY_ERROR;
                    }
                } else if (isalnum(currChar) || currChar == '_') {
                    previousChar = currChar;
                    state = StateIdentifier;
                } else {
                    strFree(&bufferString);
                    return LEXICAL_ERROR;
                }
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
                }
                state = StateStart;
                break;
            default:
                token->type = TokenDivide;
                return SUCCESS;
                break;
            }
            break;
        case StateStartOfString:
            if(strInit(&bufferString) == STR_ERROR)
                return MEMORY_ERROR;
            while(currChar != '"') {
                if (currChar == '\\') {
                    charMacro(getCharCheck, &currChar);
                    switch (currChar) {
                    case '"':
                        if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                            strFree(&bufferString);
                            return MEMORY_ERROR;
                        }
                        break;
                    case 't':
                        if (strAddChar(&bufferString, '\t') == STR_ERROR) {
                            strFree(&bufferString);
                            return MEMORY_ERROR;
                        }
                        break;
                    case 'n':
                        if (strAddChar(&bufferString, '\0') == STR_ERROR) {
                            strFree(&bufferString);
                            return MEMORY_ERROR;
                        }
                        break;
                    case '\\':
                        if (strAddChar(&bufferString, '\\') == STR_ERROR) {
                            strFree(&bufferString);
                            return MEMORY_ERROR;
                        }
                        break;
                    case 'x':
                        charMacro(getCharCheck, &currChar);
                        charMacro(getCharCheck, &previousChar);
                        if (!(isxdigit(currChar) && isxdigit(previousChar))){
                            strFree(&bufferString);
                            return LEXICAL_ERROR;
                        }
                        hexadecimalValue[0] = currChar;
                        hexadecimalValue[1] = previousChar;
                        errno = 0;
                        long number = strtol(hexadecimalValue, NULL, 16);
                        if ((number == LONG_MAX || number == LONG_MIN) && errno == ERANGE) {
                            strFree(&bufferString);
                            return LEXICAL_ERROR;
                        }
                        if (strAddChar(&bufferString, (char)number) == STR_ERROR) {
                            strFree(&bufferString);
                            return MEMORY_ERROR;
                        }
                        break;
                    default:
                        strFree(&bufferString);
                        return LEXICAL_ERROR;
                        break;
                    }
                    charMacro(getCharCheck, &currChar);
                }
                else if (currChar > '"' || currChar == '!' || currChar == ' ') {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return MEMORY_ERROR;
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
            };
            token->type = TokenStringLiteral;
            token->atribute.t = TokenStringLiteral;
            if(strInit(&token->atribute.s) == STR_ERROR)
                return MEMORY_ERROR;
            if (strCopyString(&token->atribute.s, &bufferString) == STR_ERROR)
                return MEMORY_ERROR;
            return SUCCESS;
            break;
        case StateEquals:
            switch (currChar) {
            case '=':
                token->type = TokenIsEqual;
                return SUCCESS;
                break;
            default:
                token->type = TokenEquals;
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
                token->type = TokenEquals;
                return SUCCESS;
                break;
            }
        case StateIsGreaterThan:
            switch (currChar) {
            case '=':
                token->type = TokenIsLessEqual;
                return SUCCESS;
                break;
            default:
                token->type = TokenIsGreaterThan;
                return SUCCESS;
                break;
            }
        case StateIsLessThan:
            switch (currChar) {
            case '=':
                token->type = TokenIsGreaterEqual;
                return SUCCESS;
                break;
            default:
                token->type = TokenIsLessThan;
                return SUCCESS;
                break;
            }
        case StateIdentifier:
            if(strInit(&bufferString) == STR_ERROR)
                return MEMORY_ERROR;
            if (strAddChar(&bufferString, previousChar) == STR_ERROR) {
                strFree(&bufferString);
                return MEMORY_ERROR;
            }
            while (isalnum(currChar) || currChar == '_') {
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return MEMORY_ERROR;
                }
                charMacro(getCharCheck, &currChar);       
            } 
            charMacro(unGetCharCheck, currChar);
            for (int i = 0; i < sizeof(keywords)/sizeof(keywordEntry); i++) {
                if(strCmpConstStr(&bufferString, keywords[i].key) == 0) {
                    *token = keywords[i].value;
                    strFree(&bufferString);
                    return SUCCESS;
                    break;
                }
            }
            token->type = TokenIdentifier;
            if(strInit(&token->atribute.s) == STR_ERROR)
                return MEMORY_ERROR;
            if (strCopyString(&token->atribute.s, &bufferString) == STR_ERROR)
                return MEMORY_ERROR;
            return SUCCESS;
            break;
        case StateZero:
            if(strInit(&bufferString) == STR_ERROR)
                return MEMORY_ERROR;
            if (strAddChar(&bufferString, previousChar) == STR_ERROR) {
                strFree(&bufferString);
                return MEMORY_ERROR;
            }
            switch (currChar) {
            case '.':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return MEMORY_ERROR;
                }
                state = StateIncompletedecimalNbr;
                break;
            case 'e':
            case 'E':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return MEMORY_ERROR;
                }
                state = StateUnsignedExpoNbr;
                break;
            default:
                charMacro(unGetCharCheck, currChar);
                if (sscanf(strGetStr(&bufferString),"%"SCNd64,&token->atribute.i) == EOF) {
                    strFree(&bufferString);
                    return CONVERSION_ERROR;
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
                    return MEMORY_ERROR;
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
                    return MEMORY_ERROR;
                }
                state = StateUnsignedExpoNbr;
                break;
            default:
                if (isdigit(currChar)) {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return MEMORY_ERROR;
                    }
                    break;
                } else {
                    if (sscanf(strGetStr(&bufferString),"%lf", &token->atribute.d) == EOF) {
                        strFree(&bufferString);
                        return CONVERSION_ERROR;
                    }
                    token->type = TokenDecimalNbr;
                    strFree(&bufferString);
                    return SUCCESS;
                    break;
                }
            }
            break;
        case StateUnsignedExpoNbr:
            switch (currChar)
            {
            case '+':
            case '-':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return MEMORY_ERROR;
                }
                state = StateSemiCompleteExpoNbr;
                break;
            default:
                if (isdigit(currChar)) {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return MEMORY_ERROR;
                    }
                    state = StateCompleteUnsignedExpoNbr;
                    break;
                } else {
                    strFree(&bufferString);
                    return LEXICAL_ERROR;
                }
                break;
            }
            break;
        case StateCompleteUnsignedExpoNbr:
            if (isdigit(currChar)) {
                do {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return MEMORY_ERROR;
                    }
                    charMacro(getCharCheck, &currChar);
                } while (isdigit(currChar));
                    charMacro(unGetCharCheck, currChar);
            } else {
                if (sscanf(strGetStr(&bufferString),"%lf", &token->atribute.d) == EOF) {
                    strFree(&bufferString);
                    return CONVERSION_ERROR;
                }
                token->type = TokenDecimalNbr;
                strFree(&bufferString);
                return SUCCESS;
            }
            break;
        case StateSemiCompleteExpoNbr:
            if (isdigit(currChar)) {
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return MEMORY_ERROR;
                }
                state = StateCompleteExpoNbr;
                break;
            } else {
                strFree(&bufferString);
                return LEXICAL_ERROR;
            }
            break;
        case StateCompleteExpoNbr:
            if (isdigit(currChar)) {
                do {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return MEMORY_ERROR;
                    }
                    charMacro(getCharCheck, &currChar);
                } while (isdigit(currChar));
            charMacro(unGetCharCheck, currChar);
            } else {
                if (sscanf(strGetStr(&bufferString),"%lf", &token->atribute.d) == EOF) {
                    strFree(&bufferString);
                    return CONVERSION_ERROR;
                }
                token->type = TokenDecimalNbr;
                strFree(&bufferString);
                return SUCCESS;
            }
            break;
        case StateWholeNbr:
            state = StateWholeNbr;
            switch (currChar) {
            case '.':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return MEMORY_ERROR;
                }
                state = StateIncompletedecimalNbr;
                break;
            case 'e':
            case 'E':
                if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                    strFree(&bufferString);
                    return MEMORY_ERROR;
                }
                state = StateUnsignedExpoNbr;
                break;
            default:
                if (isdigit(currChar)) {
                    if (strAddChar(&bufferString, currChar) == STR_ERROR) {
                        strFree(&bufferString);
                        return MEMORY_ERROR;
                    }
                    state = StateWholeNbr;
                    break;
                } else {
                    charMacro(unGetCharCheck, currChar);
                    if (sscanf(strGetStr(&bufferString),"%"SCNd64,&token->atribute.i) == EOF) {
                        strFree(&bufferString);
                        return CONVERSION_ERROR;
                    }
                    token->type = TokenWholeNbr;
                    strFree(&bufferString);
                    return SUCCESS;
                }
                strFree(&bufferString);
                return SUCCESS;
            }
            break;
        default:
        return LEXICAL_ERROR;
        }
    }
    return LEXICAL_ERROR;
}
