CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm
FILES=btree.c ../btree.c ../test_util.c ../test.c ../character.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

valgrind: test
	valgrind --leak-check=full --track-origins=yes ./test

clean:
	rm -f test
