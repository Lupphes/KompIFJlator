/*File name: parser_builtin_functions.c -----------------------------*
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

#include "parser_common.h"
#include "symtable.h"
#include "symbol.h"
#include "helper.h"
#include "str.h"
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"

#define MAX_PARAMETERS_IN_BUILTIN_FUNCTION 3
#define MAX_RETURN_VALUES_IN_BUILTIN_FUNCTION 3

int initBuiltInFunctions(){
    int returnCode = SUCCESS;
    
    SymbolFunctionParameter _parameters[MAX_PARAMETERS_IN_BUILTIN_FUNCTION];
    DataType _returnTypes[MAX_RETURN_VALUES_IN_BUILTIN_FUNCTION];

    SymbolFunction func;
    func.builtIn = true;
    func.parameters.count = 0;
    func.parameters.variadic = false;
    func.parameters.params = (SymbolFunctionParameter*)&_parameters;
    func.returnTypes.count = 0;
    func.returnTypes.types = (DataType*)&_returnTypes;
    callAndHandleException(strInit(&func.id));
    for (int i = 0;i < MAX_PARAMETERS_IN_BUILTIN_FUNCTION;i++){
        if (strInit(&func.parameters.params[i].id) == INTERNAL_ERROR)
            for (int j = 0; j < i; j++)
                strFree(&func.parameters.params[j].id);
    }


    //inputs() (string,int)
    callAndHandleException_clean(strCopyConstString(&func.id,"inputs"));
    func.parameters.count = 0;
    func.returnTypes.count = 2;
    func.returnTypes.types[0] = TypeString;
    func.returnTypes.types[1] = TypeInt;
    callAndHandleException_clean(addFunction(&func));

    //inputi() (int, int)
    callAndHandleException_clean(strCopyConstString(&func.id,"inputi"));
    func.parameters.count = 0;
    func.returnTypes.count = 2;
    func.returnTypes.types[0] = TypeInt;
    func.returnTypes.types[1] = TypeInt;
    callAndHandleException_clean(addFunction(&func));

    //inputf() (float64, int)
    callAndHandleException_clean(strCopyConstString(&func.id,"inputf"));
    func.parameters.count = 0;
    func.returnTypes.count = 2;
    func.returnTypes.types[0] = TypeFloat64;
    func.returnTypes.types[1] = TypeInt;
    callAndHandleException_clean(addFunction(&func));

    //print(...) ()
    callAndHandleException_clean(strCopyConstString(&func.id,"print"));
    func.parameters.count = 0;
    func.parameters.variadic = true;
    func.returnTypes.count = 0;
    callAndHandleException_clean(addFunction(&func));
    func.parameters.variadic = false;

    //int2float(i int) (float64)
    callAndHandleException_clean(strCopyConstString(&func.id,"int2float"));
    func.parameters.count = 1;
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[0].id,"i"));
    func.parameters.params[0].type = TypeInt;
    func.returnTypes.count = 1;
    func.returnTypes.types[0] = TypeFloat64;
    callAndHandleException_clean(addFunction(&func));

    //float2int(f float) (int)
    callAndHandleException_clean(strCopyConstString(&func.id,"float2int"));
    func.parameters.count = 1;
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[0].id,"f"));
    func.parameters.params[0].type = TypeFloat64;
    func.returnTypes.count = 1;
    func.returnTypes.types[0] = TypeInt;
    callAndHandleException_clean(addFunction(&func));

    //len(s string) (int)
    callAndHandleException_clean(strCopyConstString(&func.id,"len"));
    func.parameters.count = 1;
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[0].id,"s"));
    func.parameters.params[0].type = TypeString;
    func.returnTypes.count = 1;
    func.returnTypes.types[0] = TypeInt;
    callAndHandleException_clean(addFunction(&func));
    
    //substr(s string, i int, n int) (string, int)
    callAndHandleException_clean(strCopyConstString(&func.id,"substr"));
    func.parameters.count = 3;
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[0].id,"s"));
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[1].id,"i"));
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[2].id,"n"));
    func.parameters.params[0].type = TypeString;
    func.parameters.params[1].type = TypeInt;
    func.parameters.params[2].type = TypeInt;
    func.returnTypes.count = 2;
    func.returnTypes.types[0] = TypeString;
    func.returnTypes.types[1] = TypeInt;
    callAndHandleException_clean(addFunction(&func));

    //ord(s string, i int) (int, int)
    callAndHandleException_clean(strCopyConstString(&func.id,"ord"));
    func.parameters.count = 2;
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[0].id,"s"));
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[1].id,"i"));
    func.parameters.params[0].type = TypeString;
    func.parameters.params[1].type = TypeInt;
    func.returnTypes.count = 2;
    func.returnTypes.types[0] = TypeInt;
    func.returnTypes.types[1] = TypeInt;
    callAndHandleException_clean(addFunction(&func));

    //chr(s string) (string, int)
    callAndHandleException_clean(strCopyConstString(&func.id,"chr"));
    func.parameters.count = 1;
    callAndHandleException_clean(strCopyConstString(&func.parameters.params[0].id,"i"));
    func.parameters.params[0].type = TypeInt;
    func.returnTypes.count = 2;
    func.returnTypes.types[0] = TypeString;
    func.returnTypes.types[1] = TypeInt;
    callAndHandleException_clean(addFunction(&func));


    CLEAN_UP:
    strFree(&func.id);
    for (int i = 0;i < MAX_PARAMETERS_IN_BUILTIN_FUNCTION;i++)
        strFree(&func.parameters.params[i].id);
    return returnCode;
}