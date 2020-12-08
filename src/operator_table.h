#ifndef OPERATOR_TABLE_H
#define OPERATOR_TABLE_H

const int PSATable [13][13] = {
    //                +                       -                      *                     /                     (                    )                     i                   <                      <=                    >                     >=                   ==                    !=                      $
   /* + */  { AssociativityRight,    AssociativityRight,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
   /* - */  { AssociativityRight,    AssociativityRight,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
   /* * */  { AssociativityRight,    AssociativityRight,    AssociativityRight,   AssociativityRight,   AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
   /* / */  { AssociativityRight,    AssociativityRight,    AssociativityRight,   AssociativityRight,   AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
   /* ( */  { AssociativityLeft,     AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityEqual,   /*AssociativityLeft, */  AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,     AssociativityError},
   /* ) */  { AssociativityRight,    AssociativityRight,    AssociativityRight,   AssociativityRight,   AssociativityError, AssociativityRight,   /*AssociativityError,*/  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
//    /* i */  { AssociativityRight,    AssociativityRight,    AssociativityRight,   AssociativityRight,   AssociativityError, AssociativityRight,   /*AssociativityError,*/  AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,   AssociativityRight,    AssociativityRight},
   /* < */  { AssociativityLeft,     AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
   /* <= */ { AssociativityLeft,     AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
   /* > */  { AssociativityLeft,     AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
   /* >= */ { AssociativityLeft,     AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
   /* == */ { AssociativityLeft,     AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
   /* != */ { AssociativityLeft,     AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityRight,   /*AssociativityLeft, */  AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,   AssociativityError,    AssociativityRight},
   /* $ */  { AssociativityLeft,     AssociativityLeft,     AssociativityLeft,    AssociativityLeft,    AssociativityLeft,  AssociativityError,   /*AssociativityLeft, */  AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,    AssociativityLeft,     AssociativityError}
};
// Mozna sjednoceni i a ( a +-, */, a relacni

/**
 * @brief This is debug array and will not be included in final project
 * It is used in print array for normalizing the outputs
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

    "OperationNegate",
    "OperatorNotScreaming",
    "Err"   /** = > */
 };

const char* enumAssociativityTranslate[] = {
    "[",    /** [ || < */
    "]",    /** ] || > */
    "≈",    /** = > */
	"Err"
 };

const char* enumAtomTermTranslate[] = {
    "int",
    "float64",
    "\"string\"",
    "var"
 };

 const char* enumOperationTranslate[] = {
    ":+:",    /** E->E+E */
    ":-:",    /** E->E-E */
    ":*:",    /** E->E*E */
    ":/:",    /** E->E/E */
    ":():",   /** E->(E) */
    ":>:",    /** E->E>E */
    ":>=:",   /** E->E>=E */
    ":<:",    /** E->E<E */
    ":<=:",   /** E->E<=E */
    ":==:",   /** E->E==E */
    ":!=:",   /** E->E!=E */
    ":--:", /** OP-E->OPE */
    ":++:" /** OP+E->OPE */
    ":E:"
 };

#endif