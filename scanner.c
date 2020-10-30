/*File name: scanner.c ----------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Ondřej Sloup (xsloup02)                                |
 |                                                                   |
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

#include "scanner.h"
#include "keywords.h"
#include "str.h"

#define EOL '\0'

int getCharCheck(int *value) {
    *value = getchar();
    if (*value == EOF) {
        if (!feof(stdin)) {
            return UNEXPECTED_EOF_ERROR;
        }
        else if (ferror(stdin)) {
            return INPUT_ERROR;
        }
    }
    return SUCCESS;
}

int unGetCharCheck(int value) {
    value = ungetc(value, stdin);
    if (value == EOF) {
        return UNEXPECTED_EOF_ERROR;
    }
    return SUCCESS;
}

int getToken(Token* token) {
    ScannerState state = StateStart;
    string stringLiteral;
    string stringIdentifier;
    string number;
    int currChar, previousChar;
    char hexadecimalValue[3];
    token->type = TokenUndefined;
    while (token->type == TokenUndefined) {
        if (getCharCheck(&currChar) == INPUT_ERROR) {
            return INPUT_ERROR;
        }
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
                do {
                    if (getCharCheck(&currChar) == INPUT_ERROR) {
                        return INPUT_ERROR;
                    }
                } while (currChar == ' ' || currChar == '\t');
                if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                    return UNEXPECTED_EOF_ERROR;
                }
                token->type = TokenWhiteSpace;
                return SUCCESS;
                break;
            case EOL :
                do {
                    if (getCharCheck(&currChar) == INPUT_ERROR) {
                        return INPUT_ERROR;
                    }
                } while (currChar == EOL);
                if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                    return UNEXPECTED_EOF_ERROR;
                }
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
            case '|': // Feature
                state = StateOrFirst;
                break;
            case '&': // Feature
                state = StateAndFirst;
                break;
            case '0':
                previousChar = currChar;
                state = StateZero;
                break;
            default:
                if (currChar >= '1' && currChar <= '9') {
                    previousChar = currChar;
                    state = StateWholeNbr;
                } else if ((currChar >= 'A' && currChar <= 'Z') || (currChar >= 'a' && currChar <= 'z') || currChar == '_') {
                    previousChar = currChar;
                    state = StateIdentifier;
                } else {
                    return LEXICAL_ERROR;
                }
                break;
            }
            break;
        case StateSlash:
            switch (currChar) {
            case '/':
                state = StateSlash;
                do {
                    if (getCharCheck(&currChar) != 0) {
                        return INPUT_ERROR;
                    }
                } while (currChar != EOL);
                break;
                state = StateStart;
            case '*':
                if (getCharCheck(&currChar) != 0) {
                    return INPUT_ERROR;
                }
                int nextChar;
                if (getCharCheck(&nextChar) != 0) {
                    return INPUT_ERROR;
                }
                while (currChar != '*' && nextChar != '/') {
                    currChar = nextChar;
                    if (getCharCheck(&nextChar) != 0) {
                        return INPUT_ERROR;
                    }
                }
                state = StateStart;
                break;
            default:
                return LEXICAL_ERROR;
                break;
            }
            break;
        case StateStartOfString:
            if(strInit(&stringLiteral) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            if (strAddChar(&stringLiteral, previousChar) == STR_ERROR) {
                strFree(&stringLiteral);
                return MEMORY_ERROR;
            }
            do {
                if (currChar == '\\') {
                    if (getCharCheck(&currChar) != 0) {
                        strFree(&stringLiteral);
                        return INPUT_ERROR;
                    }
                    switch (currChar) {
                    case '"':
                        if (strAddChar(&stringLiteral, currChar) == STR_ERROR) {
                            strFree(&stringLiteral);
                            return MEMORY_ERROR;
                        }
                        break;
                    case 't':
                        if (strAddChar(&stringLiteral, '\t') == STR_ERROR) {
                            strFree(&stringLiteral);
                            return MEMORY_ERROR;
                        }
                        break;
                    case 'n':
                        if (strAddChar(&stringLiteral, '\0') == STR_ERROR) {
                            strFree(&stringLiteral);
                            return MEMORY_ERROR;
                        }
                        break;
                    case '\\':
                        if (strAddChar(&stringLiteral, '\\') == STR_ERROR) {
                            strFree(&stringLiteral);
                            return MEMORY_ERROR;
                        }
                        break;
                    case 'x':
                        if (getCharCheck(&currChar) != 0) {
                            strFree(&stringIdentifier);
                            return INPUT_ERROR;
                        }
                        if (getCharCheck(&previousChar) != 0) {
                            strFree(&stringIdentifier);
                            return INPUT_ERROR;
                        }
                        hexadecimalValue[0] = currChar;
                        hexadecimalValue[1] = previousChar;
                        errno = 0;
                        int number = (int)strtol(hexadecimalValue, NULL, 16);
                        if ((number == LONG_MAX || number == LONG_MIN) && errno == ERANGE)
                        {
                            strFree(&stringLiteral);
                            return LEXICAL_ERROR;
                        }
                        if (strAddChar(&stringLiteral, (char)number) == STR_ERROR) {
                            strFree(&stringLiteral);
                            return MEMORY_ERROR;
                        }
                        break;
                    default:
                        strFree(&stringLiteral);
                        return LEXICAL_ERROR;
                        break;
                    }
                }
                else if (currChar > '"' || currChar == '!' || currChar == ' ') {
                    if (strAddChar(&stringLiteral, currChar) == STR_ERROR) {
                        return MEMORY_ERROR;
                    }
                    continue;
                } else {
                    return LEXICAL_ERROR;
                }
                if (getCharCheck(&currChar) != 0) {
                    return INPUT_ERROR;
                }
            } while(currChar != '"');
            token->type = TokenEndOfString;
            token->atribute.t = TypeString;
            if(strInit(&token->atribute.s) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            if (strCopyString(&stringLiteral, &token->atribute.s) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            return SUCCESS;
            break;
        case StateEquals:
            switch (currChar) {
            case ':':
                token->type = TokenVarDefine;
                return SUCCESS;
                break;
            case '=':
                token->type = TokenIsEqual;
                return SUCCESS;
                break;
            default:
                return LEXICAL_ERROR;
                break;
            }
        case StateExclamationMark:
            switch (currChar) {
            // case ' ':
            // case '(':
            // case '  ':
            //     // feature
            //     return LEXICAL_ERROR;
            //     break;
            case '=':
                token->type = TokenIsEqual;
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
                return LEXICAL_ERROR;
                break;
            }
        case StateIsLessThan:
            switch (currChar) {
            case '=':
                token->type = TokenIsLessEqual;
                return SUCCESS;
                break;
            default:
                return LEXICAL_ERROR;
                break;
            }
        case StateAndFirst:
            switch (currChar) {
            case '&':
                // Feature
                return LEXICAL_ERROR;
                break;

            default:
                return LEXICAL_ERROR;
                break;
            }
        case StateOrFirst:
            switch (currChar) {
            case '|':
                // Feature
                return LEXICAL_ERROR;
                break;
            default:
                return LEXICAL_ERROR;
                break;
            }
        case StateIdentifier:
            state = StateIdentifier;

            if(strInit(&stringIdentifier) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            if (strAddChar(&stringIdentifier, previousChar) == STR_ERROR) {
                strFree(&stringIdentifier);
                return MEMORY_ERROR;
            }
            do
            {
                if (getCharCheck(&currChar) != 0) {
                    strFree(&stringIdentifier);
                    return INPUT_ERROR;
                }
                if (!(currChar >= 'A' && currChar <= 'Z') || (currChar >= 'a' && currChar <= 'z') || currChar == '_' || (currChar >= '0' && currChar <= '9'))
                {
                    if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                        strFree(&stringIdentifier);
                        return UNEXPECTED_EOF_ERROR;
                    }
                } else
                {
                    if (strAddChar(&stringIdentifier, currChar) == STR_ERROR) {
                        strFree(&stringIdentifier);
                        return MEMORY_ERROR;
                    }
                }


            } while ((currChar >= 'A' && currChar <= 'Z') || (currChar >= 'a' && currChar <= 'z') || currChar == '_' || (currChar >= '0' && currChar <= '9'));
            for (int i = 0; i < 9; i++)
            {
                if(strCmpConstStr(&stringIdentifier, keywords[i].key) == 0) {
                    token->type = keywords[i].value.type;
                    token->atribute.t = keywords[i].value.atribute.t;
                    break;
                } else {
                    token->type = TokenIdentifier;
                }
            }

            if(strInit(&token->atribute.s) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            if (strCopyString(&stringLiteral, &token->atribute.s) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            return SUCCESS;
            break;
        case StateZero:

            if(strInit(&number) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            if (strAddChar(&number, previousChar) == STR_ERROR) {
                strFree(&number);
                return MEMORY_ERROR;
            }
            switch (currChar) {
            case '.':
                if (strAddChar(&number, currChar) == STR_ERROR) {
                    strFree(&number);
                    return MEMORY_ERROR;
                }
                state = StateIncompletedecimalNbr;
                break;
            case 'e':
            case 'E':
                if (strAddChar(&number, currChar) == STR_ERROR) {
                    strFree(&number);
                    return MEMORY_ERROR;
                }
                state = StateUnsignedExpoNbr;
                break;
            default:
                // process whole number 0
                if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                    return UNEXPECTED_EOF_ERROR;
                }
                char* convertedCharNumber = strGetStr(&number);
                sscanf(convertedCharNumber, "%ld", &token->atribute.i);
                token->atribute.t = TypeInt;
                token->type = TokenDataType;
                strFree(&number);
                return SUCCESS;
            }
            break;
        case StateIncompletedecimalNbr:
            state = StateCompleteDecimalNbr;
            if (currChar >= '0' && currChar <= '9') {
                if (strAddChar(&number, currChar) == STR_ERROR) {
                    strFree(&number);
                    return MEMORY_ERROR;
                }
                state = StateCompleteDecimalNbr;
                break;
            } else
            {
                strFree(&number);
                return LEXICAL_ERROR;
                break;
            }
            break;
        case StateCompleteDecimalNbr:
            state = StateCompleteDecimalNbr;
            switch (currChar) {
            case 'e':
            case 'E':
                if (strAddChar(&number, currChar) == STR_ERROR) {
                    strFree(&number);
                    return MEMORY_ERROR;
                }
                state = StateUnsignedExpoNbr;
                break;

            default:
                if (currChar >= '0' && currChar <= '9') {
                    if (strAddChar(&number, currChar) == STR_ERROR) {
                        strFree(&number);
                        return MEMORY_ERROR;
                    }
                    break;
                } else {
                    // process whole number 0.0004854
                    char* convertedCharNumber = strGetStr(&number);
                    sscanf(convertedCharNumber, "%lf", &token->atribute.d);
                    token->atribute.t = TypeFloat64;
                    token->type = TokenDataType;
                    strFree(&number);
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
                if (strAddChar(&number, currChar) == STR_ERROR) {
                    strFree(&number);
                    return MEMORY_ERROR;
                }
                state = StateSemiCompleteExpoNbr;
                break;
            default:
                if (currChar >= '0' && currChar <= '9') {
                    if (strAddChar(&number, currChar) == STR_ERROR) {
                        strFree(&number);
                        return MEMORY_ERROR;
                    }
                    state = StateCompleteUnsignedExpoNbr;
                    break;
                } else {
                    strFree(&number);
                    return LEXICAL_ERROR;
                }
                break;
            }
            break;
        case StateCompleteUnsignedExpoNbr:
            if (currChar >= '0' && currChar <= '9') {
                do {
                    if (strAddChar(&number, currChar) == STR_ERROR) {
                        strFree(&number);
                        return MEMORY_ERROR;
                    }
                    if (getCharCheck(&currChar) == INPUT_ERROR) {
                        return INPUT_ERROR;
                    }
                } while (currChar >= '0' && currChar <= '9');
                if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                    return UNEXPECTED_EOF_ERROR;
                }
            } else {
                // process whole number 0.000E4854
                char* convertedCharNumber = strGetStr(&number);
                sscanf(convertedCharNumber, "%lf", &token->atribute.d);
                token->atribute.t = TypeFloat64;
                token->type = TokenDataType;
                strFree(&number);
                return SUCCESS;
            }
            break;
        case StateSemiCompleteExpoNbr:
            if (currChar >= '0' && currChar <= '9') {
                if (strAddChar(&number, currChar) == STR_ERROR) {
                    strFree(&number);
                    return MEMORY_ERROR;
                }
                state = StateCompleteExpoNbr;
                break;
            } else {
                strFree(&number);
                return LEXICAL_ERROR;
            }
            break;
        case StateCompleteExpoNbr:
            if (currChar >= '0' && currChar <= '9') {
                do {
                    if (strAddChar(&number, currChar) == STR_ERROR) {
                        strFree(&number);
                        return MEMORY_ERROR;
                    }
                    if (getCharCheck(&currChar) == INPUT_ERROR) {
                        return INPUT_ERROR;
                    }
                } while (currChar >= '0' && currChar <= '9');
                if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                    return UNEXPECTED_EOF_ERROR;
                }
            } else {
                // process whole number 0.000E+4854
                char* convertedCharNumber = strGetStr(&number);
                sscanf(convertedCharNumber, "%lf", &token->atribute.d);
                token->atribute.t = TypeFloat64;
                token->type = TokenDataType;
                strFree(&number);
                return SUCCESS;
            }
            break;
        case StateWholeNbr:
            state = StateWholeNbr;
            if(strInit(&number) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            if (strAddChar(&number, previousChar) == STR_ERROR) {
                strFree(&number);
                return MEMORY_ERROR;
            }
            switch (currChar) {
            case '.':
                if (strAddChar(&number, currChar) == STR_ERROR) {
                    strFree(&number);
                    return MEMORY_ERROR;
                }
                state = StateIncompletedecimalNbr;
                break;
            case 'e':
            case 'E':
                if (strAddChar(&number, currChar) == STR_ERROR) {
                    strFree(&number);
                    return MEMORY_ERROR;
                }
                state = StateUnsignedExpoNbr;
                break;
            default:
                if (currChar >= '0' && currChar <= '9') {
                    if (strAddChar(&number, currChar) == STR_ERROR) {
                        strFree(&number);
                        return MEMORY_ERROR;
                    }
                    break;
                } else {
                    if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                        return UNEXPECTED_EOF_ERROR;
                    }
                    // process whole number 154
                    char* convertedCharNumber = strGetStr(&number);
                    sscanf(convertedCharNumber, "%ld", &token->atribute.i);
                    token->atribute.t = TypeInt;
                    token->type = TokenDataType;
                    strFree(&number);
                    return SUCCESS;
                }
                strFree(&number);
                return SUCCESS;
            }
            break;
        }
    }
    return LEXICAL_ERROR;
}

