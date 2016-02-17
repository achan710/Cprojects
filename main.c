/*
 ============================================================================
 Name        : main.c
 Author      : Allan Chan
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
note: to set tabs in vim 
// set shiftwidth=4
// set tabstop=4
// set expandtab

// to show line numbers
// set number
*/

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"

#define TRUE 1
#define FALSE 0

#define EX4_1 TRUE
#define EX4_3 FALSE

int strindex(char s[], char t[]);
int mystrindex(char s[], char t[]);

int main(void)
{
#if (FALSE)
    // code to test my strindex
    int result;
    result = mystrindex("abceeeeabc", "eab");
    printf("result = %d\n", result);
    assert ( 6 == result);
#endif

#if (EX4_1 == TRUE)
    assert( 2 == strindex("eex", "x"));     // 1 letter in s
    assert( 5 == strindex("eexeex", "x"));     // 2 letters in s
    assert( 4 == strindex("eexeex", "e"));     // last e
    assert( 3 == strindex("eexeex", "ee"));     // last group of e
    assert( 0 == strindex("aexeex", "a"));     // first letter in s
    assert( 1 == strindex("ebdxebex", "bd"));     // 2 letters in t
#endif

#if (EX4_3 == TRUE)
	printf("!!!Hello World!!!"); /* prints !!!Hello World!!! */
#endif

	// No errors encountered
	printf("Test Complete. Great work!\n");
	return EXIT_SUCCESS;
}


/*
 * Exercise 4-1. Write the function strindex(s,t) which returns the position of the rightmost
 * occurrence of t in s, or -1 if there is none.
 *
 * Algoritm:
 * 1. instead of returning the index right away, save it to variable p
 * 2. Continue walking along s and look for matches. Update p as necessary
 * 3. return the last known p
 */
int strindex(char s[], char t[])
{
    int i, j, k;
    int p = -1;  // position

    for (i = 0; s[i] != '\0'; i++) {
        for (j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == '\0')
        {
            p = i;
        }
    }
    
    return p;
}

/*
 * Implemented my own strindex function.
 *
 * Algorithm:
 * 1. Loop all characters in s.
 * 2. Loop all characters in t against substring of s.
 * 3. once end of t is reached, indicates all characters match so return the index of the substring starting point
 */
int mystrindex(char s[], char t[])
{
    int sidx, tidx, subidx;
    sidx = 0;
    subidx = 0;
    tidx = 0;

    // loop s string
    while (s[sidx] != '\0')
    {
        //printf("%d %c\n", sidx, s[sidx]);

        // loop substring and compare against t
        while (s[subidx] == t[tidx] && t[tidx] != '\0')
        {
            //found a match
            //printf("here %d %d\n", subidx, tidx);
            tidx++;
            subidx++;
        }

        // check substring result
        if (t[tidx] == '\0' && tidx > 0)
        {
            // substring is a match and reached the last char in t. (better implementation would be to check that tidx == number of chars in t)
            //printf("got here %d %d\n", sidx, tidx);
            return sidx;
        }
        else
        {
            // did not match all chars in t. reset index
            tidx = 0;
        }

        // no match - go to next char in s
        sidx++;
        subidx = sidx;
    }

    // fail - reached end of s w/o a match
    if (s[sidx] == '\0')
    {
        return -1;
    }

    return 0;

}







