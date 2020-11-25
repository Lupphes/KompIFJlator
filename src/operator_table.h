#ifndef OPERATOR_TABLE_H
#define OPERATOR_TABLE_H

#include "expression_analysis.h"


const int _PSATable [14][7] = {
    //                  +,-                           *,/                          (                       )                           i                   <,<=,>,>=,==,!=                 $
   /* + */  { OperatorRightAssociative,    OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorRightAssociative},
   /* - */  { OperatorRightAssociative,    OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorRightAssociative},
   /* * */  { OperatorRightAssociative,    OperatorRightAssociative,  OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorRightAssociative},
   /* / */  { OperatorRightAssociative,    OperatorRightAssociative,  OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorRightAssociative},
   /* ( */  { OperatorLeftAssociative,     OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorEqualAssociative,   OperatorLeftAssociative,  OperatorLeftAssociative,    OperatorError},
   /* ) */  { OperatorRightAssociative,    OperatorRightAssociative,  OperatorError,            OperatorRightAssociative,   OperatorError,            OperatorRightAssociative,   OperatorRightAssociative},
   /* i */  { OperatorRightAssociative,    OperatorRightAssociative,  OperatorError,            OperatorRightAssociative,   OperatorError,            OperatorRightAssociative,   OperatorRightAssociative},
   /* < */  { OperatorLeftAssociative,     OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError},
   /* <= */ { OperatorLeftAssociative,     OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError},
   /* > */  { OperatorLeftAssociative,     OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError},
   /* >= */ { OperatorLeftAssociative,     OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError},
   /* == */ { OperatorLeftAssociative,     OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError},
   /* != */ { OperatorLeftAssociative,     OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorRightAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorError},
   /* $ */  { OperatorLeftAssociative,     OperatorLeftAssociative,   OperatorLeftAssociative,  OperatorError,              OperatorLeftAssociative,  OperatorLeftAssociative,    OperatorError}
};
// Mozna sjednoceni i a (

#endif