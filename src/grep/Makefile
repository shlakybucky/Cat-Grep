СС = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
TARGET = s21_grep

$(TARGET) : s21_grep.o
	$(CC) $(CFLAGS) s21_grep.o -o $(TARGET)

style :
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c
	clang-format -i *.h

test.o : test.c
	$(CC) $(CFLAGS) -c s21_grep.c -o s21_grep.o

clean : 
	rm $(TARGET) *.o