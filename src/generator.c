/*File name: generator.c---------------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Vojtěch Vlach (xvlach22)                               |
 |            Viktor Rucký (xrucky01)                                |
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
#include "expression_analysis.h"
#include <inttypes.h>
#define STRING_BUFFER_LENGTH 4096
#define STRING_BUFFER_LENGTH_SMALL 256

/**
 * @brief	Increment counter of IDs and return actual value.
 *			Ids used only to name ifjcode labels
 *
 * @return 	Return unique ID
 */
int getUID(){
	static int uid = 0;
	return uid++;
}

/**
 * @brief	Create unique ifjcode label name out of '&' and unique ID.
 *			Used for jumping between blocks of ifjcode code.
 *
 * @param 	out 	Pointer to initialized string (char* or char[]) 
 *					where the output is saved. Needs to have enough memory allocated. 
 */
void getUIDLabelName(char* out){
	if (sprintf(out,"&%d",getUID()) < 0)
		exit(INTERNAL_ERROR);
}

/**
 *	@brief	Generate the whole syntax tree (AST) to ifjcode. (print to stdout)
 *
 *	@param	root	Root of the syntax tree (AST)
 * 
 *	@return	SUCCESS 	If generating successful
 *	@return	INTERNAL_ERROR 	IF parameter root = NULL
 */
int generateTree(ASTRoot *root){
	if( root == NULL){
		fprintf(stderr, "ERROR!!! ROOT = NULL\n");
		return INTERNAL_ERROR;
	}

	// generate Header of the output file
	printf("# Generated by KompIFJlator made by team 124 (Teamleader Ondřej Sloup xsloup02)\n");
	printf(".IFJcode20\n");
	printf("DEFVAR GF@BlackHole\n");
	printf("CALL main\n");
	printf("EXIT int@0\n");
	printf("#--------------- user funtions ---------------#\n");

	generateUserFunctions(root->userFunctions);
	generateUserFunctions(root->mainFunction);

	return SUCCESS;
}

/**
 *	@brief	Generate all user functions by walking through linked list of functions. 
 * 			Handel function parametrs right to left.
 *
 * 	@param	function 	Pointer to the first function in the list
 */
void generateUserFunctions(ASTNodeFunction *function){
	while(function != NULL){
		// generate label + initial frame
		printf("\tLABEL %s\n", strGetStr(&function->function->id));
		printf("CREATEFRAME\n");
		printf("PUSHFRAME\n\n");

		// Generate Definitions Of Variables
		printf("# Define all variables of function.\n");

		for(int i = 0; i < function->variables.count; i++){
			if(function->variables.arr[i] != NULL){
				char newVarName[STRING_BUFFER_LENGTH];
				generateVariableName(function->variables.arr[i], newVarName);
				printf("DEFVAR %s\n", newVarName);
			}
		}

		// generate handling parameters (right to left)
		for(int i = (function->function->parameters.count - 1); i >= 0; --i){
			// foreach parameter
			// definition
			char newVarName[STRING_BUFFER_LENGTH];
			generateVariableName(function->variables.arr[i], newVarName);

			// pop value from stack
			printf("POPS %s\n", newVarName);
		}

		// generate function body
		printf("\n# Code of this function.\n");
		generateFunctionCodeBlock(function->code->firstStatement);

		// move to the next function
		function = function->next;
	}
}

/**
 * @brief	Generate code block of the function. Go through all the statements 
 *			in the linked list.
 *
 * @param 	codeStmnt	Pointer to a first statement in the list
 */
void generateFunctionCodeBlock(ASTNodeStatement* codeStmnt){
	// go through all codeStatments and generate them
	while(codeStmnt != NULL){
		// generate this codeStatment
		switch(codeStmnt->type){
			case StatementTypeAssignment:
				generateAssignment(&codeStmnt->value.assignment);
				break;
			case StatementTypeFunctionCall:
				generateFunctionCall(&codeStmnt->value.functionCall);
				break;
			case StatementTypeIf:
				generateIf(&codeStmnt->value.ifStatement);
				break;
			case StatementTypeFor:
				generateFor(&codeStmnt->value.forStatement);
				break;
			case StatementTypeReturn:
				generateReturn(&codeStmnt->value.returnStatement);
				break;
			default:
				fprintf(stderr, "Unknown ASTStatementType: %d\n", codeStmnt->type);
				break;
		}

		// move to the next code statement
		codeStmnt = codeStmnt->next;
	}
}

