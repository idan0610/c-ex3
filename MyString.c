/**
 * @file MyString.c
 * @author  Idan Refaeli <idan.refaeli@mail.huji.ac.il>
 * @version 1.0
 * @date 13 Aug 2015
 * 
 * @brief MyString library for representing strings
 *
 * @section LICENSE
 * This program is a free software
 *
 * @section DESCRIPTION
 * This library is used to represent strings and provide useful functions for them.
 * The MyString struct is composed of array of chars representing the string and int number
 * representing the length of the string.
 * Every function returning a pointer to MyString or changing the string uses a dynamic memory
 * allocation, to the MyString struct or to the string itself.
 * Every function check if its arguments are valid in order to prevent errors and be more efficient.
 * the length is also kept in order to save the need to calculate the length of the string whenever
 * needed.
 */

// ------------------------------ includes ------------------------------

#include "MyString.h"

// ------------------------------ consts --------------------------------
#define TO_INT_ASCII 48
#define ZERO_ASCII 48
#define NINE_ASCII 57
#define PLUS_ASCII 43
#define MINUS_ASCII 45

// ------------------------------ structs -------------------------------

/**
 * Represents a MyString
 */
struct _MyString
{
	char* _string; // The string
	size_t _length; // The length of the string
};

// ------------------------------ functions -----------------------------

/**
 * @brief Check if the string of given MyString is not NULL and if so, free it.
 * @param str
 */
static void freeString(MyString* str)
{
	if (str != NULL && str->_string != NULL)
	{
		free(str->_string);
	}
}

/**
 * @brief Find the length of c string.
 * 		  cString should be set and not NULL.
 * @param cString
 * @return the length
 */
static size_t getLength(const char* cString)
{
	size_t length = 0;

	while (cString[length] != '\0')
	{
		length++;
	}

	return length;
}

/**
 * @brief Find the number of digits of given number.
 * 		  num should be set and not null.
 * @param num
 * @return The number of digits
 */
static int getNumOfDigits(int num)
{
	int numOfDigits = 0;
	while (num != 0)
	{
		numOfDigits++;
		num /= 10;
	}

	return numOfDigits;
}

/**
 * @brief Compare between 2 chars by their ASCII value
 * @param ch1
 * @param ch2
 * @return 1 if ch1 is bigger then ch2, -1 if ch2 is bigger then ch1, 0 if they are equal.
 */
static int defaultComparator(const char ch1, const char ch2)
{
	if (ch1 > ch2)
	{
		return 1;
	}
	else if (ch1 < ch2)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief Used to cast 2 void* pointers into 2 MyString** pointers and  call to myStringCompare
 * 		  with the new pointers.
 * 		  str1 and str2 should be allocated.
 * @param str1
 * @param str2
 * @return result of myStringCompare
 */
static int myStringComparatorCasting(const void* str1, const void* str2)
{
	MyString* myStr1 = *(MyString**)str1;
	MyString* myStr2 = *(MyString**)str2;
	return myStringCompare(myStr1, myStr2);
}

/**
 * @brief Allocates a new MyString and sets its value to "" (the empty string).
 * 			It is the caller's responsibility to free the returned MyString.
 *
 * 	COMPLEXITY: O(1) because there is a constant number of commands: creating pointer to MyString,
 * 				allocating it using malloc (assume malloc is O(1)), comparison, setting
 * 				values to the MyString allocated and return.
 *
 * RETURN VALUE:
 * @return a pointer to the new string, or NULL if the allocation failed.
 */
MyString * myStringAlloc()
{
	MyString* myString = (MyString*)malloc(sizeof(MyString));
	if (myString == NULL)
	{
		return NULL;
	}

	myString->_string = NULL;
	myString->_length = 0;

	return myString;
}

/**
 * @brief Frees the memory and resources allocated to str.
 * COMPLEXITY: O(1) because there is a constant number of commands: comparrison and freeing memory
 * 			   using free (assume free is O(1))
 * @param str the MyString to free.
 * If str is NULL, no operation is performed.
 */
void myStringFree(MyString *str)
{
	if(str != NULL)
	{
		free(str->_string);
		free(str);
	}
}

/**
 * @brief Allocates a new MyString with the same value as str. It is the caller's
 * 			responsibility to free the returned MyString.
 * @param str the MyString to clone.
 * COMPLEXITY: O(N) because myStringSetFromMyString is O(N).
 * RETURN VALUE:
 *   @return a pointer to the new string, or NULL if the allocation failed.
 */
MyString * myStringClone(const MyString *str)
{
	if (str == NULL)
	{
		return NULL;
	}

	MyString* clone = myStringAlloc();

	if (clone == NULL)
	{
		return NULL;
	}

	if (myStringSetFromMyString(clone, str) == MYSTRING_ERROR)
	{
		return NULL;
	}

	return clone;
}

/**
 * @brief Sets the value of str to the value of other.
 * COMPLEXITY: O(N) where N is the length of str, or the number of bytes of str->string takes,
 * 			   because of memcpy complexity.
 * @param str the MyString to set
 * @param other the MyString to set from
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
*/
MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
	if (str == NULL || other == NULL)
	{
		return MYSTRING_ERROR;
	}

	freeString(str);

	str->_string = (char*)malloc(other->_length*sizeof(char));

	if (str->_string == NULL)
	{
		return MYSTRING_ERROR;
	}

	str->_length = other->_length;
	memcpy(str->_string, other->_string, str->_length);

	return MYSTRING_SUCCESS;
}

