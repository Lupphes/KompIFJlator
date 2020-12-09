CC=gcc -std=c11

# Compiler
COM=compiler
NAME=ifj20.out
$(COM): *.h *.c
	$(CC) -o $(NAME) main.c parser_common.c parser_builtin_functions.c scanner.c parser.c symtable.c helper.c str.c expression_analysis.c ast.c generator.c

.PHONY: clean
clean:
	rm -r $(NAME)