СС = gcc
CFLAGS = -Wall -Werror -Wextra
TARGET = s21_cat

$(TARGET) : s21_cat.o
	$(CC) $(CFLAGS) s21_cat.o -o $(TARGET)

style :
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c
	clang-format -i *.h

test.o : test.c
	$(CC) $(CFLAGS) -c s21_cat.c -o s21_cat.o

clean : 
	rm $(TARGET) *.o