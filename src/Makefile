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
NAME=ifj20.out

# Compiler
$(COM): $(LIBDIR)/*.h $(LIBDIR)/*.c 
	$(CC) -o $(NAME) $<

.PHONY: all
all: directories $(SCANNEROBJ) $(EXPRESSOBJ) $(SYM)-test $(INT)-test $(GNROBJ) $(COM)

directories: ${BUILDDIR}
${BUILDDIR}:
		${MKDIR_P} ${BUILDDIR}

# Scanner
$(SCN)-test: $(LIBDIR)/*.h $(LIBDIR)/$(SCN).h $(TESTSDIR)/$(SCN)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(SCN).c $(TESTSDIR)/$(SCN)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $<

$(SCN)-test2: $(LIBDIR)/*.h $(LIBDIR)/$(SCN).h $(TESTSDIR)/$(SCN)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(SCN).c $(TESTSDIR)/$(SCN)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $<

$(SCN)-test3: $(LIBDIR)/*.h $(LIBDIR)/$(SCN).h $(TESTSDIR)/$(SCN)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(SCN).c $(TESTSDIR)/$(SCN)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $<

# Expression analysis
$(EXPRESSOBJ): $(LIBDIR)/*.h $(LIBDIR)/$(EXP).h $(TESTSDIR)/$(EXP)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(EXP).c $(TESTSDIR)/$(EXP)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $<

# Symtable
$(SYM)-test: $(LIBDIR)/*.h $(LIBDIR)/$(SYM).h $(LIBDIR)/*.c $(LIBDIR)/$(SYM).c $(TESTSDIR)/$(SYM)/*.c
	$(CC) $(CFLAGS) $(DEBUGFLAG) -o $(BUILDDIR)/$@ $<

# Inter-code
$(INT)-test: $(LIBDIR)/*.h $(LIBDIR)/*.c $(TESTSDIR)/$(INT)/*.c
	$(CC) $(WEXFLAGS) $(CFLAGS) -o $(BUILDDIR)/$@ $<

# Generator
$(GNROBJ): $(LIBDIR)/*.h $(LIBDIR)/*.c $(TESTSDIR)/$(GNR)/*.c
	$(CC) $(WEXFLAGS) $(CFLAGS) -o $(BUILDDIR)/$@ $<

.PHONY: clean-test
clean-test:
	rm -rf $(BUILDDIR)

.PHONY: clean
clean:
	rm -r $(NAME)