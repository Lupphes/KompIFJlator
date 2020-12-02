#ifndef OPERATOR_TABLE_H
#define OPERATOR_TABLE_H

const int _PSATable [14][14] = {
    //                  +                              -                           *                            /                          (                       )                           i                            <                       <=                          >                           >=                              ==                          !=                          $
   /* + */  { OperatorRightAssociative,    OperatorRightAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,    OperatorRightAssociative},
   /* - */  { OperatorRightAssociative,    OperatorRightAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,    OperatorRightAssociative},
   /* * */  { OperatorRightAssociative,    OperatorRightAssociative,    OperatorRightAssociative,   OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,    OperatorRightAssociative},
   /* / */  { OperatorRightAssociative,    OperatorRightAssociative,    OperatorRightAssociative,   OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,    OperatorRightAssociative},
   /* ( */  { OperatorLeftAssociative,     OperatorLeftAssociative,     OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorEqualAssociative,   OperatorLeftAssociative,  OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,     OperatorError},
   /* ) */  { OperatorRightAssociative,    OperatorRightAssociative,    OperatorRightAssociative,   OperatorRightAssociative,   OperatorError,            OperatorRightAssociative,   OperatorError,            OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,    OperatorRightAssociative},
   /* i */  { OperatorRightAssociative,    OperatorRightAssociative,    OperatorRightAssociative,   OperatorRightAssociative,   OperatorError,            OperatorRightAssociative,   OperatorError,            OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,   OperatorRightAssociative,    OperatorRightAssociative},
   /* < */  { OperatorLeftAssociative,     OperatorLeftAssociative,     OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,               OperatorRightAssociative},
   /* <= */ { OperatorLeftAssociative,     OperatorLeftAssociative,     OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,               OperatorRightAssociative},
   /* > */  { OperatorLeftAssociative,     OperatorLeftAssociative,     OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,               OperatorRightAssociative},
   /* >= */ { OperatorLeftAssociative,     OperatorLeftAssociative,     OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,               OperatorRightAssociative},
   /* == */ { OperatorLeftAssociative,     OperatorLeftAssociative,     OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,               OperatorRightAssociative},
   /* != */ { OperatorLeftAssociative,     OperatorLeftAssociative,     OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,              OperatorError,               OperatorRightAssociative},
   /* $ */  { OperatorLeftAssociative,     OperatorLeftAssociative,     OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,  OperatorError,              OperatorLeftAssociative,  OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,    OperatorLeftAssociative,     OperatorError}
};
// Mozna sjednoceni i a ( a +-, */, a relacni

/**
 * @brief This is debug array and will not be included in final project
 * It is used in print array for normalizing the outputs
 * 
 */
const char* enumOperatorTranslate[] = {
    "+", /** + */
    "-", /** - */
    "*", /** * */
    "/", /** / */

    "(", /** ( */
    ")", /** ) */
    "id", /** id */

    "<", /** < */
    "<=", /** <= */
    ">", /** > */
    ">=", /** >= */
    "==", /** == */
    "!=", /** != */
    "$", /** $ */
    "lalala",
    /* this need redo */
    "OperatorWholeNumber", /** int -- whole numeber */ // EXPATOM
    "OperatorDecimal", /** float -- decimal number */
    "OperatorStringLiteral", /** string -- text string */
    "OperationNegate",
    
    "<",  /** [ || < */
    ">",  /** ] || > */
    "=",  /** = > */
    "OperatorStart", /** ^ */
    "E", /** Exp */
    "OperatorNotScreaming",
    "OperatorError"  /** = > */
 };

#endif