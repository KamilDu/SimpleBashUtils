CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: s21_grep

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c -o s21_grep.o

clean:
	rm -rf *.o s21_grep 

rebuild: clean all 
