/*File name: generator.c---------------------------------------------*
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


#include "generator.h"
#include <inttypes.h>
#define STRING_BUFFER_LENGTH 4096
#define STRING_BUFFER_LENGTH_SMALL 256

int getUID(){
	static int uid = 0;
	return uid++;
}

void getUIDLabelName(char* out){
	if (sprintf(out,"&%d",getUID()) < 0)
		exit(INTERNAL_ERROR);
}

/**
 *	@brief	Generates the whole tree
 *
 *	@param	root	Root of the tree
 * 
 *	@return	SUCCESS 	If generating successful
 *	@return	INTERNAL_ERROR 	IF parameter root = NULL
 */
int generateTree(ASTRoot *root){
	if( root == NULL){
		fprintf(stderr, "ERROR!!! ROOT = NULL\n");
		return INTERNAL_ERROR;
	}

	printf("# Generated by KompIFJlator made by team 124 (Teamleader Ondřej Sloup xsloup02)\n");
	printf(".IFJcode20\n");
	printf("DEFVAR GF@BlackHole\n");
	printf("JUMP main\n");
	printf("#--------------- user funtions ---------------#\n");

	generateUserFunctions(root->userFunctions);
	generateMain(root->mainFunction);

	return SUCCESS;
}

/**
 *	@brief	Generates main function of given program
 *
 * 	@param	function 	Pointer to the function
 *
 *	@return	SUCCESS			If generating was successful
 *	@return	INTERNAL_ERROR	If param function is NULL and main does not exist
 */
int generateMain(ASTNodeFunction *function){
	if(function == NULL){
		fprintf(stderr, "missing main\n");
		return INTERNAL_ERROR;
	}

	printf("\n#------------------- main --------------------#\n");
	printf("\tLABEL main\n");
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n\n");

	generateFunctionBody(function);

	printf("\nPOPFRAME\n");
	printf("CREATEFRAME\n");
	//generateDebug();

	return SUCCESS;
}

/**
 *	@brief	Generates all user finctions by walking through linked list of functions
 *
 * 	@param	function 	Pointer to the first function
 */
void generateUserFunctions(ASTNodeFunction *function){
	while(function != NULL){
		generateFunction(function);
		function = function->next;
	}
}

/**
 *	@brief	Generates given function. (Only frame and calls generateFunctionBody)
 *
 * 	@param	function 	Pointer to the function definition. Guaranteed to NOT be NULL.
 */
void generateFunction(ASTNodeFunction *function){
	printf("WRITE string@New\\032function\\010\n\n");
	if(strCmpConstStr(&function->function->id, "main")){
		printf("LABEL %s\n", strGetStr(&function->function->id));
	}

	printf("PUSHFRAME\n\n");

	generateFunctionBody(function);

	printf("\nPOPFRAME\n");
	printf("RETURN\n");		// není náhodou RETURN už uložený v kodu ??
}

// Generates function body. Param function guaranteed to NOT be NULL.
void generateFunctionBody(ASTNodeFunction *function){
	printf("WRITE string@Hello\\032main!\\010\n");
	// definice všech promennych
	printf("#DEFVAR all of them.\n");
	


	// code 
}

// generates debug instruction to ifjcode output
void generateDebug(){
	printf("WRITE string@-------\\010\n");
	printf("BREAK\n");
	printf("WRITE string@-------\\010\n");
}

void generateFunctionCall(ASTNodeFunctionCall* call){
	if (call->function->builtIn)
		generateBuiltInFunctionCall(call);
	else
		generateUserFunctionCall(call);
}

void generateBuiltInFunctionCall(ASTNodeFunctionCall* call){
	if (!strCmpConstStr(&call->function->id,"inputs"))
		generateBuiltInInputs(call);
	else if (!strCmpConstStr(&call->function->id,"inputi"))
		generateBuiltInInputi(call);
	else if (!strCmpConstStr(&call->function->id,"inputf"))
		generateBuiltInInputf(call);
	else if (!strCmpConstStr(&call->function->id,"print"))
		generateBuiltInPrint(call);
	else if (!strCmpConstStr(&call->function->id,"int2float"))
		generateBuiltInInt2Float(call);
	else if (!strCmpConstStr(&call->function->id,"float2int"))
		generateBuiltInFloat2Int(call);
	else if (!strCmpConstStr(&call->function->id,"len"))
		generateBuiltInLen(call);
	else if (!strCmpConstStr(&call->function->id,"substr"))
		generateBuiltInSubstr(call);
	else if (!strCmpConstStr(&call->function->id,"ord"))
		generateBuiltInOrd(call);
	else if (!strCmpConstStr(&call->function->id,"chr"))
		generateBuiltInChr(call);
	else
		exit(INTERNAL_ERROR);
}

