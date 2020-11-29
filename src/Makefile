# Directory level
PROJDIR := ..
LIBDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build
TESTSDIR := $(PROJDIR)/tests

MKDIR_P = mkdir -p

CC=gcc
CFLAGS=-g -Wall -Werror -pedantic -std=c11
WEXFLAGS=-Wextra -Wno-missing-field-initializers
VERBOSE = TRUE

# Scanner Tests
PRJ=scanner
PRJ2=parser
SCANNEROBJ=$(PRJ)-test $(PRJ)-test2 $(PRJ)-test3 $(PRJ2)-test

# Symtable
SYMTABLE=symtable

.PHONY: directories

all: directories $(SCANNEROBJ) symtable-test

directories: ${BUILDDIR}
${BUILDDIR}:
		${MKDIR_P} ${BUILDDIR}

$(PRJ)-test: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

$(PRJ)-test2: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test2.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

$(PRJ)-test3: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(WEXFLAGS) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test3.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

# Parser

$(PRJ2)-test: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ2).h $(TESTSDIR)/$(PRJ2)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ2).c $(TESTSDIR)/$(PRJ2)/*.c 
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ2)/$(PRJ2)-test.c $(LIBDIR)/$(PRJ2).c $(LIBDIR)/str.c
$(SYMTABLE)-test:	$(TESTSDIR)/$(SYMTABLE)/*.c $(LIBDIR)/$(SYMTABLE).h $(LIBDIR)/$(SYMTABLE).c $(LIBDIR)/*.h $(LIBDIR)/*.c
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(SYMTABLE)/*.c $(LIBDIR)/$(SYMTABLE).c $(LIBDIR)/str.c

clean:
	rm -rf $(BUILDDIR)
#