/**
 * @brief	Generate operation (+, -, *, /, >, <, =, !=, >=, <=)
 *
 * @param 	opType 		Type of the operation.
 * @param 	dataType 	Type of the output of the operation
 */
void generateOperation(OperationType opType, DataType dataType){
	char buffer[STRING_BUFFER_LENGTH_SMALL];
	switch(opType){
		case OperationAdd:
			if (dataType == TypeString){
				printf("CREATEFRAME\n");
				printf("DEFVAR TF@left\n");
				printf("DEFVAR TF@right\n");
				printf("POPS TF@right\n");
				printf("POPS TF@left\n");
				printf("CONCAT TF@left TF@left TF@right\n");
				printf("PUSHS TF@left\n");
			} else {
				printf("ADDS\n");
			}
			break;
		case OperationSub:
			printf("SUBS\n");
			break;
		case OperationMul:
			printf("MULS\n");
			break;
		case OperationDiv:
			getUIDLabelName(buffer);
			printf("POPS GF@BlackHole\n");
			dataType == TypeInt ? printf("JUMPIFNEQ %s GF@BlackHole int@0\n",buffer) : printf("JUMPIFNEQ %s GF@BlackHole float@0x0p+0\n",buffer);
			printf("EXIT int@9\n");
			printf("LABEL %s\n",buffer);
			printf("PUSHS GF@BlackHole\n");
			dataType == TypeInt ? printf("IDIVS\n") : printf("DIVS\n");
			break;
		case OperationUnS:
			printf("POPS GF@BlackHole\n");
			dataType == TypeInt ? printf("PUSHS int@0\n") : printf("PUSHS float@0x0p+0\n");
			printf("PUSHS GF@BlackHole\n");
			printf("SUBS\n");
			break;
		case OperationGth:
			printf("GTS\n");
			break;
		case OperationLes:
			printf("LTS\n");
			break;
		case OperationEqu:
			printf("EQS\n");
			break;
		case OperationNEq:
			printf("EQS\n");
			printf("NOTS\n");
			break;
		case OperationGEq:
			printf("LTS\n");
			printf("NOTS\n");
			break;
		case OperationLEq:
			printf("GTS\n");
			printf("NOTS\n");
			break;
		case OperationPar:
		case OperationUnA:
			break;
	}
}

/**
 * @brief	Generate expression. Atom or operation (unary and binary). Recursive. Push result value to stack.
 *
 * @param 	exp		Expression to be generated.
 */
void generateExpresion(ExpExp* exp){
	char buffer[STRING_BUFFER_LENGTH];
	switch(exp->type){
		case ExpExpAtom:
			generateTermRepresentation(&exp->ExpProperties.atom,buffer);
			printf("PUSHS %s\n",buffer);
			break;
		case ExpExpOperation:
			if (isBinaryOperation(exp->ExpProperties.operation.type)){
				generateExpresion(exp->ExpProperties.operation.value.binary.first);
				generateExpresion(exp->ExpProperties.operation.value.binary.second);
			} else {
				generateExpresion(exp->ExpProperties.operation.value.unary.first);
			}
			generateOperation(exp->ExpProperties.operation.type,exp->dataType);
			break;
	}
}

/**
 * @brief	Generate variable name to format "LF@$<uid>". 
 * 			Use uid stored in the variable definition.
 *
 * @param 	var 	Variable defintion whose name is being generated
 * @param 	out 	Pointer to initialized string (char* or char[]) 
 *					where the output is saved. Needs to have enough memory allocated.
 *
 * @return 	Pointer to the end of the generated name in parameter out.
 */
char* generateVariableName(const SymbolVariable* var, char* out){
	int len = var->type != TypeBlackHole ? sprintf(out,"LF@$%d",var->uid) : sprintf(out,"GF@BlackHole");
	if (len < 0)
		exit(INTERNAL_ERROR);
	return out + len;
}

/**
 * @brief	Generate debug instruction to ifjcode output
 */
void generateDebug(){
	printf("WRITE string@-------\\010\n");
	printf("BREAK\n");
	printf("WRITE string@-------\\010\n");
}

/**
 * @brief	Generate function call. Decide if it calls ifj20 built-in function or 
 * 			a user created function.
 *
 * @param 	call 	Pointer to FunctionCall statement.
 */
void generateFunctionCall(ASTNodeFunctionCall* call){
	if (call->function->builtIn)
		generateBuiltInFunctionCall(call);
	else
		generateUserFunctionCall(call);
}

/**
 * @brief	Generate ifj20 builtIn function. Decide which one of them it is according to function id.
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored.
 */
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

