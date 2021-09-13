/*
 * Task2, task2.c
 * Purpose: create project on base OS Protothreads using functions PT_WAIT_UNTIL, PT_SCHEDULE.
 *
 * @author Oleksandr Ushkarenko
 * @version 1.0 13/09/2021
 */

#include "pt.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * This identifier is used to determine the number of elements in the numeric array.
 */
#define N 128

/*
 * This identifier is used to determine the maximum value of generated random number.
 */
#define MAX_NUMBER 1000

/*
 * These codes are used to describe current state of the program execution.
 */
#define ARRAY_EMPTY 0
#define ARRAY_FILLED 1
#define ARRAY_SORTED 2

/*
 * The instances of pt data type (protothread control structure) that are used
 * to manage protothreads in the program.
 */
static struct pt pt_thread_fill_array;
static struct pt pt_thread_bubble_sort;
static struct pt pt_thread_print_array;

/*
 * This variable is used in the program as the array's index.
 */
static int counter = 0;

/*
 * The array of integer numbers. In the program generated N random numbers 
 * in the range from 0 to MAX_NUMBER are put into this array.
 */
static int numbers[N];

/*
 * The variable is used to determine the current state of program execution.
 */
static int status = ARRAY_EMPTY;

/*
 * Declaration of the protothread function in which a random number is generated and put into array.
 * 
 * @param pt the pointer to the pt structure that describes a protothread
 */
static PT_THREAD(pt_thread_fill_array_func(struct pt *pt)) {
        PT_BEGIN(pt);
                PT_WAIT_UNTIL(pt, status == ARRAY_EMPTY);     
                srand(time(NULL));
                for(int i = 0; i < N; i++) {
                        numbers[i] = rand() % MAX_NUMBER + 1;
                }
                status = ARRAY_FILLED;
        PT_END(pt);
}

/*
 * Declaration of the protothread function in which the array is being sorted.
 * The bubble sorting algorithm (one of the most resource-consuming algorithms)
 * is used just for demonstration of task performing by protothread.
 * 
 * @param pt the pointer to the pt structure that describes a protothread
 */
static PT_THREAD(pt_thread_bubble_sort_func(struct pt *pt))
{
        PT_BEGIN(pt);
        PT_WAIT_UNTIL(pt, status == ARRAY_FILLED);     
        for(int i = 0; i < N; i++) {
                for(int j = 1; j < N; j++) {
                        if(numbers[j-1] > numbers[j]) {
                                int temp = numbers[j];
                                numbers[j] = numbers[j-1];
                                numbers[j-1] = temp;
                        }
                }
        }
        status = ARRAY_SORTED;
        PT_END(pt);
}

/*
 * Declaration of the protothread function that prints the elements of the array
 * in the console. Whel all elements of the array has been printed, the execution
 * of the thread terminates.
 * 
 * @param pt the pointer to the pt structure that describes a protothread
 */
static PT_THREAD(pt_thread_print_array_func(struct pt *pt))
{
        PT_BEGIN(pt);
        PT_WAIT_UNTIL(pt, status == ARRAY_SORTED);
        while(1) {
                printf("A[%d]=%d\n", counter, numbers[counter]);
                fflush(stdout);
                if(counter < (N - 1)) {
                        counter++;
                } else {
                           PT_EXIT(pt);
                }
        }
        PT_END(pt);
}

/*
 * The main function of the program.
 * Demonstration of using the protothreads (filling an array with random numbers,
 * sorting the array and printing array's elements in the console window).
 * 
 * @param argc the number of command line arguments
 * @param argv the array that contains passed command line arguments
 * @return 0 if successful
 */
int main(int argc, char *argv[]) {

        PT_INIT(&pt_thread_fill_array);
        PT_INIT(&pt_thread_bubble_sort);
        PT_INIT(&pt_thread_print_array);

        while(PT_SCHEDULE(pt_thread_print_array_func(&pt_thread_print_array)) != 0) {
                PT_SCHEDULE(pt_thread_fill_array_func(&pt_thread_fill_array));
                PT_SCHEDULE(pt_thread_bubble_sort_func(&pt_thread_bubble_sort));
        }
       
        return 0;
}
