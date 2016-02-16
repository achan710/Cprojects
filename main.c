/*
 ============================================================================
 Name        : main.c
 Author      : Allan Chan
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"

#define TRUE 1
#define FALSE 0

#define EX4_1 TRUE
#define EX4_3 FALSE

int strindex(char s[], char t[]);


int main(void)
{

#if (EX4_1 == TRUE)
    assert( 2 == strindex("abcabc", "ca"));     // "ca" located at index 2
#endif

#if (EX4_3 == TRUE)
	printf("!!!Hello World!!!"); /* prints !!!Hello World!!! */
#endif

	// No errors encountered
	printf("Test Complete. Great work!");
	return EXIT_SUCCESS;
}


/*
 * Exercise 4-1. Write the function strindex(s,t) which returns the position of the rightmost
 * occurrence of t in s, or -1 if there is none.
 */
int strindex(char s[], char t[])
{
    int i, j, k;
    for (i = 0; s[i] != '\0'; i++) {
        for (j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == '\0')
            return i;
    }
    return -1;
}







