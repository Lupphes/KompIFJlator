#include <stdio.h>
#include <scanner.h>
#include <token.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

#define EOL '\0'



int getCharCheck(int *value) {
    value = getchar();
    if (value == EOF) {
        if (!feof(stdin)) {
            return UNEXPECTED_EOF_ERROR;
        }
        else if (ferror(stdin)) {
            return INPUT_ERROR;
        }
    }
    return SUCCESS;
}

int unGetCharCheck(char *value) {
    value = ungetc(value, stdin);
    if (value == EOF) {
        return UNEXPECTED_EOF_ERROR;
    }
    return SUCCESS;
}

int getToken(Token* token) {
    ScannerState state = StateStart;
    int currChar;
    while (token->type == "") {
        if (getCharCheck(currChar) == INPUT_ERROR) {
            return INPUT_ERROR;
        }
        switch (state) {
        case StateStart:
            switch (currChar) {
            case ',':
                state = StateComma;
                token->type = TokenComma;
                return SUCCESS;
                break;
            case ';':
                state = StateSemicolon;
                token->type = TokenSemicolon;
                return SUCCESS;
                break;
            case '(':
                state = StateLeftBracket;
                token->type = StateLeftBracket;
                return SUCCESS;
                break;
            case ')':
                state = StateRightBracket;
                token->type = StateRightBracket;
                return SUCCESS;
                break;
            case '{':
                state = StateLeftCurlyBracket;
                token->type = StateLeftCurlyBracket;
                return SUCCESS;
                break;
            case '}':
                state = StateRightCurlyBracket;
                token->type = StateRightCurlyBracket;
                return SUCCESS;
                break;
            case '/':
                state = StateSlash;
                break;
            case '"':
                state = StateStartOfString;
                break;
            case ' ' :
            case '  ' :
                state = StateWhiteSpace;
                do {
                    if (getCharCheck(currChar) == INPUT_ERROR) {
                        return INPUT_ERROR;
                    }
                } while (currChar == ' ' || currChar == '   ');
                if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                    return UNEXPECTED_EOF_ERROR;
                }
                token->type = TokenWhiteSpace;
                return SUCCESS;
                break;
            case EOL :
                do {
                    if (getCharCheck(currChar) == INPUT_ERROR) {
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
                state = StateMultiply;
                token->type = TokenMultiply;
                return SUCCESS;
                break;
            case '+':
                state = StateAdd;
                token->type = StateAdd;
                return SUCCESS;
                break;
            case '-':
                state = StateSubtract;
                token->type = StateSubtract;
                return SUCCESS;
                break;
            case '|': // Feature
                state = StateOrFirst;
                break;
            case '&': // Feature
                state = StateAndFirst;
                break;
            case '0':
                state = StateZero;
                break;
            default:
                if (currChar >= '1' && currChar <= '9') {
                    state = StateWholeNbr;
                } else if ((currChar >= 'A' && currChar <= 'Z') || (currChar >= 'a' && currChar <= 'z') || currChar == '_') {
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
                    if (getCharCheck(currChar) != 0) {
                        return INPUT_ERROR;
                    }
                } while (currChar != EOL);
                break;
                state = StateStart;
            case '*':
                state = StateStarStart;
                if (getCharCheck(currChar) != 0) {
                    return INPUT_ERROR;
                }   
                int nextChar;
                if (getCharCheck(nextChar) != 0) {
                        return INPUT_ERROR;
                    } 
                while (currChar != '*' && nextChar != '/') {
                    currChar = nextChar;  
                    if (getCharCheck(nextChar) != 0) {
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
            string stringLiteral;
            if(strInit(&stringLiteral) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            do {
                if (currChar == '\\') {
                    if (getCharCheck(currChar) != 0) {
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
                        if (strAddChar(&stringLiteral, '   ') == STR_ERROR) {
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
                        char hexadecimalValue[3];
                        if (strAddChar(&hexadecimalValue[0], currChar) == STR_ERROR) {
                            strFree(&stringLiteral);
                            return MEMORY_ERROR;
                        }
                        if (strAddChar(&hexadecimalValue[1], currChar) == STR_ERROR) {
                            strFree(&stringLiteral);
                            return MEMORY_ERROR;
                        }
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
                if (getCharCheck(currChar) != 0) {
                        return INPUT_ERROR;
                }
            } while(currChar != '"');
            token->type = TokenEndOfString;
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
                state = StateVarDefine;
                token->type = TokenVarDefine;
                return SUCCESS;
                break;
            case '=':
                state = StateIsEqual;
                token->type = TokenIsEqual;
                return SUCCESS;
                break;
            default:
                return LEXICAL_ERROR;
                break;
            }
        case StateExclamationMark:
            switch (currChar) {
            case ' ':
            case '(':
            case '  ':
                // feature
                return LEXICAL_ERROR;
                break;
            case '=':
                state = StateIsEqual;
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
                state = StateIsGreaterEqual;
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
                state = StateIsLessEqual;
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
            string stringIdentifier;
            if(strInit(&stringIdentifier) == STR_ERROR) {
                return MEMORY_ERROR;
            }
            if (strAddChar(&stringIdentifier, currChar) == STR_ERROR) {
                strFree(&stringIdentifier);
                return MEMORY_ERROR;
            }
            do {
                if (getCharCheck(currChar) != 0) {
                    return INPUT_ERROR;
                } 
                if (strAddChar(&stringIdentifier, currChar) == STR_ERROR) {
                    strFree(&stringIdentifier);
                    return MEMORY_ERROR;
                }
            } while ((currChar >= 'A' && currChar <= 'Z') || (currChar >= 'a' && currChar <= 'z') || currChar == '_' || (currChar >= '0' && currChar <= '9'));
            if (unGetCharCheck(currChar) == UNEXPECTED_EOF_ERROR) {
                return UNEXPECTED_EOF_ERROR;
            }
            state = StateIdentifier;
            token->type = TokenIdentifier;
            return SUCCESS;
            break;
        case StateZero:
            switch (currChar) {
            case '.':
                state = StateIncompletedecimalNbr;
                break;
            case 'e':
            case 'E':
                state = StateUnsignedExpoNbr;
                break;
            default:
                return LEXICAL_ERROR;
                break;
            }
            break;
        case StateWholeNbr:
            switch (currChar) {
            case '.':
                state = StateIncompletedecimalNbr;
                break;
            case 'e':
            case 'E':
                state = StateUnsignedExpoNbr;
                break;
            default:
                return LEXICAL_ERROR;
                break;
            }
            break;
        case StateUnderline:
            state = StateCompleteBaseNbr; // Feature
            break;
        case StateIncompletedecimalNbr:
            if (currChar >= '0' && currChar <= '9') {
                /* code */
            } else {
                return LEXICAL_ERROR;
            }
            break;
        case StateUnsignedExpoNbr:
            switch (currChar) {
            case '+':
            case '-':
                state = StateSemiCompleteExpoNbr;
                break;
            default:
                if (currChar >= '0' && currChar <= '9') {
                    state = StateCompleteUnsignedExpoNbr;
                }
                break;
            }
            break;
        case StateCompleteBaseNbr:
            /* */
            break;
        case StateSemiCompleteExpoNbr:
            switch (currChar) {
                if (currChar >= '0' && currChar <= '9') {
                    /* code */
                } else {
                    return LEXICAL_ERROR;
                }
            default:
                return LEXICAL_ERROR;
                break;
                break;
            }
        }
    }

    return 0;
}