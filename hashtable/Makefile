CC=gcc
CFLAGS=-Wall -std=c11 -pedantic
FILES=hashtable.c test.c test_util.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

valgrind: test
	valgrind --leak-check=full --track-origins=yes ./test

clean:
	rm -f test
