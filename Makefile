CFLAGS = -g -Wextra -Wall -Wvla -c
CC = c99 

tests: MyString.c MyString.h
	$(CC) $(CFLAGS) MyString.c -o MyString.o
	$(CC) MyString.o -o tests
	tests
	
main: MyString.c MyStringMain.c MyString.h
	$(CC) $(CFLAGS) -DNDEBUG MyStringMain.c -o MyStringMain.o
	$(CC) $(CFLAGS) -DNDEBUG MyString.c -o MyString.o
	$(CC) MyStringMain.o MyString.o -o main
	main
	
myString: MyString.c MyString.h
	$(CC) $(CFLAGS) -DNDEBUG MyString.c -o MyString.o
	ar rcs libmyString.a MyString.o
	
clean:
	rm -f tests main myString