/**
 * @brief filter the value of str acording to a filter.
 * 	remove from str all the occurrence of chars that are filtered by filt
 *	(i.e. filr(char)==true)
 *	COMPLEXITY: O(N) where N is the length of str, because the loop runs on every char in str.
 *				If the given filt is bigger then O(N), then the complexity is that of filt.
 * @param str the MyString to filter
 * @param filt the filter
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure. */
MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
	if (str == NULL || str->_string == NULL || filt == NULL)
	{
		return MYSTRING_ERROR;
	}

	char* filteredString = (char*)malloc(str->_length * sizeof(char));

	unsigned int i, j = 0;

	for (i = 0; i < str->_length; i++)
	{
		if (filt(&str->_string[i]) == false)
		{
			filteredString[j] = str->_string[i];
			j++;
		}
	}

	freeString(str);
	str->_length = j;
	str->_string = (char*)malloc(str->_length * sizeof(char));

	if (str->_string == NULL)
	{
		return MYSTRING_ERROR;
	}

	memcpy(str->_string, filteredString, str->_length);

	free(filteredString);

	return MYSTRING_SUCCESS;
}

/**
 * @brief Sets the value of str to the value of the given C string.
 * 			The given C string must be terminated by the null character.
 * 			Checking will not use a string without the null character.
 * 	COMPLEXICTY: O(N) where N is the length of cString because getLength() is O(N) and also
 * 				 memcpy() is O(N).
 * @param str the MyString to set.
 * @param cString the C string to set from.
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringSetFromCString(MyString *str, const char * cString)
{
	if (str == NULL || cString == NULL)
	{
		return MYSTRING_ERROR;
	}

	freeString(str);

	str->_length = getLength(cString);
	str->_string = (char*)malloc(str->_length * sizeof(char));

	if (str->_string == NULL)
	{
		return MYSTRING_ERROR;
	}

	memcpy(str->_string, cString, str->_length);

	return MYSTRING_SUCCESS;
}

/**
 * @brief Sets the value of str to the value of the integer n.
 *	(i.e. if n=7 than str should contain ‘7’)
 * 	You are not allowed to use itoa (or a similar functions) here but must code your own conversion
 * 	function.
 * COMPLEXICTY: O(N) where N is the number of digits on the given int n, because getNumberOfDigits
 * runs in O(N) and also the loop in this function runs for N times.
 * @param str the MyString to set.
 * @param n the int to set from.
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
	if (str == NULL)
	{
		return MYSTRING_ERROR;
	}

	freeString(str);
	int num = getNumOfDigits(n);
	str->_length = num;

	if (n < 0)
	{
		str->_length++;
		str->_string = (char*)malloc(str->_length * sizeof(char));
		str->_string[0] = '-';
		n *= -1;
	}
	else
	{
		str->_string = (char*)malloc(str->_length * sizeof(char));
	}


	if (str->_string == NULL)
	{
		return MYSTRING_ERROR;
	}

	int i;

	for (i = 0; i < num; i++)
	{
		str->_string[str->_length - i - 1] = (char)(n % 10) + TO_INT_ASCII;
		n /= 10;
	}

	return MYSTRING_SUCCESS;
}

/**
 * @brief Returns the value of str as an integer.
 * 	If str cannot be parsed as an integer,
 * 	the return value should be MYSTR_ERROR_CODE
 * 	NOTE: positive and negative integers should be supported.
 * 	COMPLEXITY: O(N) where N is the length of str, because the loop runs for N times.
 * @param str the MyString
 * @return an integer
 */
int myStringToInt(const MyString *str)
{
	if (str == NULL)
	{
		return MYSTR_ERROR_CODE;
	}

	int power = 1, num = 0, i;

	for (i = str->_length - 1; i >= 0; i--)
	{
		if (str->_string[i] >= ZERO_ASCII && str->_string[i] <= NINE_ASCII)
		{
			num += ((int)str->_string[i] - TO_INT_ASCII)*power;
			power *= 10;
		}
		else if (i == 0 && ( str->_string[i] == PLUS_ASCII || str->_string[i] == MINUS_ASCII ))
		{
			if (str->_string[i] == MINUS_ASCII)
			{
				num *= -1;
			}
		}
		else
		{
			return MYSTR_ERROR_CODE;
		}
	}

	return num;
}