/**
 * @brief	Generate a user created function. Push parameters to stack left to right.
 *			Pop return values from stack left to right
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 *					(parameters expression to push to stack and symbols of variables to pop return value to)
 */
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
	for (int i = 0 ;i < call->lValues.count;i++){
		char buffer[STRING_BUFFER_LENGTH];
		generateVariableName(call->lValues.arr[i],buffer);
		printf("POPS %s\n",buffer);
	}
}

/**
 * @brief	Generates ifjcode int literal term.
 *
 * @param 	lit 	Integer value to be generated.
 * @param 	out 	Pointer to initialized string (char* or char[]) 
 *					where the output is saved. Needs to have enough memory allocated. 
 *
 * @return 	Pointer to the end of the generated string in parameter out.
 */
char* generateIntLiteral(int64_t lit, char* out){
	int len = sprintf(out,"int@%"PRId64,lit);
	if (len < 0)
		exit(INTERNAL_ERROR);
	return out + len;
}

/**
 * @brief	Generates ifjcode float literal term.
 *
 * @param 	lit 	Double value to be generated.
 * @param 	out 	Pointer to initialized string (char* or char[]) 
 *					where the output is saved. Needs to have enough memory allocated. 
 *
 * @return 	Pointer to the end of the generated string in parameter out.
 */
char* generateFloatLiteral(double lit, char* out){
	int len = sprintf(out, "float@%a",lit);
	if (len < 0)
		exit(INTERNAL_ERROR);
	return out + len;
}

/**
 * @brief	Generates ifjcode string literal term.
 *
 * @param 	lit 	String value to be generated.
 * @param 	out 	Pointer to initialized string (char* or char[]) 
 *					where the output is saved. Needs to have enough memory allocated. 
 *
 * @return 	Pointer to the end of the generated string in parameter out.
 */
char* generateStringLiteral(const char* lit,char* out){
	int len = sprintf(out, "string@");
	if (len < 0)
		exit(INTERNAL_ERROR);
	out += len;
	for(const char* i = lit; *i !='\0'; i++){
		if (*i <= 32 || *i == 35 || *i == 92){
			sprintf(out,"\\%03d",*i);
			out += 4;
		} else {
			*out = *i;
			out++;
		}
	}
	*out = '\0';
	return out;
}

/**
 * @brief	Decide what type the term is and generate it by calling proper method.
 *
 * @param 	term	Pointer to the terminal (term) to be generated.
 * @param 	out 	Pointer to initialized string (char* or char[]) 
 *					where the output is saved. Needs to have enough memory allocated. 
 *
 * @return 	Pointer to the end of the generated string in parameter out.
 */
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

/**
 * @brief	Generate ifj20 builtIn function print. Write all terms.
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 *					(terms to print)
 */
void generateBuiltInPrint(ASTNodeFunctionCall* printCall){
	for (int i = 0; i <printCall->parameters.count;i++){
		char argBuffer[STRING_BUFFER_LENGTH];
		generateTermRepresentation(printCall->parameters.arr[i],argBuffer);
		printf("WRITE %s\n",argBuffer);
	}
}