void generateUserFunctionCall(ASTNodeFunctionCall* call){
	//Pushing parameters to stack.
	for (int i = 0;i < call->parameters.count;i++){
		char buffer[STRING_BUFFER_LENGTH];
		generateTermRepresentation(call->parameters.arr[i],buffer);
		printf("PUSHS %s\n",buffer);
	}
	//Calling the function
	printf("CALL %s\n",strGetStr(&call->function->id));
	printf("POPFRAME\n");
	//Assigning the return values
	for (int i = call->lValues.count-1;i >= 0;i--){
		char buffer[STRING_BUFFER_LENGTH];
		generateVariableName(call->lValues.arr[i],buffer);
		printf("POPS %s\n",buffer);
	}
}

char* generateVariableName(const SymbolVariable* var, char* out){
	int len = var->type != TypeBlackHole ? sprintf(out,"LF@$%d",var->uid) : sprintf(out,"GF@BlackHole");
	if (len < 0)
		exit(INTERNAL_ERROR);
	return out + len;
}

char* generateIntLiteral(int64_t lit, char* out){
	int len = sprintf(out,"int@%"PRId64,lit);
	if (len < 0)
		exit(INTERNAL_ERROR);
	return out + len;
}

char* generateFloatLiteral(double lit, char* out){
	int len = sprintf(out, "float@%a",lit);
	if (len < 0)
		exit(INTERNAL_ERROR);
	return out + len;
}

char* generateStringLiteral(const char* lit,char* out){
	int len = sprintf(out, "string@");
	if (len < 0)
		exit(INTERNAL_ERROR);
	out += len;
	for(const char* i = lit; *i !='\0'; i++){
		if (*i <= 32 || *i == 35 || *i == 92){
			sprintf(out,"\\%3d",*i);
			out += 4;
		} else {
			*out = *i;
			out++;
		}
	}
	*out = '\0';
	return out;
}

char* generateTermRepresentation(const Term* term,char* out){
	switch(term->type){
		case TermFloatLiteral:
			return generateFloatLiteral(term->value.d,out);
		case TermIntegerLiteral:
			return generateIntLiteral(term->value.i,out);
		case TermStringLiteral:
			return generateStringLiteral(strGetStr(&term->value.s),out);
		case TermVariable:
			return generateVariableName(term->value.v,out);
		default:
			exit(INTERNAL_ERROR); //Shouldn't get to this line.
	}
}

void generateBuiltInPrint(ASTNodeFunctionCall* printCall){
	for (int i = 0; i <printCall->parameters.count;i++){
		char argBuffer[STRING_BUFFER_LENGTH];
		generateTermRepresentation(printCall->parameters.arr[i],argBuffer);
		printf("WRITE %s\n",argBuffer);
	}
}

void generateBuiltInInt2Float(ASTNodeFunctionCall* int2floatCall){
	char lValue[STRING_BUFFER_LENGTH];
	char rValue[STRING_BUFFER_LENGTH];
	generateVariableName(int2floatCall->lValues.arr[0],lValue);
	generateTermRepresentation(int2floatCall->parameters.arr[0],rValue);
	printf("INT2FLOAT %s %s\n",lValue,rValue);
}

void generateBuiltInFloat2Int(ASTNodeFunctionCall* float2intCall){
	char lValue[STRING_BUFFER_LENGTH];
	char rValue[STRING_BUFFER_LENGTH];
	generateVariableName(float2intCall->lValues.arr[0],lValue);
	generateTermRepresentation(float2intCall->parameters.arr[0],rValue);
	printf("FLOAT2INT %s %s\n",lValue,rValue);
}

void generateBuiltInLen(ASTNodeFunctionCall* lenCall){
	char lValue[STRING_BUFFER_LENGTH];
	char rValue[STRING_BUFFER_LENGTH];
	generateVariableName(lenCall->lValues.arr[0],lValue);
	generateTermRepresentation(lenCall->parameters.arr[0],rValue);
	printf("STRLEN %s %s\n",lValue,rValue);
}

