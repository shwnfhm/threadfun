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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <ctype.h>

//*********************************************************
// Typedefs and Structs
//*********************************************************

//*********************************************************
// Struct to hold data to be passed to a thread.
//*********************************************************
typedef struct{
    int *index;     //index of current token being processed in 'tokens'
    char **tokens;  //array of tokens entered by the user
} ThreadData;

//*********************************************************
// Function Prototypes
//*********************************************************
void *upperRunner(void *param);
void *lowerRunner(void *param);
void *numberRunner(void *param);
void *otherRunner(void *param);

//*********************************************************
//
// "UPPER" Thread Runner
//  
// Thread function that prints all tokens that begin with
// an uppercase letter.
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to ThreadData struct, contains index of current token being processed and array of tokens
//
// Local Variables
// ---------------
// data    ThreadData*     pointer to ThreadData struct, contains index of current token being processed and array of tokens
//
//*********************************************************
void *upperRunner(void *param){

    //cast the pointer to a ThreadData pointer
    ThreadData *data = (ThreadData *)param;

    //while there are still tokens to process
    while ((data->tokens)[*(data->index)]){

        //if the first letter in token is an uppercase letter,
        //print the token and move the index to the next token
        if (data->tokens[*(data->index)][0] >= 'A' && data->tokens[*(data->index)][0] <= 'Z'){
            printf("UPPER: %s\n", data->tokens[*(data->index)]);
            *(data->index) += 1;
        }

        //otherwise, yield to other threads to process the token
        else{
            sched_yield();
        }
    }
    pthread_exit(0);
}

//*********************************************************
//
// "LOWER" Thread Runner
//
// Thread function that prints all tokens that begin with
// an lowercase letter.
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to ThreadData struct, contains index of current token being processed and array of tokens
//
// Local Variables
// ---------------
// data     ThreadData*     pointer to ThreadData struct, contains index of current token being processed and array of tokens
//
//*********************************************************
void *lowerRunner(void *param){

    //cast param to ThreadData struct
    ThreadData *data = (ThreadData *)param;

    //while there are still tokens to process
    while ((data->tokens)[*(data->index)]){

        //if the first letter in token is a lowercase letter, print the token and move the index to the next token
        if (data->tokens[*(data->index)][0] >= 'a' && data->tokens[*(data->index)][0] <= 'z'){
            printf("LOWER: %s\n", data->tokens[*(data->index)]);
            *(data->index) += 1;
        }

        //otherwise, yield to other threads to process the token
        else{
            sched_yield();
        }
    }
    pthread_exit(0);
}

//*********************************************************
//
// "NUMBER" Thread Runner
//
// Thread function that prints all tokens that are numbers.
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to ThreadData struct, contains index of current token being processed and array of tokens
//
// Local Variables
// ---------------
// data    ThreadData*     pointer to ThreadData struct, contains index of current token being processed and array of tokens
// endptr  char*           end pointer, used to verify if a token is a number
//
//*********************************************************
void *numberRunner(void *param){

    //cast param to ThreadData struct
    ThreadData *data = (ThreadData *)param;

    //while there are still tokens to process
    while ((data->tokens)[*(data->index)]){

        //check if token is a number based on the value of endptr -
        //this will point to the first character that is not a number in the token
        char *endptr = NULL;
        strtol(data->tokens[*(data->index)], &endptr, 10);

        //if token is a number, print the token and move the index to the next token
        if (!strcmp(endptr, "")){
            printf("NUMBER: %s\n", data->tokens[*(data->index)]);
            *(data->index) += 1;
        }

        //otherwise, yield to other threads to process the token
        else{
            sched_yield();
        }
    }
    pthread_exit(0);
}

//*********************************************************
//
// "OTHER" Thread Runner
//
// Thread function that prints all tokens that do not begin
// with a letter and are not numbers. 
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to ThreadData struct, contains index of current token being processed and array of tokens
//
// Local Variables
// ---------------
// data     ThreadData*     pointer to ThreadData struct, contains index of current token being processed and array of tokens
// endptr   char*           end pointer, used to verify if a token is not a number
//
//*********************************************************
void *otherRunner(void *param){

    //cast param to ThreadData struct
    ThreadData *data = (ThreadData *)param;

    //while there are still tokens to process
    while ((data->tokens)[*(data->index)]){

        //check if token is a number based on the value of endptr -
        //this will point to the first character that is not a number
        char *endptr = NULL;
        strtol(data->tokens[*(data->index)], &endptr, 10);

        //if token is not a number and does not begin with a letter, 
        //print the token and move the index to the next token
        if(strcmp(endptr, "") && !(data->tokens[*(data->index)][0] >= 'A' && data->tokens[*(data->index)][0] <= 'Z') && !(data->tokens[*(data->index)][0] >= 'a' && data->tokens[*(data->index)][0] <= 'z')){
            printf("OTHER: %s\n", data->tokens[*(data->index)]);
            *(data->index) += 1;
        }

        //otherwise, yield to other threads to process the token
        else{
            sched_yield();
        }
    }
    pthread_exit(0);
}

//*********************************************************
//
// Main Function
//
// Creates and joins 4 threads, each of which will parse 
// through the tokens entered by the user and print the 
// tokens that match the thread's category in the order the
// user entered the tokens (tokens are grouped in the following
// categories: UPPER, LOWER, NUMBER, or OTHER).
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
// threads  pthread_t[]    array of threads
// data     ThreadData[]   array of thread data
// tokens   char**         array of tokens entered by the user, to be passed to each thread
// index    int            current index in tokens, to be passed to each thread
//
//*********************************************************
int main(int argc, char *argv[]){

    //stores the threads
    pthread_t threads[4];

    //stores the data for each thread
    ThreadData data[4];

    //stores the tokens entered by the user.
    char **tokens = (char **)malloc(sizeof(char *) * (argc));

    //index of the current token being processed
    int index = 0;

    //check if the user supplied any tokens
    if (argc == 1){
        printf("Usage: tokenid <one or more tokens>\n");
        exit(1);
    }

    //copy the user entered tokens into 'tokens'
    for (int i = 1; i < argc; i++){
        tokens[i - 1] = (char *)malloc(sizeof(char) * (strlen(argv[i]) + 1));
        strcpy(tokens[i - 1], argv[i]);
    }

    //set each thread's data
    for (int i = 0; i < 4; i++){
        data[i].index = &index;
        data[i].tokens = tokens;
    }

    //create the threads for each category
    pthread_create(&threads[0], NULL, upperRunner, (void *)data);
    pthread_create(&threads[1], NULL, lowerRunner, (void *)data);
    pthread_create(&threads[2], NULL, numberRunner, (void *)data);
    pthread_create(&threads[3], NULL, otherRunner, (void *)data);

    //join the threads
    for (int i = 0; i < 4; i++){
        pthread_join(threads[i], NULL);
    }

    //free the memory allocated for the tokens
    for (int i = 0; i < argc - 1; i++){
        free(tokens[i]);
        tokens[i] = NULL;
    }
    free(tokens);
    tokens = NULL;

    return 0;
}