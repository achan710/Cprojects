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
#include <math.h>   /* for fmod(x, y) */

// Preprocessor Constants
#define REV 1 //code revision indicator

#define TRUE 1
#define FALSE 0

#define EX4_1 FALSE
#define EX4_3 FALSE
#define EX4_4 TRUE

// Problem Constants

#define MAXOP 100 /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define MAXVAL 100  /* maximum depth of val stack */
#define BUFSIZE 100

// Function Declarations

void push (double f); /* Ex 4.3 */
double pop(void);
int getop(char []);
int getch(void);
void ungetch(int);

/* Ex 4-4 */
void clear(void);
void swap(void);
void print(void);
void duplicate(void);

#if (EX4_3)
int polish_calc();
#endif

// Global Variables

int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */
char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */


int main(void)
{
    printf("Test %d starting...\n", REV);

    // test print
    // test print - prints 50 and 100
    push(50.0);
    push(100);
    print();


    // test swap with exactly 2 items - prints 100 50
    printf("test swap 1\n");
    swap();
    print();


    // test swap with more than 2 items - only top 2 items should swap (eg 50, 25)
    printf("test swap 2\n");
    push(25);
    swap();
    print();


    // test duplicate - should repeat the values in stack (e.g. 100 25 50 100 25 50)
    printf("dup\n");
    duplicate();
    print();

    // test clear - stack should be empty
    printf("clear\n");
    clear();
    pop();




#if (EX4_3 == TRUE)

    /*
     * Test cases
     * 6 5 %  --> 1
     * 51 50 % --> 1
     */
    polish_calc();
#endif

	// No errors encountered
	printf("Test Complete. Great work!\n");
	return EXIT_SUCCESS;
}

#if (EX4_3 == TRUE)
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
            case '%':
                op2 = pop();    /* added mod functionality */
                push(fmod(pop(), op2));
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
#endif

/*
 * Exercise 4-4. Add the commands to print the top elements of the stack without popping, to
duplicate it, and to swap the top two elements. Add a command to clear the stack.
 */

/* clear : clear values in stack */
void clear(void)
{
    /* set stack index to 0*/
    sp = 0;
}

/* print: prints the stack*/
void print(void)
{
    int i;

    /* print all values up to sp */
    for (i = 0; i < sp; i++)
    {
        printf("%.8g\n", val[i]);
    }
}

/* duplicate: duplicate the stack
 *
 * Algorithm:
 * 1. check that there is enough room in stack (at least double the current size)
 * 2. save off the last index offset
 * 3. loop up to the last index and copy the 1st item location of sp
 */
void duplicate(void)
{
    int i;
    int last;

    if ((sp * 2) > MAXVAL)
    {
        // duplicating exceed stack size
        printf("error: stack full, can't duplicate\n");
    }
    else
    {
        // OK to duplicate
        last = sp;
        /* loop to existing last index, add values offset by 1 */
        for (i = 0; i < last; i++)
        {
            val[sp++] = val[i];
        }
    }
}

/*
 * swap: swap the top 2 elements of stack
 *
 * Algorithm
 * 1. ensure that there are at least 2 items, otherwise error
 * 2. pop 2 items and save to temp variable
 * 3. push items back in but in reverse order
 */
void swap(void)
{
    double t1;
    double t2;
    //requires 2 elements in stack
    if (sp > 1)
    {
        //swap
        t1 = pop();
        t2 = pop();
        push(t1);
        push(t2);
    }
    else
    {
        printf("error: stack contains only 1 element\n");
    }
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
