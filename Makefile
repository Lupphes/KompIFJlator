
PRJ=scanner
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-g -Wall -Werror -pedantic -std=c11

all: $(PROGS)

$(PRJ)-test: *.c *.h
	$(CC) $(CFLAGS) -o $@ $(PRJ)-test.c $(PRJ).c str.c

clean:
	rm -f *.o *.out $(PROGS)
#