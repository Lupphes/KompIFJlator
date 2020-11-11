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

#ifndef LEX_ANALYSIS_H
#define LEX_ANALYSIS_H

typedef enum {
    SUCCESS = 0
} ErrorGetToken;


typedef enum {
    SymbolMultiple = 0,
    SymbolDivide,

    SymbolAdd,
    SymbolSubtract,

    SymbolLessThan,
    SymbolLessEqual,
    SymbolGreaterThan,
    SymbolGreaterEqual,
    SymbolIsEqual,
    SymbolIsNotEqual,

    SymbolEquals, // =
    SymbolVariableDefine, // :=
    SymbolResultAdd,
    SymbolResultSubtract,
    SymbolResultMultiply,
    SymbolResultDivide,

    SymbolRightBracket,
    SymbolLeftBracket

} Symbols;

int analyzeExpression(Token* token);


#endif
