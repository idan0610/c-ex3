/**
 * @file MyStringMain.c
 * @author  Idan Refaeli <idan.refaeli@mail.huji.ac.il>
 * @version 1.0
 * @date 13 Aug 2015
 *
 * @brief Comparing 2 strings given as input
 *
 * @section LICENSE
 * This program is a free software
 *
 * @section DESCRIPTION
 * Asks the user for 2 strings, convert them to MyString and compare between them. Writes the result in file.
 *
 * Input  : 2 Strings to compare
 * Process: Converting the strings to MyString and comparing the strings.
 * Output : File with the results of comparing.
 */

// ------------------------------ includes ------------------------------
#include "MyString.h"

// ------------------------------ consts -=------------------------------
#define MAX_LENGTH 501

// ------------------------------ functions -----------------------------
/**
 * @brief Write to file "test.out" the statement "smallStr is smaller then bigStr" where smallStr
 *  and bigStr are given MyString pointers.
 * @param smallStr MyString pointer
 * @param bigStr MyString pointer
 */
void writeResult(MyString* smallStr, MyString* bigStr)
{
	FILE* stream = fopen("test.out", "w");

	myStringWrite(smallStr, stream);
	fputs(" is smaller then ", stream);
	myStringWrite(bigStr, stream);

	fclose(stream);

	char (* ( *f())[])();
}

/**
 * @brief Asks for 2 strings, compare between them and write to file the results.
 */
int main()
{
	char str1[MAX_LENGTH] = {0}, str2[MAX_LENGTH] = {0};
	printf("Enter the first string:\n");
	fgets(str1, MAX_LENGTH, stdin);
	printf("Enter the second string:\n");
	fgets(str2, MAX_LENGTH, stdin);

	// Replace the last char '\n' that fgets automatic puts in the end of the strings with '\0'
	int lenStr1 = strlen(str1);
	int lenStr2 = strlen(str2);

	str1[lenStr1 - 1] = '\0';
	str2[lenStr2 - 1] = '\0';

	MyString* myStr1 = myStringAlloc();
	MyString* myStr2 = myStringAlloc();

	myStringSetFromCString(myStr1, str1);
	myStringSetFromCString(myStr2, str2);

	printf("Compare between the 2 strings and writing the result to test.out...\n");

	int compare = myStringCompare(myStr1, myStr2);

	if (compare <= 0)
	{
		writeResult(myStr1, myStr2);
	}
	else
	{
		writeResult(myStr2, myStr1);
	}

	printf("Done.\n");

	myStringFree(myStr1);
	myStringFree(myStr2);
	return 0;
}
