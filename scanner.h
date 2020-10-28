#include <str.h>
#include <stdint.h>

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
    /* Brackets, commas, semicolons */
    StateComma,
    StateSemicolon,
    StateRightBracket,
    StateLeftBracket,
    StateLeftCurlyBracket,
    StateRightCurlyBracket,
    StateEOL,
    StateEOF,
    /* String */
    StateStartOfString,
    StateEscapeSequence,
    StateEscapedHex,
    StateHexFirst,
    StateEndOfString,
    /* Comments */
    StateSlash,
    StateCommentModeLine,
    StateStarStart,
    StateCommentModeMulti,
    StateCommentDone,
    /* White spaces */
    StateWhiteSpace,
    /* Identifier */
    StateIdentifier,
    /* Conditions */
    StateEquals,
    StateVarDefine,
    StateIsEqual,
    StateExclamationMark, // Feature
    StateNotEqual,
    StateIsGreaterThan,
    StateIsGreaterEqual,
    StateIsLessThan,
    StateIsLessEqual,
    /* Operators */
    StateMultiply,
    StateAdd,
    StateSubtract,
    /* Conditional operators -- feature */
    StateAndFirst,
    StateAndSecond,
    StateOrFirst,
    StateOrSecond,
    /* Numbers */
    StateWholeNbr,
    StateZero,
    StateCompleteBaseNbr, // Feature
    StateIncompletedecimalNbr,
    StateCompleteDecimalNbr,
    StateUnsignedExpoNbr,
    StateCompleteUnsignedExpoNbr,
    StateSemiCompleteExpoNbr,
    StateCompleteExpoNbr,
    StateUnderline
} ScannerState;

int getCharCheck(char *value);
int unGetCharCheck(char *value);
int getToken(Token*);
_TokenType processIdentifier(); // Compare strings and return tokens

