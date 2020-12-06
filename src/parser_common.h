/*File name: parser_common.h ----------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Viktor Rucký (xrucky01)                                |
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

#ifndef PARSER_COMMON_H
#define PARSER_COMMON_H
#include <stdbool.h>
#include "token.h"
#include "term.h"
#include "symbol.h"
#include "helper.h"
#include "data_type.h"

extern Token curTok;
extern DubiousFunctionCallArray dubiousFunctionCalls;

//Shorthand. Advances to the next token without any checks of the current token. If there are issues raised by the nextToken function, returns from the function with the return code returned by nextToken.
#define acceptAny() {if ((returnCode = nextToken()) != SUCCESS) return returnCode;}

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SYNTAX_ERROR. If the acceptance of the next token fails, returns the error value returned by nextToken.
#define assert(term) {if((returnCode =  accept(term)) != SUCCESS) return returnCode;}

//Variant of assert that executes the clean-up function instead of returning straight away in case of failure.
#define assert_clean(term) {if((returnCode =  accept(term)) != SUCCESS) goto CLEAN_UP;}

//Short hand. Tries to accept the specied token type. If the current token isn't of the specied type, returns from the current function with SUCCESS, i.e. it instead applies the epsilon rule. If there is a failure with nextToken, reuturs the returned error code.
#define assertOrEpsilon(token) {if((returnCode = accept(token)) == SYNTAX_ERROR) return SUCCESS; else if (returnCode != SUCCESS) return returnCode;}

//Variant of assertOrEpsilon that execturs the clean-up function instead of returning straight away in case of failure or epsilon rule.
#define assertOrEpsilon_clean(token) {if((returnCode = accept(token)) == SYNTAX_ERROR) {returnAndClean(SUCCESS);} else if (returnCode != SUCCESS) goto CLEAN_UP;}

//Short hand. Asserts the existence of a TokenEOL on the input. Then it reads TokenEOLs from the input until it hits the first token that isn't TokenEOL.
#define EOL_Mandatory() {assert(TokenEOL); while(peek(TokenEOL)) { acceptAny(); }}

//Short hand. Reads TokenEOLs from the input until it hits the first token that isn't TokenEOL.
#define EOL_Optional() {while(peek(TokenEOL)) { acceptAny(); }}

int accept(_TokenType type);
bool peek(_TokenType type);
int nextToken();
int prevToken();
void freeCurTok();
int beginParsing();
int validateFunctionCall(const SymbolFunction* function, const SymbolVariableArray* lValues, const TermArray* functionParameters);
int parseTerm(Term* term, bool autoForward);
DataType termType(Term* term);
void freeTerm(Term* term);

#endif