void generateBuiltInChr(ASTNodeFunctionCall* chrCall){
	//str, int <- int<0;255>
	/*
	if (i < 0 || i > 255)
		return _, 1
	else
		return (char)i, 0
	*/
	/*
	PUSHS int@255
	PUSHS LF@i
	GTS
	PUSHS int@0
	PUSHS LF@i
	LTS 
	ORS
	PUSHS bool@true
	JUMPIFNEQS &UID_ELSE
	MOVE LF@R2 int@1
	JUMP &UID_END
	LABEL &UID_ELSE
	INT2CHAR LF@R1 LF@i
	MOVE LF@R2 int@0
	LABEL &UID_END
	*/
	char output[STRING_BUFFER_LENGTH_SMALL];
	char errorCode[STRING_BUFFER_LENGTH_SMALL];
	char input[STRING_BUFFER_LENGTH_SMALL];
	char label[STRING_BUFFER_LENGTH_SMALL];
	generateVariableName(chrCall->lValues.arr[0],output);
	generateVariableName(chrCall->lValues.arr[1],errorCode);
	generateTermRepresentation(chrCall->parameters.arr[0],input);
	getUIDLabelName(label);
	printf("PUSHS int@255\n");
	printf("PUSHS %s\n",input);
	printf("GTS");
	printf("PUSHS int@0\n");
	printf("PUSHS %s\n",input);
	printf("LTS\n");
	printf("ORS\n");
	printf("PUSHS bool@true\n");
	printf("JUMPIFNEQS %s_ELSE\n",label);
	printf("MOVE %s int@1\n",errorCode);
	printf("JUMP %s_END",label);
	printf("LABEL %s_ELSE\n",label);
	printf("INT2CHAR %s %s",output,input);
	printf("MOVE %s int@0\n",errorCode);
	printf("LABEL %s_END",label);
}

void generateBuiltInOrd(ASTNodeFunctionCall* ordCall){
	//int, int <- str, i<0;len(str)-1>
	/*
	if (i > -1 && i < len(str))
		return (char)str[i], 0
	else
		return _, 1
	*/
	char output[STRING_BUFFER_LENGTH_SMALL];
	char errorCode[STRING_BUFFER_LENGTH_SMALL];
	char inputString[STRING_BUFFER_LENGTH_SMALL];
	char inputIndex[STRING_BUFFER_LENGTH_SMALL];
	char label[STRING_BUFFER_LENGTH_SMALL];
	generateVariableName(ordCall->lValues.arr[0],output);
	generateVariableName(ordCall->lValues.arr[1],errorCode);
	generateTermRepresentation(ordCall->parameters.arr[0],inputString);
	generateTermRepresentation(ordCall->parameters.arr[1],inputIndex);
	getUIDLabelName(label);
	
	printf("CREATEFRAME");
	printf("DEFVAR TF@len");
	printf("STRLEN TF@len %s\n",inputString);
	printf("PUSHS int@len\n");
	printf("PUSHS %s\n",inputIndex);
	printf("LTS");
	printf("PUSHS int@-1\n");
	printf("PUSHS %s\n",inputIndex);
	printf("GTS\n");
	printf("ANDS\n");
	printf("PUSHS bool@true\n");
	printf("JUMPIFNEQS %s_ELSE\n",label);
		
		printf("STR2INT %s %s %s",output,inputString,inputIndex);
		printf("MOVE %s int@1\n",errorCode);
		printf("JUMP %s_END",label);

		printf("LABEL %s_ELSE\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END",label);
}

