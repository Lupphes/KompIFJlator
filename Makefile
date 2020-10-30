
PRJ=scanner
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS= -Wall -Werror -pedantic -std=c99

all: $(PROGS)

$(PRJ)-test: *.c *.h
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

clean:
	rm -f *.o *.out $(PROGS)
#