/**
 * @brief	Generate ifj20 builtIn function int2float.
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
void generateBuiltInInt2Float(ASTNodeFunctionCall* int2floatCall){
	char lValue[STRING_BUFFER_LENGTH];
	char rValue[STRING_BUFFER_LENGTH];
	generateVariableName(int2floatCall->lValues.arr[0],lValue);
	generateTermRepresentation(int2floatCall->parameters.arr[0],rValue);
	printf("INT2FLOAT %s %s\n",lValue,rValue);
}

/**
 * @brief	Generate ifj20 builtIn function float2int. 
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
void generateBuiltInFloat2Int(ASTNodeFunctionCall* float2intCall){
	char lValue[STRING_BUFFER_LENGTH];
	char rValue[STRING_BUFFER_LENGTH];
	generateVariableName(float2intCall->lValues.arr[0],lValue);
	generateTermRepresentation(float2intCall->parameters.arr[0],rValue);
	printf("FLOAT2INT %s %s\n",lValue,rValue);
}

/**
 * @brief	Generate ifj20 builtIn function Len. 
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
void generateBuiltInLen(ASTNodeFunctionCall* lenCall){
	char lValue[STRING_BUFFER_LENGTH];
	char rValue[STRING_BUFFER_LENGTH];
	generateVariableName(lenCall->lValues.arr[0],lValue);
	generateTermRepresentation(lenCall->parameters.arr[0],rValue);
	printf("STRLEN %s %s\n",lValue,rValue);
}

/**
 * @brief	Generate ifj20 builtIn function chr. 
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
void generateBuiltInChr(ASTNodeFunctionCall* chrCall){
	char output[STRING_BUFFER_LENGTH_SMALL];
	char errorCode[STRING_BUFFER_LENGTH_SMALL];
	char input[STRING_BUFFER_LENGTH_SMALL];
	char label[STRING_BUFFER_LENGTH_SMALL];
	generateVariableName(chrCall->lValues.arr[0],output);
	generateVariableName(chrCall->lValues.arr[1],errorCode);
	generateTermRepresentation(chrCall->parameters.arr[0],input);
	getUIDLabelName(label);
	printf("PUSHS %s\n",input);
	printf("PUSHS int@255\n");
	printf("GTS\n");
	printf("PUSHS %s\n",input);
	printf("PUSHS int@0\n");
	printf("LTS\n");
	printf("ORS\n");
	printf("PUSHS bool@true\n");
	printf("JUMPIFNEQS %s_ELSE\n",label);
		printf("MOVE %s int@1\n",errorCode);
		printf("JUMP %s_END\n",label);
	printf("LABEL %s_ELSE\n",label);
		printf("INT2CHAR %s %s\n",output,input);
		printf("MOVE %s int@0\n",errorCode);
	printf("LABEL %s_END\n",label);
}

/**
 * @brief	Generate ifj20 builtIn function ord. 
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
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
	
	printf("CREATEFRAME\n");
	printf("DEFVAR TF@len\n");
	printf("STRLEN TF@len %s\n",inputString);
	printf("PUSHS %s\n",inputIndex);
	printf("PUSHS TF@len\n");
	printf("LTS\n");
	printf("PUSHS %s\n",inputIndex);
	printf("PUSHS int@-1\n");
	printf("GTS\n");
	printf("ANDS\n");
	printf("PUSHS bool@true\n");
	printf("JUMPIFNEQS %s_ELSE\n",label);
		
		printf("STRI2INT %s %s %s\n",output,inputString,inputIndex);
		printf("MOVE %s int@0\n",errorCode);
		printf("JUMP %s_END\n",label);

		printf("LABEL %s_ELSE\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END\n",label);
}

/**
 * @brief	Generate ifj20 builtIn function substr. 
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
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
	printf("PUSHS %s\n",inputCount);
	printf("PUSHS int@-1\n");
	printf("GTS\n");
	printf("PUSHS TF@len\n");
	printf("PUSHS %s\n",inputIndex);
	printf("GTS\n");
	printf("PUSHS %s\n",inputIndex);
	printf("PUSHS int@-1\n");
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
		printf("JUMPIFEQS %s_ForInit\n",label);
		printf("MOVE TF@end TF@len\n");
		
			printf("LABEL %s_ForInit\n",label);
			printf("DEFVAR TF@i\n");
			printf("DEFVAR TF@strBuf\n");
			printf("DEFVAR TF@outBuf\n");
			printf("MOVE TF@outBuf string@\n");
			printf("MOVE %s int@0\n",errorCode);
			printf("MOVE TF@i %s\n",inputIndex);
			
			printf("LABEL %s_ForCheck\n",label);
			printf("PUSHS TF@i\n");
			printf("PUSHS TF@end\n");
			printf("LTS\n");
			printf("PUSHS bool@true\n");
			printf("JUMPIFNEQS %s_FLUSH_BUFFER\n",label);

			printf("GETCHAR TF@strBuf %s TF@i\n",inputString);
			printf("CONCAT TF@outBuf TF@outBuf TF@strBuf\n");
			printf("ADD TF@i TF@i int@1\n");
			printf("JUMP %s_ForCheck\n",label);


		printf("LABEL %s_ELSE\n",label);
		printf("MOVE %s int@1\n",errorCode);
		printf("JUMP %s_END\n",label);
	
	printf("LABEL %s_FLUSH_BUFFER\n",label);
	printf("MOVE %s TF@outBuf\n",output);
	printf("LABEL %s_END\n",label);
}

/**
 * @brief	Generate ifj20 builtIn function inputs. 
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
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
		printf("JUMP %s_END\n",label);

		printf("LABEL %s_ERROR\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END\n",label);
}

/**
 * @brief	Generate ifj20 builtIn function inputi. 
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
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
		printf("JUMP %s_END\n",label);

		printf("LABEL %s_ERROR\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END\n",label);
}

/**
 * @brief	Generate ifj20 builtIn function inputf. 
 *
 * @param 	call 	Pointer to FunctionCall statement where all the needed data is stored. 
 */
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
		printf("JUMP %s_END\n",label);

		printf("LABEL %s_ERROR\n",label);
		printf("MOVE %s int@1\n",errorCode);
	
	printf("LABEL %s_END\n",label);
}