/**
 * @brief Returns the value of str as a C string, terminated with the
 * 	null character. It is the caller's responsibility to free the returned
 * 	string by calling free().
 * 	COMPLEXCITY: O(N) where N is the length of str because of memcpy.
 * @param str the MyString
 * RETURN VALUE:
 *  @return the new string, or NULL if the allocation failed.
 */
char * myStringToCString(const MyString *str)
{
	if (str == NULL)
	{
		return NULL;
	}

	char* cString = (char*)malloc((str->_length + 1) * sizeof(char));
	memcpy(cString, str->_string, str->_length);
	cString[str->_length] = '\0';
	return cString;
}

/**
 * @brief Appends a copy of the source MyString src to the destination MyString dst.
 * COMPLEXCITY: O(N) where N is the length of src, because of memcpy.
 * @param dest the destination
 * @param src the MyString to append
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringCat(MyString * dest, const MyString * src)
{
	if (dest == NULL || src == NULL)
	{
		return MYSTRING_ERROR;
	}

	char* temp = myStringToCString(dest);

	freeString(dest);
	dest->_string = (char*)malloc((dest->_length + src->_length) * sizeof(char));

	if (dest->_string == NULL)
	{
		return MYSTRING_ERROR;
	}

	memcpy(dest->_string, temp, dest->_length);
	memcpy(dest->_string + dest->_length, src->_string, src->_length);

	dest->_length += src->_length;

	free(temp);

	return MYSTRING_SUCCESS;
}

/**
 * @brief Sets result to be the concatenation of str1 and str2.
 * 	result should be initially allocated by the caller.
 * 	result shouldn't be the same struct as str1 or str2.
 * 	COMPLEXITY: O(N + M) where N is the length of str1 and M is the length of str2 because of
 * 				the 2 memcpy calls.
 * @param str1
 * @param str2
 * @param result
 * RETURN VALUE:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2, MyString *result)
{
	if (str1 == NULL || str2 == NULL || result == NULL)
	{
		return MYSTRING_ERROR;
	}

	freeString(result);

	result->_string = (char*)malloc((str1->_length + str2->_length) * sizeof(char));
	if (result->_string == NULL)
	{
		return MYSTRING_ERROR;
	}

	memcpy(result->_string, str1->_string, str1->_length);
	memcpy(result->_string + str1->_length, str2->_string, str2->_length);
	result->_length = str1->_length + str2->_length;

	return MYSTRING_SUCCESS;
}

/**
 * @brief Compare str1 and str2.
 * COMPLEXITY: O(N) where N is the length of the shorter of str1 and str2, because that the
 * complexity of myStringCustomCompare()
 * @param str1
 * @param str2
 *
 * RETURN VALUE:
 * @return an integral value indicating the relationship between the strings:
 * 	A zero value indicates that the strings are equal.
 * 	A value greater than zero indicates that the first character that does not match has a greater
 * 	ASCII value in str1 than in str2;
 * 	And a value less than zero indicates the opposite.
 * 	If strings cannot be compared, the return value should be MYSTR_ERROR_CODE
 */
int myStringCompare(const MyString *str1, const MyString *str2)
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	return myStringCustomCompare(str1, str2, defaultComparator);
}

/**
 * @brief Compares str1 and str2.
 * COMPLEXITY: O(N) where N is the length of the shorter one of str1 and str2, because the loop
 * runs N times
 * @param str1
 * @param str2
 * @param comparator
 * RETURN VALUE:
 * @return an integral value indicating the relationship between the strings:
 * 	A zero value indicates that the strings are equal according to the custom comparator
 * 	(3rd parameter).
 * 	A value greater than zero indicates that the first MyString is bigger according to the
 * 	comparator.
 * 	And a value less than zero indicates the opposite.
 * 	If strings cannot be compared, the return value should be MYSTR_ERROR_CODE
 */
