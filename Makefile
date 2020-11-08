#Directory level
PROJDIR := $(realpath $(CURDIR))
LIBDIR := $(PROJDIR)/lib
BUILDDIR := $(PROJDIR)/build

MKDIR_P = mkdir -p

CC=gcc
CFLAGS=-g -Wall -Werror -pedantic -std=c11
VERBOSE = TRUE

#Scanner Tests
PRJ=scanner
SCANNEROBJ=$(PRJ)-test $(PRJ)-test2 $(PRJ)-test3

.PHONY: directories

all: directories $(SCANNEROBJ)

directories: ${BUILDDIR}
${BUILDDIR}:
		${MKDIR_P} ${BUILDDIR}

$(PRJ)-test: $(LIBDIR)/*.h $(LIBDIR)/scanner/scanner.h $(LIBDIR)/scanner/tests/*.h $(LIBDIR)/*.c  $(LIBDIR)/scanner/scanner.c $(LIBDIR)/scanner/tests/*.c 
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@.out $(LIBDIR)/scanner/tests/$(PRJ)-test.c $(LIBDIR)/scanner/$(PRJ).c $(LIBDIR)/str.c

$(PRJ)-test2: $(LIBDIR)/*.h $(LIBDIR)/scanner/scanner.h $(LIBDIR)/scanner/tests/*.h $(LIBDIR)/*.c  $(LIBDIR)/scanner/scanner.c $(LIBDIR)/scanner/tests/*.c 
	$(CC) $(CFLAGS) -o  $(BUILDDIR)/$@.out $(LIBDIR)/scanner/tests/$(PRJ)-test2.c $(LIBDIR)/scanner/$(PRJ).c $(LIBDIR)/str.c

$(PRJ)-test3: $(LIBDIR)/*.h $(LIBDIR)/scanner/scanner.h $(LIBDIR)/scanner/tests/*.h $(LIBDIR)/*.c  $(LIBDIR)/scanner/scanner.c $(LIBDIR)/scanner/tests/*.c 
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@.out $(LIBDIR)/scanner/tests/$(PRJ)-test3.c $(LIBDIR)/scanner/$(PRJ).c $(LIBDIR)/str.c

clean:
	rm -f *.o $(BUILDDIR) $(SCANNEROBJ)
#