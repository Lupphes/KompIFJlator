# Directory level
PROJDIR := ..
LIBDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build
TESTSDIR := $(PROJDIR)/tests

MKDIR_P = mkdir -p

CC=gcc
CFLAGS=-g -Wall -Werror -pedantic -std=c11 #-Werror
DEBUGFLAG=-fsanitize=address -fno-omit-frame-pointer
WEXFLAGS=-Wextra -Wno-missing-field-initializers
VERBOSE = TRUE
# -Werror
# Scanner Tests
PRJ=scanner
PRJ3=expression_analysis
SCANNEROBJ=$(PRJ)-test $(PRJ)-test2 $(PRJ)-test3 $(PRJ3)-test

# Symtable + inter_code
SYMTABLE=symtable
INTER_CODE=inter_code

.PHONY: directories

all: directories $(SCANNEROBJ) compiler #
#symtable-test generator-test

directories: ${BUILDDIR}
${BUILDDIR}:
		${MKDIR_P} ${BUILDDIR}

$(PRJ)-test: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

$(PRJ)-test2: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test2.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

$(PRJ)-test3: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test3.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

$(PRJ3)-test: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ3).h $(TESTSDIR)/$(PRJ3)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ3).c $(TESTSDIR)/$(PRJ3)/*.c 
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ3)/$(PRJ3)-test.c $(LIBDIR)/$(PRJ3).c $(LIBDIR)/str.c $(LIBDIR)/parser_common.c  $(LIBDIR)/symtable.c $(LIBDIR)/scanner.c $(LIBDIR)/helper.c $(LIBDIR)/parser.c $(LIBDIR)/ast.c $(LIBDIR)/parser_builtin_functions.c $(LIBDIR)/generator.c

# $(SYMTABLE)-test:	$(TESTSDIR)/$(SYMTABLE)/*.c $(LIBDIR)/$(SYMTABLE).h $(LIBDIR)/$(SYMTABLE).c $(LIBDIR)/*.h $(LIBDIR)/*.c
# 	$(CC) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(SYMTABLE)/*.c $(LIBDIR)/$(SYMTABLE).c $(LIBDIR)/str.c

$(INTER_CODE)-test:	$(TESTSDIR)/$(INTER_CODE)/*.c $(LIBDIR)/*.c $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(INTER_CODE)/*.c $(LIBDIR)/$(INTER_CODE).c

generator-test: directories $(TESTSDIR)/generator/*.c $(LIBDIR)/*.c $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@ $(TESTSDIR)/generator/*.c parser_common.c parser_builtin_functions.c scanner.c parser.c symtable.c helper.c str.c expression_analysis.c ast.c generator.c

compiler: directories $(LIBDIR)/*.h $(LIBDIR)/*.c 
	$(CC) $(CFLAGS) $(WEXFLAGS) -o $(BUILDDIR)/$@  main.c parser_common.c parser_builtin_functions.c scanner.c parser.c symtable.c helper.c str.c expression_analysis.c ast.c generator.c

clean:
	rm -rf $(BUILDDIR)
#