int myStringCustomCompare(const MyString* str1, const MyString* str2,
						  int (*comparator)(const char, const char))
{
	if (str1->_string == NULL || str2->_string == NULL || str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}

	unsigned int i = 0;

	while (i < str1->_length && i < str2->_length)
	{
		if (comparator(str1->_string[i], str2->_string[i]) > 0)
		{
			return 1;
		}
		else if (comparator(str1->_string[i], str2->_string[i]) < 0)
		{
			return -1;
		}
		i++;
	}

	if (str1->_length == str2->_length)
	{
		return 0;
	}
	else if (str1->_length == i)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

/**
 * @brief Check if str1 is equal to str2.
 * COMPLEXITY: O(N) where N is the length of the shorter of str1 and str2 because of the complexity
 * of myStringCustomEqual()
 * @param str1
 * @param str2
 *
 * RETURN VALUE:
 * @return an integral value indicating the equality of the strings (logical equality -
 * that they are composed of the very same characters):
 * 	A zero value indicates that the strings are not equal.
 * 	A greater than zero value indicates that the strings are equal.
 * 	If strings cannot be compared, the return value should be MYSTR_ERROR_CODE
  */
int myStringEqual(const MyString *str1, const MyString *str2)
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	return myStringCustomEqual(str1, str2, defaultComparator);
}

/**
 * @brief Check if str1 is equal to str2.
 * COMPLEXITY: O(N) where N is the length of the shorter of str1 and str2, because of the
 * complexity of myStringCustomCompare().
 * @param str1
 * @param str2
 * @param comparator
 *
 * RETURN VALUE:
 * @return an integral value indicating the equality of the strings using a custom comparator
 * (3rd parameter):
 * 	A zero value indicates that the strings are not equal.
 * 	A greater than zero value indicates that the strings are equal.
 * 	If string cannot be compared, the return value should be MYSTR_ERROR_CODE
 */
