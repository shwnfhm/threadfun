//*********************************************************
//
// Shawn Fahimi
// Operating Systems
// Programming Project #3: threadfun2
// February 23, 2023
// Instructor: Dr. Michael Scherger
//
//*********************************************************

//*********************************************************
// Includes and Defines
//*********************************************************
#include <unistd.h>    /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <errno.h>     /* Errors */
#include <stdio.h>     /* Input/Output */
#include <stdlib.h>    /* General Utilities */
#include <pthread.h>   /* POSIX Threads */
#include <string.h>    /* String handling */

//*********************************************************
// Typedefs and Structs
//*********************************************************

//*********************************************************
// Struct to hold data to be passed to a thread.
// This shows how multiple data items can be passed 
// to a thread.
//*********************************************************
typedef struct str_thdata {
    int thread_no;      //thread number
    char message[100];  //message to be printed by the thread
} thdata;

//*********************************************************
// Function Prototypes
//*********************************************************
void print_message_function(void *ptr);

//*********************************************************
//
// Print Message Function
//
// Thread runner function that prints a message "Thread 'n'
// says 'message'", where 'n' is the thread number and
// 'message' is the message passed to the thread.
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// ptr    void *     reference      pointer to thdata struct with thread number and message
//
// Local Variables
// ---------------
// data   thdata*    pointer to thdata struct; contains thread number and message to be printed
//
//*********************************************************
void print_message_function(void *ptr){

    //cast the pointer to a thdata pointer
    thdata *data = (thdata *)ptr; 

    //print the message and exit
    printf("Thread %d says %s \n", data->thread_no, data->message);
    pthread_exit(0);
}

//*********************************************************
//
// Main Function
//
// Creates a number of threads specified by the user and 
// passes a message to each thread to print. The message
// is "hello" or "hi" if the thread id is even or odd,
// respectively.
//
// Return Value
// ------------
// int      0 if successfully executed, 1 if not
//
// Function Parameters
// -------------------
// argc     int     value      number of arguments
// argv     char*[] reference  arguments
//
// Local Variables
// ---------------
// numThreads   int         number of threads to create (user-specified)
// threads      pthread_t[] array of threads
// data         thdata[]    array of thdata structs to be passed to threads
//
//*********************************************************
int main(int argc, char *argv[]){

    //check for correct number of arguments
    if(argc != 2){
        printf("Usage: threadarray <non-negative number of threads>\n");
        exit(1);
    }

    //check for valid and non-negative number of threads
    else if(strtol(argv[1], NULL, 10) <= 0 && strcmp("0", argv[1])){
        printf("Usage: threadarray <non-negative number of threads>\n");
        exit(1);
    }

    //create a number of threads based on the user input
    int numThreads = atoi(argv[1]);
    pthread_t threads[numThreads]; 
    thdata data[numThreads];

    //create the threads, assign sequential ID's 
    //starting from 1, and pass them a message
    //to print out in print_message_function
    for (int i = 0; i < numThreads; i++){
        data[i].thread_no = i + 1;
        strcpy(data[i].message, (i % 2 == 0) ? "Hello!" : "Hi!");
        pthread_create(&threads[i], NULL, (void *)&print_message_function, (void *)&data[i]);
    }

    //wait for all threads to finish and exit
    for (int i = 0; i < numThreads; i++){
        pthread_join(threads[i], NULL);
    }
    exit(0);
}