# Directory level
PROJDIR := ..
LIBDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build
TESTSDIR := $(PROJDIR)/tests

MKDIR_P = mkdir -p

CC=gcc
CFLAGS=-g -Wall -Werror -pedantic -std=c11
VERBOSE = TRUE

# Scanner Tests
PRJ=scanner
SCANNEROBJ=$(PRJ)-test $(PRJ)-test2 $(PRJ)-test3

# Symtable
SYMTABLE=symtable

.PHONY: directories

all: directories $(SCANNEROBJ) symtable-test

directories: ${BUILDDIR}
${BUILDDIR}:
		${MKDIR_P} ${BUILDDIR}

$(PRJ)-test: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

$(PRJ)-test2: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test2.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

$(PRJ)-test3: $(LIBDIR)/*.h $(LIBDIR)/$(PRJ).h $(TESTSDIR)/$(PRJ)/*.h $(LIBDIR)/*.c  $(LIBDIR)/$(PRJ).c $(TESTSDIR)/$(PRJ)/*.c 
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@.out $(TESTSDIR)/$(PRJ)/$(PRJ)-test3.c $(LIBDIR)/$(PRJ).c $(LIBDIR)/str.c

$(SYMTABLE)-test:	$(TESTSDIR)/$(SYMTABLE)/*.c $(LIBDIR)/$(SYMTABLE).h $(LIBDIR)/$(SYMTABLE).c $(LIBDIR)/*.h $(LIBDIR)/*.c
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@ $(TESTSDIR)/$(SYMTABLE)/*.c $(LIBDIR)/$(SYMTABLE).h $(LIBDIR)/$(SYMTABLE).c

clean:
	rm -rf $(BUILDDIR)
#