/**
 * @brief	Generate assignment n right values to n left values. (eg. i, _ = input(i))
 *
 * @param 	assignment 	Pointer to assignment which contains array of right values (expressions) 
 *			and left values (symbolVariables).
 */
void generateAssignment(ASTNodeAssignment *assignment){
	if(assignment == NULL)
		return;

	// save the smaller number from these two
	int valuesCount = (assignment->lValues.count < assignment->rValues.count) ? assignment->lValues.count : assignment->rValues.count ;

	// push rValue to stack right to left
	for(int i = (valuesCount - 1); i >= 0; i--){
		generateExpresion(assignment->rValues.arr[i]);
	}
	
	// pop it to lValue left to right
	for(int i = 0; i < valuesCount;i++){
		char lValueName[STRING_BUFFER_LENGTH];
		generateVariableName(assignment->lValues.arr[i], lValueName);
		printf("POPS %s\n", lValueName);
	}
}

/**
 * @brief	Generate if structure.
 *
 * @param 	ifStatement 	Pointer to a ifStatement which cointains all data that needs to be generated.
 *							(condition, if clause, else clause)
 */
void generateIf(ASTNodeIf* ifStatement){
	if (ifStatement == NULL)
		return;

	// prepare unique labels
	char elseLabel[STRING_BUFFER_LENGTH];
	char endIfLabel[STRING_BUFFER_LENGTH];
	getUIDLabelName(elseLabel);
	getUIDLabelName(endIfLabel);

	// generate condition 
	generateExpresion(ifStatement->condition);
	printf("PUSHS bool@false\n");	// condition negation for eliminating redundant jumps
	printf("\nJUMPIFEQS %s\n", elseLabel);

	// generate if clause and jump to endIfLabel
	generateFunctionCodeBlock(ifStatement->ifClause->firstStatement);
	printf("JUMP %s\n", endIfLabel);

	// generate elseLabel
	printf("\tLABEL %s\n", elseLabel);

	// generate else clause
	generateFunctionCodeBlock(ifStatement->elseClause->firstStatement);

	// generate endIfLabel
	printf("\tLABEL %s\n", endIfLabel);
}

/**
 * @brief	Generate for structure.
 *
 * @param 	forStatement 	Pointer to a forStatement which cointains all data that needs to be generated.
 *							(initial assignment, condition, increment assignment, inside code)
 */
void generateFor(ASTNodeFor* forStatement){
	if (forStatement == NULL)
		return;

	// prepare unique labels
	char forLabel[STRING_BUFFER_LENGTH];
	char endForLabel[STRING_BUFFER_LENGTH];
	getUIDLabelName(forLabel);
	getUIDLabelName(endForLabel);

	// initial assignment 	(eg. i := 0)
	generateAssignment(forStatement->initAssignment);

	// LABEL for
	printf("\tLABEL %s\n", forLabel);

	// for condition	(eg. i < 5)
	generateExpresion(forStatement->condition);
	printf("PUSHS bool@false\n");	// condition negation for eliminating redundant jumps
	printf("\nJUMPIFEQS %s\n", endForLabel);

    // generate code inside of for   struct _ASTNodeBlock* code;
    generateFunctionCodeBlock(forStatement->code->firstStatement);

	// generate incrementation
    // ASTNodeAssignment* incrementAssignment;
    generateAssignment(forStatement->incrementAssignment);

	// JUMP for
	printf("JUMP %s\n", forLabel);

	// generate label endfor
	printf("\tLABEL %s\n", endForLabel);
}

/**
 * @brief	Generate return statement. Push return values to stack right to left
 *
 * @param 	returnStatement 	Pointer to a returnStatement which cointains all data that needs to be generated.
 *								(array of expression)
 */
void generateReturn(ASTNodeReturn* returnStatement){
	if (returnStatement == NULL)
		return;

	// generate all return expressions and PUSH them to STACK (right-to-left)
	for (int i = (returnStatement->rValues.count-1); i >= 0; i--){
		generateExpresion(returnStatement->rValues.arr[i]);
	}

	// generate return
	printf("RETURN\n");
}
