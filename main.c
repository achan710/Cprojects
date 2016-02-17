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
#include <stdlib.h> /*for atof() */
#include "assert.h"
#include <ctype.h>

// Preprocessor Constants
#define TRUE 1
#define FALSE 0

#define EX4_1 FALSE
#define EX4_3 TRUE


// Problem Constants
#if (EX4_3 == TRUE)
#define MAXOP 100 /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define MAXVAL 100  /* maximum depth of val stack */
#define BUFSIZE 100
#endif

// Function Declarations
int strindex(char s[], char t[]); /* Ex4.1 */
int mystrindex(char s[], char t[]);

void push (double f); /* Ex 4.3 */
double pop(void);
int getop(char []);
int getch(void);
void ungetch(int);
int polish_calc();

// Global Variables
#if (EX4_3 == TRUE)
int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */
char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */
#endif


int main(void)
{
#if (EX4_1 == TRUE)
    // code to test my strindex
    int result;
    result = mystrindex("abceeeeabc", "eab");
    printf("result = %d\n", result);
    assert ( 6 == result);

    // Ex. 4.1 start here
    assert( 2 == strindex("eex", "x"));     // 1 letter in s
    assert( 5 == strindex("eexeex", "x"));     // 2 letters in s
    assert( 4 == strindex("eexeex", "e"));     // last e
    assert( 3 == strindex("eexeex", "ee"));     // last group of e
    assert( 0 == strindex("aexeex", "a"));     // first letter in s
    assert( 1 == strindex("ebdxebex", "bd"));     // 2 letters in t
#endif

#if (EX4_3 == TRUE)
polish_calc();
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
 * EX4_1
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


/*
 * Exercise 4-3. Given the basic framework, it's straightforward 
 * to extend the calculator. Add the modulus (%) operator and 
 * provisions for negative numbers. c Polish calculator.
 */
int polish_calc()
{
    int type;
    double op2;
    char s[MAXOP];
    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("error: zero divisor\n");
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }
    return 0;
}

/* push: push f onto value stack */
void push (double f)
{
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

/* getop: get next character or numeric operand */
int getop(char s[])
{
    int i, c;
    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    if (!isdigit(c) && c != '.')
        return c; /* not a number */
    i = 0;
    if (isdigit(c)) /* collect integer part */
        while (isdigit(s[++i] = c = getch()))
            ;
    if (c == '.') /* collect fraction part */
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

int getch(void) /* get a (possibly pushed-back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
