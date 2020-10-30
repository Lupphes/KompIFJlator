
PRJ=scanner
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-Wall -std=c99 -lm

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c token.h str.c str.h keywords.h error.h
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

clean:
	rm -f *.o *.out $(PROGS)
#