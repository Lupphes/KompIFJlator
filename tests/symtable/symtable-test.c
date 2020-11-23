/*File name: symtable-test.c ----------------------------------------*
 |Project:    Implementace překladače imperativního jazyka IFJ20     |
 |Team:       124, varianta II                                       |
 |Authors:    Vojtěch Vlach (xvlach22)                               |
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

#include "./../../src/symtable.h"
#include <stdlib.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
	printf("Hello World.\n");

	printf("\nHashCode function tests:\n");
	char *key[] = {"Ahoj", "jak", "se", "mas"};
	string str;
	string *s = &str;
	for(int i = 0; i < 4; i++){
		strInit(s);
		strCopyConstString(s, key[i]);
		int hash = hashCode(s);
		printf("hash(%s): \t%d\n", key[i], hash);
		strFree(s);
	}





	return 0;
}
