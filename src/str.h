/*File name: str.h -----------------------------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20                    |
 |Team:       124, varianta II                                                      |
 |Authors:    PŘEVZATO ZE SOUBORU "jednoduchy_interpret.zip" ZE STRÁNEK             |
 |            http://www.fit.vutbr.cz/study/courses/IFJ/public/project/             |
 |            Viktor Rucký (xrucky01) (úpravy)                                      |
 |                                                                                  |
 |  _      _     _   __                   __  _        _                            |
 | | |    (_)   | | /_/                  /_/ | |      | |                           |
 | | |     _  __| | ___   _   _  __   ___   _| |_ __ _| |__  _   _                  |
 | | |    | |/ _` |/ _ \ | | | | \ \ / / | | | __/ _` | '_ \| | | |                 |
 | | |____| | (_| |  __/ | |_| |  \ V /| |_| | || (_| | | | | |_| |                 |
 | |______|_|\__,_|\___|  \__,_|   \_/  \__, |\__\__,_|_| |_|\__,_|                 |
 |                                       __/ |                                      |
 |                                      |___/                                       |
 *----------------------------------------------------------------------------------*/

//hlavickovy soubor pro praci s nekonecne dlouhymi retezci
#ifndef STR_H
#define STR_H

#include "error.h"

typedef struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  int length;		// skutecna delka retezce
  int allocSize;	// velikost alokovane pameti
} string;

typedef enum {
    STR_SUCCESS = SUCCESS,
    STR_ERROR = INTERNAL_ERROR
} ErrorString;


int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strCopyString(string *s1, const string *s2);
int strCopyConstString(string *s1, const char* s2);
int strCmpString(const string *s1, const string *s2);
int strCmpConstStr(const string *s1, const char *s2);

char *strGetStr(string *s);
int strGetLength(const string *s);

#endif