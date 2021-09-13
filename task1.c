/*
 * Task1, task1.c
 * Purpose: create project using OS Protothreads to schedule two or more threads.
 *
 * @author Oleksandr Ushkarenko
 * @version 1.0 13/09/2021
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pt.h"

/*
 * This identifier is used to determine the maximum number of symbols that can be
 * stored in the array of char (i.e. the length of the string).
 */
#define MAX_STR_LEN 100

/*
 * The instances of pt data type (protothread control structure) that are used
 * to manage protothreads in the program.
 */
static struct pt pt_thread_1;
static struct pt pt_thread_2;

/*
 * The array to store the string enetered by a user in the console.
 */
static char text[MAX_STR_LEN];

/*
 * Declaration of the protothread function in which a string entered by a user is read from
 * the console and stored in the global array of char. If the user enters 'quit' the while loop
 * breaks and the execution of the thread stops. After each data reading from the console
 * the yielding from the current protothread occurs and other protothreads can continue their
 * execution.
 * 
 * @param pt the pointer to the pt structure that describes a protothread
 */
static int pt_thread_1_func(struct pt *pt)
{
        PT_BEGIN(pt);
        while(1){
                printf("Enter some text ('quit' for exit): ");
                fflush(stdout);
                scanf("%s", text);
                if((strcmp(text, "quit") == 0) || (strcmp(text, "QUIT") == 0)){
                        return 0;
                }
                PT_YIELD(pt);
        }
        PT_END(pt);
}

/*
 * Declaration of the protothread function in which a string that stored in the global array of char
 * is printed in the console window. Before printing, each symbol in the array is being converted 
 * to the uppercase. After the array of char has been printed, the yielding from the current protothread
 * occurs and other protothreads can continue their execution.
 * 
 * @param pt the pointer to the pt structure that describes a protothread
 */
static int pt_thread_2_func(struct pt *pt)
{
        PT_BEGIN(pt);
        while(1){
                printf("You entered (upper case): ");
                fflush(stdout);
                int i = 0;
                while(text[i]) {
                        putchar(toupper(text[i]));
                        i++;
                }
                putchar('\n');
                PT_YIELD(pt);
        }
        PT_END(pt);
}

/*
 * The main function of the program.
 * Demonstration of using the protothreads (reading text data from the console
 * and printing this text in uppercase).
 * 
 * @param argc the number of command line arguments
 * @param argv the array that contains passed command line arguments
 * @return 0 if successful
 */
int main(int argc, char *argv[]) {

        PT_INIT(&pt_thread_1);
        PT_INIT(&pt_thread_2);
       
        while(pt_thread_1_func(&pt_thread_1) && pt_thread_2_func(&pt_thread_2)) { }

        return 0;
}