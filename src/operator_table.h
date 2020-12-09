/*File name: operator_table.h ----------------------------------------*
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

#ifndef OPERATOR_TABLE_H
#define OPERATOR_TABLE_H

/**
 * @brief The operator precedence table
 * This array is used to find the right associativity from given operators (from Operator enum)
 * @author Ondřej Sloup
 * @author Dominik Irovský (prvotní implementace)
 * 
 */
const int PSATable [13][13] = {
    //                    +                      -                      *                     /                     (                   )                    <                    <=                    >                     >=                   ==                    !=                       $
    /* +  */    { AssociativityRight,   AssociativityRight,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
    /* -  */    { AssociativityRight,   AssociativityRight,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
    /* *  */    { AssociativityRight,   AssociativityRight,    AssociativityRight,   AssociativityRight,   AssociativityLeft,  AssociativityRight,  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
    /* /  */    { AssociativityRight,   AssociativityRight,    AssociativityRight,   AssociativityRight,   AssociativityLeft,  AssociativityRight,  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
    /* (  */    { AssociativityLeft,    AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityEqual,  AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,     AssociativityError},
    /* )  */    { AssociativityRight,   AssociativityRight,    AssociativityRight,   AssociativityRight,   AssociativityError, AssociativityRight,  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
    /* <  */    { AssociativityLeft,    AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
    /* <= */    { AssociativityLeft,    AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
    /* >  */    { AssociativityLeft,    AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
    /* >= */    { AssociativityLeft,    AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
    /* == */    { AssociativityLeft,    AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
    /* != */    { AssociativityLeft,    AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
    /* $  */    { AssociativityLeft,    AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityError,  AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,     AssociativityError}
};

/**
 * @brief This array is used to convert ExpItem operation to printable char
 * This array is used for debugging purposes in function printStack()
 * @author Ondřej Sloup
 * 
 */
const char* enumOperatorTranslate[] = {
    "+",    /** + */
    "-",    /** - */
    "*",    /** * */
    "/",    /** / */
    "(",    /** ( */
    ")",    /** ) */
    "<",    /** < */
    "<=",   /** <= */
    ">",    /** > */
    ">=",   /** >= */
    "==",   /** == */
    "!=",   /** != */
    "$$",   /** $ */
    "- -",  /** Operator nothing */
    "Err"   /** Error state */
 };

/**
 * @brief This array is used to convert ExpItem associativity to printable char
 * This array is used for debugging purposes in function printStack()
 * @author Ondřej Sloup
 * 
 */
const char* enumAssociativityTranslate[] = {
    "[",    /** [ or < */
    "]",    /** ] or > */
    "≈",    /** = */
	"Err"   /** Error state */
 };

/**
 * @brief This array is used to convert Term datatype to printable char
 * This array is used for debugging purposes in function printStack()
 * @author Ondřej Sloup
 * 
 */
const char* enumAtomTermTranslate[] = {
    "int",          /** Int type term */
    "float64",      /** Float64 type term */
    "\"string\"",   /** String type term */
    "var"           /** Variable */
 };

#endif
