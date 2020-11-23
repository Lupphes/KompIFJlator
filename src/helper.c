/*File name: helper.c -----------------------------------------------*
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

#include "helper.h"
#include "error.h"
#include "str.h"
#include "symbol.h"
#include "term.h"
#include <stdlib.h>

/**
 * @brief Initalises the string array for use. This function must be called before any other on an uninitialised string array.
 * 
 * @param arr The string array to initialise.
 */
void initStringArray(StringArray* arr){
    arr->count = 0;
    arr->arr = NULL;
}

/**
 * @brief Adds a string to the specified string array (by performing a deep copy).
 * 
 * @param arr The array to inser the string into.
 * @param str The string to insert into the array.
 * @return int SUCCESS if the operation was succesful. INTERNAL_ERROR if there was a problem with memory allocation.
 */
int addToStringArray(StringArray* arr, string* str){
    int returnCode;
    
    int newCount = ++arr->count;
    if ((arr->arr = realloc(arr->arr,sizeof(string)*newCount)) == NULL)
        return INTERNAL_ERROR;
    callAndHandleException(strInit(&arr->arr[newCount-1]));
    callAndHandleException(strCopyString(&arr->arr[newCount-1],str));

    return SUCCESS;
}

/**
 * @brief Return the number of elements in the specified string array.
 * 
 * @param arr The array to get the number of elements of.
 * @return int The number of elements in the specified array.
 */
int countInStringArray(StringArray* arr){
    return arr->count;
}

/**
 * @brief Frees the memory used by the string array (destroys the string "objects" contained within).
 * 
 * @param arr The array to free its memory.
 */
void freeStringArray(StringArray* arr){
    for(int i = 0; i < arr->count;i++)
        strFree(&arr->arr[i]);
    free(arr->arr);
    arr->count = -1;
}


/**
 * @brief Initalises the variable-symbol array for use. This function must be called before any other on an uninitialised variable-symbol array.
 * 
 * @param arr The string array to initialise.
 */
void initSymbolVariableArray(SymbolVariableArray* arr){
    arr->count = 0;
    arr->arr = NULL;
}

/**
 * @brief Adds a variable to the specified variable-symbol array (by performing a shallow copy).
 * 
 * @param arr The array to inser the variable symbol into.
 * @param str The variable symbol to insert into the array.
 * @return int SUCCESS if the operation was succesful. INTERNAL_ERROR if there was a problem with memory allocation.
 */
int addToSymbolVariableArray(SymbolVariableArray* arr, SymbolVariable* var){
    int returnCode;
    
    int newCount = ++arr->count;
    if ((arr->arr = realloc(arr->arr,sizeof(SymbolVariable)*newCount)) == NULL)
        return INTERNAL_ERROR;
    arr->arr[newCount-1] = var;

    return SUCCESS;
}

/**
 * @brief Return the number of elements in the specified variable-symbol array.
 * 
 * @param arr The array to get the number of elements of.
 * @return int The number of elements in the specified array.
 */
int countInSymbolVariableArray(SymbolVariableArray* arr){
    return arr->count;
}

/**
 * @brief Frees the memory used by the symbol-variable array (does not destroy the "objects" stored within the array).
 * 
 * @param arr The array to free its memory.
 */
void freeSymbolVariableArray(SymbolVariableArray* arr){
    free(arr->arr);
    arr->count = -1;
}

/**
 * @brief Initalises the term array for use. This function must be called before any other on an uninitialised term array.
 * 
 * @param arr The string array to initialise.
 */
void initTermArray(TermArray* arr){
    arr->count = 0;
    arr->arr = NULL;
}

/**
 * @brief Adds a variable to the specified term array (by performing a shallow copy).
 * 
 * @param arr The array to insert the term into.
 * @param str The term to insert into the array.
 * @return int SUCCESS if the operation was succesful. INTERNAL_ERROR if there was a problem with memory allocation.
 */
int addToTermArray(TermArray* arr, Term* term){
    int returnCode;
    
    int newCount = ++arr->count;
    if ((arr->arr = realloc(arr->arr,sizeof(Term)*newCount)) == NULL)
        return INTERNAL_ERROR;
    arr->arr[newCount-1] = term;

    return SUCCESS;
}

/**
 * @brief Return the number of elements in the specified term array.
 * 
 * @param arr The array to get the number of elements of.
 * @return int The number of elements in the specified array.
 */
int countInTermArray(TermArray* arr){
    return arr->count;
}

/**
 * @brief Frees the memory used by the term array (DESTROYS the "objects" stored within the array).
 * 
 * @param arr The array to free its memory.
 */
void freeTermArray(TermArray* arr){
    for(int i = 0; i < arr->count;i++){
        if(arr->arr[i]->type == TermStringLiteral)
            strFree(&arr->arr[i]->value.s);
        free(arr->arr[i]);
    }
    free(arr->arr);
    arr->count = -1;
}

/**
 * @brief Initalises the function-call array for use. This function must be called before any other on an uninitialised function-call array.
 * 
 * @param arr The function-call array to initialise.
 */
void initDubiousFunctionCallArray(DubiousFunctionCallArray* arr){
    arr->count = 0;
    arr->arr = NULL;
}

/**
 * @brief Adds a variable to the specified function-call array (by performing a deep copy).
 * 
 * @param arr The array to insert the function call into.
 * @param str The function call to insert into the array.
 * @return int SUCCESS if the operation was succesful. INTERNAL_ERROR if there was a problem with memory allocation.
 */
int addToDubiousFunctionCallArray(DubiousFunctionCallArray* arr, DubiousFunctionCall* functionCall){
    int returnCode;
    
    int newCount = ++arr->count;
    if ((arr->arr = realloc(arr->arr,sizeof(Term)*newCount)) == NULL)
        return INTERNAL_ERROR;
    arr->arr[newCount-1] = *functionCall;

    return SUCCESS;
}

/**
 * @brief Return the number of elements in the specified function-call array.
 * 
 * @param arr The array to get the number of elements of.
 * @return int The number of elements in the specified array.
 */
int countInDubiousFunctionCallArray(DubiousFunctionCallArray* arr){
    return arr->count;
}

/**
 * @brief Frees the memory used by the function-call array (DESTROYS the "objects" stored within the array).
 * 
 * @param arr The array to free its memory.
 */
void freeDubiousFunctionCallArray(DubiousFunctionCallArray* arr){
    for(int i = 0; i < arr->count;i++){
        for (int j = 0; i < arr->arr[i].functionParameters->count;j++)
            free(arr->arr[i].functionParameters->arr[j]);
        free(arr->arr[i].functionParameters);
        strFree(&arr->arr[i].functionName);
    }
    free(arr->arr);
    arr->count = -1;
}