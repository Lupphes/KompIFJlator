
PRJ=scanner
#
PROGS=$(PRJ)-test $(PRJ)-test3
CC=gcc
CFLAGS=-g -Wall -Werror -pedantic -std=c11

all: $(PROGS)

$(PRJ)-test: *.c *.h
	$(CC) $(CFLAGS) -o $@.out $(PRJ)-test.c $(PRJ).c str.c

$(PRJ)-test3: *.c *.h
	$(CC) $(CFLAGS) -o $@.out $(PRJ)-test3.c $(PRJ).c str.c

clean:
	rm -f *.o *.out $(PROGS)
#