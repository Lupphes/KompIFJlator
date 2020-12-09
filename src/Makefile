# Directory level
PROJDIR := ..
LIBDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build
TESTSDIR := $(PROJDIR)/tests

MKDIR_P = mkdir -p

CC=gcc -std=c11 
CFLAGS=-g -Wall -Werror -pedantic -Werror
DEBUGFLAG=-fsanitize=address -fno-omit-frame-pointer
WEXFLAGS=-Wextra -Wno-missing-field-initializers
VERBOSE = TRUE

# Scanner tests
SCN=scanner
SCANNEROBJ=$(SCN)-test $(SCN)-test2 $(SCN)-test3 

# Expression analysis
EXP=expression_analysis
EXPRESSOBJ=$(EXP)-test

# Symtable + inter_code
SYM=symtable
INT=inter_code

# Generator
GNR=generator
GNROBJ=$(GNR)-test

# Compiler
COM=compiler

.PHONY: directories

all: directories $(SCANNEROBJ) $(EXPRESSOBJ) $(GNROBJ) $(COM)
#symtable-test generator-test

directories: ${BUILDDIR}
${BUILDDIR}:
		${MKDIR_P} ${BUILDDIR}

# Scanner
$(SCN)-test: $(LIBDIR)/*.h $(LIBDIR)/$(SCN).h $(TESTSDIR)/$(SCN)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(SCN).c $(TESTSDIR)/$(SCN)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(SCN)/$(SCN)-test.c $(LIBDIR)/$(SCN).c $(LIBDIR)/str.c

$(SCN)-test2: $(LIBDIR)/*.h $(LIBDIR)/$(SCN).h $(TESTSDIR)/$(SCN)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(SCN).c $(TESTSDIR)/$(SCN)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(SCN)/$(SCN)-test2.c $(LIBDIR)/$(SCN).c $(LIBDIR)/str.c

$(SCN)-test3: $(LIBDIR)/*.h $(LIBDIR)/$(SCN).h $(TESTSDIR)/$(SCN)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(SCN).c $(TESTSDIR)/$(SCN)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(SCN)/$(SCN)-test3.c $(LIBDIR)/$(SCN).c $(LIBDIR)/str.c

# Expression analysis
$(EXP)-test: $(LIBDIR)/*.h $(LIBDIR)/$(EXP).h $(TESTSDIR)/$(EXP)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(EXP).c $(TESTSDIR)/$(EXP)/*.c 
	$(CC) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(EXP)/$(EXP)-test.c $(LIBDIR)/$(EXP).c $(LIBDIR)/str.c $(LIBDIR)/parser_common.c  $(LIBDIR)/symtable.c $(LIBDIR)/scanner.c $(LIBDIR)/helper.c $(LIBDIR)/parser.c $(LIBDIR)/ast.c $(LIBDIR)/parser_builtin_functions.c

# Symtable
$(SYM)-test: $(TESTSDIR)/$(SYM)/*.c $(LIBDIR)/$(SYM).h $(LIBDIR)/$(SYM).c $(LIBDIR)/*.h $(LIBDIR)/*.c
	$(CC) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(SYM)/*.c $(LIBDIR)/$(SYM).c $(LIBDIR)/str.c

# Inter-code
$(INT)-test: $(TESTSDIR)/$(INT)/*.c $(LIBDIR)/*.c $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(INT)/*.c $(LIBDIR)/$(INT).c

# Generator
$(GNR)-test: directories $(TESTSDIR)/generator/*.c $(LIBDIR)/*.c $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@ $(TESTSDIR)/generator/*.c parser_common.c parser_builtin_functions.c scanner.c parser.c symtable.c helper.c str.c expression_analysis.c ast.c generator.c

# Compiler
$(COM): directories $(LIBDIR)/*.h $(LIBDIR)/*.c 
	$(CC) $(CFLAGS) $(WEXFLAGS) -o $(BUILDDIR)/$@  main.c parser_common.c parser_builtin_functions.c scanner.c parser.c symtable.c helper.c str.c expression_analysis.c ast.c generator.c

clean:
	rm -rf $(BUILDDIR)
#