int myStringCustomEqual(const MyString* str1, const MyString* str2,
						int (*comparator)(const char, const char))
{
	if (str1 == NULL || str2 == NULL || comparator == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	int compare = myStringCustomCompare(str1, str2, comparator);

	if (compare == MYSTR_ERROR_CODE)
	{
		return MYSTR_ERROR_CODE;
	}
	else if (compare != 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * COMPLEXITY: O(1) because there is a constant number of operations in O(1)
 * @return the amount of memory (all the memory that used by the MyString object itself and
 * its allocations), in bytes, allocated to str1.
 */
unsigned long myStringMemUsage(const MyString *str1)
{
	if (str1 == NULL)
	{
		return 0;
	}
	unsigned long memory = sizeof(str1);
	memory += str1->_length * sizeof(char);

	return memory;
}

/**
 * COMPLEXITY: O(1) because there is a constant number of operations in O(1)
 * @return the length of the string in str1.
 */
unsigned long myStringLen(const MyString *str1)
{
	if (str1 == NULL)
	{
		return 0;
	}
	return str1->_length;
}

/**
 * Writes the content of str to stream. (like fputs())
 * COMPLEXITY: O(1) because there is a constant number of operations in O(1)
 *
 * RETURNS:
 *  @return MYSTRING_SUCCESS on success, MYSTRING_ERROR on failure.
 */
MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{
	if (str == NULL || stream == NULL)
	{
		return MYSTRING_ERROR;
	}

	char* output = myStringToCString(str);
	int writeCheck = fputs(output, stream);
	free(output);

	if (writeCheck >= 0)
	{
		return MYSTRING_SUCCESS;
	}

	return MYSTRING_ERROR;
}

/**
 * @brief sort an array of MyString pointers
 * COMPLEXITY: O(N^2) because of the complexity of qsort on worst case.
 * @param arr
 * @param len
 * @param comparator custom comparator
 *
 * RETURN VALUE:none
  */
void myStringCustomSort(MyString** arr, size_t len, int (*comparator)(const void*, const void*))
{
	if (arr == NULL || !(len > 0) || comparator == NULL)
	{
		return ;
	}
	qsort(arr, len, sizeof(MyString*), comparator);
}

/**
 * @brief sorts an array of MyString pointers according to the default comparison (like in myStringCompare)
 * COMPLEXITY: O(N^2) because of the complexity of qsort on worst case inside myStringCustomSort.
 * @param arr
 * @param len
 *
 * RETURN VALUE: none
  */
void myStringSort(MyString** arr, size_t len)
{
	if (arr == NULL || !(len > 0))
	{
		return ;
	}

	myStringCustomSort(arr, len, myStringComparatorCasting);
}

// ------------------------------ test ---------------------------------

#ifndef NDEBUG

/**
 * @brief Unit-testing to myStringAlloc()
 */
void testMyStringAlloc()
{
	printf("Testing myStringAlloc()...\n");
	printf("Allocating a new empty MyString to myString\n");
	MyString* myString = myStringAlloc();

	if(myString == NULL)
	{
		perror("Error in memory allocation.\n");
	}
	else
	{
		printf("Empty string successfully allocated\n");
	}

	free(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringFree()
 */
void testMyStringFree()
{
	printf("Testing myStringFree()...\n");
	printf("Allocating a new empty MyString to myString\n");
	MyString* myString = myStringAlloc();
	printf("Trying to free to memory allocated to myString\n");
	myStringFree(myString);
	printf("Memory successfully freed. Setting myString to NULL\n");
	myString = NULL;
	printf("Trying to free memory allocated to NULL\n");
	myStringFree(myString);
	printf("Nothing happened as needed\n");

	printf("\n");
}

/**
 * @brief Unit-testing to myStringClone()
 */
void testMyStringClone()
{
	printf("Testing myStringClone()...\n");
	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString to be \"abc\"\n");
	myStringSetFromCString(myString1, "abc");
	printf("Cloning myString1 into myString2\n");
	MyString* myString2 = myStringClone(myString1);
	if (myString2 == NULL)
	{
		perror("Error in memory allocation.\n");
	}
	else
	{
		char* res = myStringToCString(myString2);
		printf("Cloning success. myString2 is \"%s\"\n", res);
		free(res);
	}

	myStringFree(myString1);
	myStringFree(myString2);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringSetFromMyString()
 */
void testMyStringSetFromMyString()
{
	printf("Testing myStringSetFromMyString()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString1 to be \"abc\"\n");
	myStringSetFromCString(myString1, "abc");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString2 to be \"def\"\n");
	myStringSetFromCString(myString2, "def");

	if (myStringSetFromMyString(myString2, myString1) == MYSTRING_ERROR)
	{
		perror("Error in allocating memory.\n");
	}
	else
	{
		char* res = myStringToCString(myString2);
		printf("Success. myString2 = %s\n", res);
		free(res);
	}

	myStringFree(myString1);
	myStringFree(myString2);

	printf("\n");
}

/**
 * @brief Used as an argument to myStringFilter() in testMyStringFilter()
 * @parm pointer to char for filtering
 * @return true if the given char is filtered, false otherwise.
 */
static bool filter(const char* letter)
{
	if (*letter == 'b' || *letter == 'g')
	{
		return true;
	}

	return false;
}

/**
 * @brief Unit-testing to myStringFilter()
 */
void testMyStringFilter()
{
	printf("Testing myStringFilter()...\n");
	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString = myStringAlloc();
	printf("Setting myString to be \"abcbfdgbdfsgjh\"\n");
	myStringSetFromCString(myString, "abcbfdgbdfsgjh");

	bool (*filt)(const char *);
	filt = filter;

	int x = myStringFilter(myString, filt);

	if (x == MYSTRING_ERROR)
	{
		perror("Error in allocating memory\n");
	}
	else
	{
		char* str = myStringToCString(myString);
		printf("Success. myString = %s\n", str);
		free(str);
	}

	myStringFree(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringSetFromCString()
 */
void testMyStringSetFromCString()
{
	printf("Testing myStringSetFromCString()...\n");
	printf("Allocating empty MyString to myString\n");
	MyString* myString = myStringAlloc();
	printf("Setting myString1 to \"abc\"\n");
	char* res = NULL;
	if (myStringSetFromCString(myString, "abc") == MYSTRING_SUCCESS)
	{
		res = myStringToCString(myString);
		printf("SUCCESS. myString = %s\n", res);
		free(res);
	}
	else
	{
		perror("Error allocating memory.");
	}

	printf("Setting myString to \"def\"\n");
	if (myStringSetFromCString(myString, "def") == MYSTRING_SUCCESS)
	{
		res = myStringToCString(myString);
		printf("SUCCESS. myString1 = %s\n", res);
		free(res);
	}
	else
	{
		perror("Error allocating memory.");
	}

	myStringFree(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringSetFromInt()
 */
void testMyStringSetFromInt()
{
	printf("Testing myStringSetFromInt()...\n");
	printf("Allocating a new empty MyString to myString\n");
	MyString* myString = myStringAlloc();
	char* res = NULL;

	printf("Setting myString to 765\n");
	if (myStringSetFromInt(myString, 765) == MYSTRING_ERROR)
	{
		perror("Error in memory allocation.\n");
	}
	else
	{
		res = myStringToCString(myString);
		printf("Success. myString = %s\n", res);
		free(res);
	}

	printf("Setting myString to -1385\n");
	if (myStringSetFromInt(myString, -1385) == MYSTRING_ERROR)
	{
		perror("Error in memory allocation.\n");
	}
	else
	{
		res = myStringToCString(myString);
		printf("Success. myString = %s\n", res);
		free(res);
	}

	myStringFree(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringToInt()
 */
void testMyStringToInt()
{
	printf("Testing myStringSetFromInt()...\n");
	printf("Allocating a new empty MyString to myString\n");
	MyString* myString = myStringAlloc();
	printf("Setting myString to 765\n");
	myStringSetFromInt(myString, 765);
	int num = myStringToInt(myString);
	if (num == MYSTR_ERROR_CODE)
	{
		perror("Error in function myStringToInt.\n");
	}
	else
	{
		printf("Success. num = %d\n", num);
	}

	printf("Setting myString to \"abc\"\n");
	myStringSetFromCString(myString, "abc");
	num = myStringToInt(myString);
	if (num == MYSTR_ERROR_CODE)
	{
		printf("Returns error as expected, because of bad argument.\n");
	}
	else
	{
		perror("ERROR");
	}

	myStringFree(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringToCString()
 */
void testMyStringToCString()
{
	printf("Testing myStringToCString()...\n");

	printf("Allocating a new empty MyString to myString\n");
	MyString* myString = myStringAlloc();
	printf("Setting myString to \"abc\"\n");
	myStringSetFromCString(myString, "abc");
	char* cString = myStringToCString(myString);
	if (cString == NULL)
	{
		perror("Error in memory allocation.\n");
	}
	else
	{
		printf("Success. cString = %s\n", cString);
	}

	free(cString);
	myStringFree(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringCat()
 */
void testMyStringCat()
{
	printf("Testing myStringToCString()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString1 to \"abc\"\n");
	myStringSetFromCString(myString1, "abc");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString2 to \"def\"\n");
	myStringSetFromCString(myString2, "def");

	printf("Appending myString2 to myString1:\n");
	if(myStringCat(myString1, myString2) == MYSTRING_ERROR)
	{
		perror("Error in memory allocation.\n");
	}
	else
	{
		char* res = myStringToCString(myString1);
		printf("Success. myString1 = %s\n", res);
		free(res);
	}

	myStringFree(myString1);
	myStringFree(myString2);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringCatTo()
 */
void testMyStringCatTo()
{
	printf("Testing myStringCatTo()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString1 to \"abc\"\n");
	myStringSetFromCString(myString1, "abc");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString2 to \"def\"\n");
	myStringSetFromCString(myString2, "def");

	printf("Allocating a new empty MyString to result\n");
	MyString* result = myStringAlloc();

	printf("Concatenate myString1 and myString2 into result\n");
	if(myStringCatTo(myString1, myString2, result) == MYSTRING_ERROR)
	{
		perror("Error in memory Allocation.\n");
	}
	else
	{
		char* res = myStringToCString(result);
		printf("Success. result = %s\n", res);
		free(res);
	}

	myStringFree(myString1);
	myStringFree(myString2);
	myStringFree(result);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringCompare()
 */
void testMyStringCompare()
{
	printf("Testing myStringCompare()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString1 to \"abc\"\n");
	myStringSetFromCString(myString1, "abc");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString2 to \"def\"\n");
	myStringSetFromCString(myString2, "def");

	int compare = myStringCompare(myString1, myString2);

	if (compare == MYSTR_ERROR_CODE)
	{
		perror("Error in myStringCompare()\n");
	}
	else if (compare > 0)
	{
		printf("myString1 is bigger then myString2\n");
	}
	else if (compare < 0)
	{
		printf("myString2 is bigger then myString1\n");
	}
	else
	{
		printf("myString1 and myString2 are equal\n");
	}

	printf("Setting myString2 to \"abc\"\n");
	myStringSetFromCString(myString2, "abc");

	compare = myStringCompare(myString1, myString2);

	if (compare == MYSTR_ERROR_CODE)
	{
		perror("Error in myStringCompare()\n");
	}
	else if (compare > 0)
	{
		printf("myString1 is bigger then myString2\n");
	}
	else if (compare < 0)
	{
		printf("myString2 is bigger then myString1\n");
	}
	else
	{
		printf("myString1 and myString2 are equal\n");
	}

	printf("Setting myString2 to NULL\n");
	freeString(myString2);
	myString2->_string = NULL;

	compare = myStringCompare(myString1, myString2);

	if (compare == MYSTR_ERROR_CODE)
	{
		perror("ERROR as expected, because and compare\n");
	}
	else if (compare > 0)
	{
		printf("myString1 is bigger then myString2\n");
	}
	else if (compare < 0)
	{
		printf("myString2 is bigger then myString1\n");
	}
	else
	{
		printf("myString1 and myString2 are equal\n");
	}

	myStringFree(myString1);
	myStringFree(myString2);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringCustomCompare()
 */
void testMyStringCustomCompare()
{
	printf("Testing myStringCustomCompare()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString1 to \"abc\"\n");
	myStringSetFromCString(myString1, "abc");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString2 to \"def\"\n");
	myStringSetFromCString(myString2, "def");

	int (*comparator)(const char, const char);
	comparator = defaultComparator;

	int compare = myStringCustomCompare(myString1, myString2, comparator);

	if (compare == MYSTR_ERROR_CODE)
	{
		perror("ERROR in myStringCustomCompare()\n");
	}
	else if (compare > 0)
	{
		printf("myString1 is bigger then myString2\n");
	}
	else if (compare < 0)
	{
		printf("myString2 is bigger then myString1\n");
	}
	else
	{
		printf("myString1 and myString2 are equal\n");
	}

	printf("Setting myString2 to \"abc\"\n");
	myStringSetFromCString(myString2, "abc");

	compare = myStringCustomCompare(myString1, myString2, comparator);

	if (compare == MYSTR_ERROR_CODE)
	{
		perror("ERROR in myStringCustomCompare()\n");
	}
	else if (compare > 0)
	{
		printf("myString1 is bigger then myString2\n");
	}
	else if (compare < 0)
	{
		printf("myString2 is bigger then myString1\n");
	}
	else
	{
		printf("myString1 and myString2 are equal\n");
	}

	printf("Setting myString2 to NULL\n");
	freeString(myString2);
	myString2->_string = NULL;

	compare = myStringCustomCompare(myString1, myString2, comparator);

	if (compare == MYSTR_ERROR_CODE)
	{
		perror("ERROR as expected, because and compare\n");
	}
	else if (compare > 0)
	{
		printf("myString1 is bigger then myString2\n");
	}
	else if (compare < 0)
	{
		printf("myString2 is bigger then myString1\n");
	}
	else
	{
		printf("myString1 and myString2 are equal\n");
	}

	myStringFree(myString1);
	myStringFree(myString2);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringEqual()
 */
void testMyStringEqual()
{
	printf("Testing myStringEqual()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString1 to \"abc\"\n");
	myStringSetFromCString(myString1, "abc");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString2 to \"def\"\n");
	myStringSetFromCString(myString2, "def");

	int equal = myStringEqual(myString1, myString2);
	if (equal == MYSTR_ERROR_CODE)
	{
		perror ("ERROR in myStringEqual()\n");
	}
	else if (equal > 0)
	{
		printf("myString1 and myString2 are equal\n");
	}
	else
	{
		printf("myString1 and myString2 are not equal\n");
	}

	printf("Setting myString2 to \"abc\"\n");
	myStringSetFromCString(myString2, "abc");

	equal = myStringEqual(myString1, myString2);
	if (equal == MYSTR_ERROR_CODE)
	{
		perror ("ERROR in myStringEqual()\n");
	}
	else if (equal > 0)
	{
		printf("myString1 and myString2 are equal\n");
	}
	else
	{
		printf("myString1 and myString2 are not equal\n");
	}

	printf("Setting myString2 to NULL\n");
	freeString(myString2);
	myString2->_string = NULL;

	equal = myStringEqual(myString1, myString2);
	if (equal == MYSTR_ERROR_CODE)
	{
		perror ("ERROR as expected because can't compare\n");
	}
	else if (equal > 0)
	{
		printf("myString1 and myString2 are equal\n");
	}
	else
	{
		printf("myString1 and myString2 are not equal\n");
	}

	myStringFree(myString1);
	myStringFree(myString2);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringCustomEqual()
 */
void testMyStringCustomEqual()
{
	printf("Testing myStringCustomEqual()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString1 to \"abc\"\n");
	myStringSetFromCString(myString1, "abc");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString2 to \"def\"\n");
	myStringSetFromCString(myString2, "def");

	int (*comparator)(const char, const char);
	comparator = defaultComparator;

	int equal = myStringCustomEqual(myString1, myString2, comparator);
	if (equal == MYSTR_ERROR_CODE)
	{
		perror ("ERROR in myStringCustomEqual\n");
	}
	else if (equal > 0)
	{
		printf("myString1 and myString2 are equal\n");
	}
	else
	{
		printf("myString1 and myString2 are not equal\n");
	}

	printf("Setting myString2 to \"abc\"\n");
	myStringSetFromCString(myString2, "abc");

	equal = myStringCustomEqual(myString1, myString2, comparator);
	if (equal == MYSTR_ERROR_CODE)
	{
		perror ("ERROR in myStringCustomEqual\n");
	}
	else if (equal > 0)
	{
		printf("myString1 and myString2 are equal\n");
	}
	else
	{
		printf("myString1 and myString2 are not equal\n");
	}

	printf("Setting myString2 to NULL\n");
	freeString(myString2);
	myString2->_string = NULL;

	equal = myStringCustomEqual(myString1, myString2, comparator);
	if (equal == MYSTR_ERROR_CODE)
	{
		perror("ERROR as expected\n");
	}
	else if (equal > 0)
	{
		printf("myString1 and myString2 are equal\n");
	}
	else
	{
		printf("myString1 and myString2 are not equal\n");
	}

	myStringFree(myString1);
	myStringFree(myString2);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringMemUsage()
 */
void testMyStringMemUsage()
{
	printf("Testing myStringMemUsage()...\n");

	printf("Allocating a new empty MyString to myString\n");
	MyString* myString = myStringAlloc();
	printf("Setting myString to \"abc\"\n");
	myStringSetFromCString(myString, "abc");

	unsigned long memory = myStringMemUsage(myString);

	printf("Memory usage of myString: %lu", memory);

	myStringFree(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringLen()
 */
void testMyStringLen()
{
	printf("Testing myStringLen()...\n");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString = myStringAlloc();
	printf("Setting myString to \"abc\"\n");
	myStringSetFromCString(myString, "abc");

	unsigned long len = myStringLen(myString);

	printf("Length of myString: %lu", len);

	myStringFree(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringWrite()
 */
void testMyStringWrite()
{
	printf("Testing myStringWrite()...\n");

	printf("Allocating a new empty MyString to myString\n");
	MyString* myString = myStringAlloc();
	printf("Setting myString to \"abc\"\n");
	myStringSetFromCString(myString, "abc");

	FILE* stream = fopen("testWrite.txt", "w");

	if (myStringWrite(myString, stream) == MYSTRING_SUCCESS)
	{
		printf("Wrote myString to  test.txt successfully\n");
	}
	else
	{
		printf("ERROR\n");
	}

	fclose(stream);
	myStringFree(myString);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringCustomSort()
 */
void testMyStringCustomSort()
{
	printf("Testing myStringCustomSort()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString to \"abb\"\n");
	myStringSetFromCString(myString1, "abb");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString to \"aab\"\n");
	myStringSetFromCString(myString2, "aab");

	printf("Allocating a new empty MyString to myString3\n");
	MyString* myString3 = myStringAlloc();
	printf("Setting myString to \"bbb\"\n");
	myStringSetFromCString(myString3, "bbb");

	printf("Allocating a new empty MyString to myString4\n");
	MyString* myString4 = myStringAlloc();
	printf("Setting myString to \"aaa\"\n");
	myStringSetFromCString(myString4, "aaa");

	MyString* arr[] = {myString1, myString2, myString3, myString4};

	myStringCustomSort(arr, 4, myStringComparatorCasting);

	int i;
	FILE* stream = fopen("testSort.txt", "w");

	for (i = 0; i < 4; i++)
	{
		myStringWrite(arr[i], stream);
	}

	printf("Sorting success. Results in testSort.txt.\n");

	myStringFree(myString1);
	myStringFree(myString2);
	myStringFree(myString3);
	myStringFree(myString4);
	fclose(stream);
	printf("\n");
}

/**
 * @brief Unit-testing to myStringSort()
 */
void testMyStringSort()
{
	printf("Testing myStringSort()...\n");

	printf("Allocating a new empty MyString to myString1\n");
	MyString* myString1 = myStringAlloc();
	printf("Setting myString to \"abb\"\n");
	myStringSetFromCString(myString1, "abb");

	printf("Allocating a new empty MyString to myString2\n");
	MyString* myString2 = myStringAlloc();
	printf("Setting myString to \"aab\"\n");
	myStringSetFromCString(myString2, "aab");

	printf("Allocating a new empty MyString to myString3\n");
	MyString* myString3 = myStringAlloc();
	printf("Setting myString to \"bbb\"\n");
	myStringSetFromCString(myString3, "bbb");

	printf("Allocating a new empty MyString to myString4\n");
	MyString* myString4 = myStringAlloc();
	printf("Setting myString to \"aaa\"\n");
	myStringSetFromCString(myString4, "aaa");

	MyString* arr[] = {myString1, myString2, myString3, myString4};

	myStringSort(arr, 4);

	int i;
	FILE* stream = fopen("test2Sort.txt", "w");

	for (i = 0; i < 4; i++)
	{
		myStringWrite(arr[i], stream);
	}

	printf("Sorting success. Results in testSort.txt.\n");

	myStringFree(myString1);
	myStringFree(myString2);
	myStringFree(myString3);
	myStringFree(myString4);
	fclose(stream);
	printf("\n");
}

/**
 * @brief: Runs all the Unit-testing for the functions.
 */
int main()
{
	testMyStringAlloc();
	testMyStringFree();
	testMyStringClone();
	testMyStringSetFromMyString();
	testMyStringFilter();
	testMyStringSetFromCString();
	testMyStringSetFromInt();
	testMyStringToInt();
	testMyStringToCString();
	testMyStringCat();
	testMyStringCatTo();
	testMyStringCompare();
	testMyStringCustomCompare();
	testMyStringEqual();
	testMyStringCustomEqual();
	testMyStringMemUsage();
	testMyStringLen();
	testMyStringWrite();
	testMyStringCustomSort();
	testMyStringSort();
	return 0;
}

#endif