void generateBuiltInSubstr(ASTNodeFunctionCall* substrCall){
	char output[STRING_BUFFER_LENGTH_SMALL];
	char errorCode[STRING_BUFFER_LENGTH_SMALL];
	char inputString[STRING_BUFFER_LENGTH_SMALL];
	char inputIndex[STRING_BUFFER_LENGTH_SMALL];
	char inputCount[STRING_BUFFER_LENGTH_SMALL];
	char label[STRING_BUFFER_LENGTH_SMALL];
	generateVariableName(substrCall->lValues.arr[0],output);
	generateVariableName(substrCall->lValues.arr[1],errorCode);
	generateTermRepresentation(substrCall->parameters.arr[0],inputString);
	generateTermRepresentation(substrCall->parameters.arr[1],inputIndex);
	generateTermRepresentation(substrCall->parameters.arr[2],inputCount);
	getUIDLabelName(label);


	printf("CREATEFRAME\n");
	printf("DEFVAR TF@len\n");
	printf("STRLEN TF@len %s\n",inputString);
	printf("PUSHS int@-1\n");
	printf("PUSHS %s\n",inputCount);
	printf("GTS\n");
	printf("PUSHS TF@len\n");
	printf("PUSHS %s\n",inputIndex);
	printf("LTS\n");
	printf("PUSHS int@-1\n");
	printf("PUSHS %s\n",inputIndex);
	printf("GTS\n");
	printf("ANDS\n");
	printf("ANDS\n");
	printf("PUSHS bool@true\n");
	printf("JUMPIFNEQS %s_ELSE\n",label);

		printf("DEFVAR TF@end\n");
		printf("PUSHS %s\n",inputIndex);
		printf("PUSHS %s\n",inputCount);
		printf("ADDS\n");
		printf("POPS TF@end\n");
		printf("PUSHS TF@len\n");
		printf("PUSHS TF@end\n");
		printf("GTS\n");
		printf("PUSHS bool@true\n");
		printf("JUMPIFNEQS %s_ForInit\n",label);
		printf("MOVE TF@end TF@len\n");
		
			printf("LABEL %s_ForInit\n",label);
			printf("DEFVAR TF@i\n");
			printf("DEFVAR TF@strBuf\n");
			printf("MOVE %s string@\n",output);
			printf("MOVE %s int@0\n",errorCode);
			printf("MOVE TF@i %s\n",inputIndex);
			
			printf("LABEL %s_ForCheck\n",label);
			printf("PUSHS TF@end\n");
			printf("PUSHS TF@i\n");
			printf("LTS\n");
			printf("PUSHS bool@true\n");
			printf("JUMPIFNEQS %s_END\n",label);

			printf("GETCHAR TF@strBuf TF@i\n");
			printf("CONCAT %s %s TF@strBuf\n",output,output);
			printf("ADD TF@i TF@i int@1\n");
			printf("JUMP %s_ForCheck\n",label);


		printf("LABEL %s_ELSE\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END",label);

}

void generateBuiltInInputs(ASTNodeFunctionCall* inputsStatment){
	char output[STRING_BUFFER_LENGTH_SMALL];
	char errorCode[STRING_BUFFER_LENGTH_SMALL];
	char label[STRING_BUFFER_LENGTH_SMALL];
	generateVariableName(inputsStatment->lValues.arr[0],output);
	generateVariableName(inputsStatment->lValues.arr[1],errorCode);
	getUIDLabelName(label);

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@out\n");
	printf("READ TF@out string\n");
	printf("JUMPIFEQ %s_ERROR TF@out nil@nil\n",label);

		printf("MOVE %s TF@out\n",output);
		printf("MOVE %s int@0\n",errorCode);
		printf("JUMP %s_END",label);

		printf("LABEL %s_ERROR\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END\n",label);
}

void generateBuiltInInputi(ASTNodeFunctionCall* inputiStatment){
	char output[STRING_BUFFER_LENGTH_SMALL];
	char errorCode[STRING_BUFFER_LENGTH_SMALL];
	char label[STRING_BUFFER_LENGTH_SMALL];
	generateVariableName(inputiStatment->lValues.arr[0],output);
	generateVariableName(inputiStatment->lValues.arr[1],errorCode);
	getUIDLabelName(label);

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@out\n");
	printf("READ TF@out int\n");
	printf("JUMPIFEQ %s_ERROR TF@out nil@nil\n",label);

		printf("MOVE %s TF@out\n",output);
		printf("MOVE %s int@0\n",errorCode);
		printf("JUMP %s_END",label);

		printf("LABEL %s_ERROR\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END\n",label);
}

void generateBuiltInInputf(ASTNodeFunctionCall* inputfStatment){
	char output[STRING_BUFFER_LENGTH_SMALL];
	char errorCode[STRING_BUFFER_LENGTH_SMALL];
	char label[STRING_BUFFER_LENGTH_SMALL];
	generateVariableName(inputfStatment->lValues.arr[0],output);
	generateVariableName(inputfStatment->lValues.arr[1],errorCode);
	getUIDLabelName(label);

	printf("CREATEFRAME\n");
	printf("DEFVAR TF@out\n");
	printf("READ TF@out float\n");
	printf("JUMPIFEQ %s_ERROR TF@out nil@nil\n",label);

		printf("MOVE %s TF@out\n",output);
		printf("MOVE %s int@0\n",errorCode);
		printf("JUMP %s_END",label);

		printf("LABEL %s_ERROR\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END\n",label);
}