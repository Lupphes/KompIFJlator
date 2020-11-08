/*File name: str.c -----------------------------------------------------------------*
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

//jednoducha knihovna pro praci s nekonecne dlouhymi retezci
#include <string.h>
#include <malloc.h>
#include "str.h"

#define STR_LEN_INC 8
// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla 

int strInit(string *s)
// funkce vytvori novy retezec
{
   if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
      return STR_ERROR;
   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;
   return STR_SUCCESS;
}

void strFree(string *s)
// funkce uvolni retezec z pameti
{
   free(s->str);
}

void strClear(string *s)
// funkce vymaze obsah retezce
{
   s->str[0] = '\0';
   s->length = 0;
}

int strAddChar(string *s1, char c)
// prida na konec retezce jeden znak
{
   if (s1->length + 1 >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL)
         return STR_ERROR;
      s1->allocSize = s1->length + STR_LEN_INC;
   }
   s1->str[s1->length] = c;
   s1->length++;
   s1->str[s1->length] = '\0';
   return STR_SUCCESS;
}

int strCopyString(string *s1, const string *s2)
// prekopiruje retezec s2 do s1
{
   int newLength = s2->length;
   if (newLength >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
         return STR_ERROR;
      s1->allocSize = newLength + 1;
   }
   strcpy(s1->str, s2->str);
   s1->length = newLength;
   return STR_SUCCESS;
}

//Copies a C string into a smart string structure
int strCopyConstString(string *s1, const char* s2){
   int newLength = strlen(s2);
   if (newLength >= s1->allocSize){
      if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
         return STR_ERROR;
      s1->allocSize = newLength + 1;
   }
   strcpy(s1->str,s2);
   s1->length = newLength;
   return STR_SUCCESS;
}

int strCmpString(const string *s1, const string *s2)
// porovna oba retezce a vrati vysledek
{
   return strcmp(s1->str, s2->str);
}

int strCmpConstStr(const string *s1, const char* s2)
// porovna nas retezec s konstantnim retezcem
{
   return strcmp(s1->str, s2);
}

char *strGetStr(string *s)
// vrati textovou cast retezce
{
   return s->str;
}

int strGetLength(const string *s)
// vrati delku daneho retezce
{
   return s->length;
}
