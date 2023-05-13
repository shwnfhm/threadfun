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
#include <math.h>

//*********************************************************
// Typedefs and Structs
//*********************************************************

//*********************************************************
// Struct to hold data to be passed to a thread.
//*********************************************************
typedef struct {
    int *statistic;    //pointer to the statistic to be calculated in the thread (min/max/mode/median)
    float *fStatistic; //pointer to the statistic to be calculated in the thread (mean/median/std. dev.)
    int *modeCheck;    //flag to check if mode has been calculated
    int *nums;         //array of integers to process
} thdata;

//*********************************************************
// Function Prototypes
//*********************************************************
int compare(const void *a, const void *b);
void *minRunner(void *param);
void *maxRunner(void *param);
void *meanRunner(void *param);
void *medianRunner(void *param);
void *modeRunner(void *param);
void *stdDevRunner(void *param);

//*********************************************************
//
// Integer Comparison Function
//
// Compares two integers 'a' and 'b' and returns the 
// difference, used for quicksort.
//
// Return Value
// ------------
// int   difference between a and b
//
// Function Parameters
// -------------------
// a    const void *    first integer
// b    const void *    second integer
//
// Local Variables
// ---------------
// none
//
//*********************************************************
int compare(const void *a, const void *b){
    return (*(int *)a - *(int *)b);
}

//*********************************************************
//
// Min Thread Runner
//
// Thread function that finds and returns the minimum value 
// in a set of integers (via a pointer in the thdata struct).
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to the thdata struct, contains the array of integers, its length, and the address to store the minimum value
//
// Local Variables
// ---------------
// data    thdata*     pointer to the thdata struct, containing the array of integers, its length, and the address to store the minimum value
// num     int*        array of integers to process, minus the count
// min     int         holds the minimum value in array 'num'
//
//*********************************************************
void *minRunner(void *param){

    //cast the void pointer to a thdata pointer
    thdata *data = (thdata *)param;

    //seperate pointer that starts at the first integer in the array
    int *num = data->nums + 1;

    //set the minimum value to the first integer in the array
    int min = num[0];

    //iterate through the array and find the minimum value
    for (int i = 0; i < (data->nums)[0]; i++){
        if (num[i] < min){
            min = num[i];
        }
    }

    //set the minimum value and exit the thread
    *(data->statistic) = min;
    pthread_exit(0);
}

//*********************************************************
//
// Max Thread Runner
//
// Thread function that finds and returns the maximum value 
// of a set of integers (via a pointer in the thdata struct).
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to the thdata struct, contains the array of integers, its length, and the address to store the maximum value
//
// Local Variables
// ---------------
// data    thdata*     pointer to the thdata struct, containing the array of integers, its length, and the address to store the maximum value
// num     int*        array of integers to process, minus the count
// max     int         holds the maximum value in array 'num'
//
//*********************************************************
void *maxRunner(void *param){

    //cast the void pointer to a thdata pointer
    thdata *data = (thdata *)param;

    //seperate pointer that starts at the first integer in the array
    int *num = data->nums + 1;

    //set the maximum value to the first integer in the array
    int max = num[0];

    //iterate through the array and find the maximum value
    for (int i = 0; i < (data->nums)[0]; i++){
        if (num[i] > max){
            max = num[i];
        }
    }

    //set the maximum value and exit
    *(data->statistic) = max;
    pthread_exit(0);
}

//*********************************************************
//
// Mean Thread Runner
//
// Thread function that calculates and returns the mean of a 
// set of integers (via a pointer in the thdata struct).
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to the thdata struct, containing the array of integers, its length, and the address to store the mean
//
// Local Variables
// ---------------
// data    thdata*     pointer to the thdata struct, containing the array of integers, its length, and the address to store the mean
// num     int*        array of integers to process, minus the count
// sum     int         holds the sum of all the integers in array 'num'
//
//*********************************************************
void *meanRunner(void *param){

    //cast the void pointer to a thdata pointer
    thdata *data = (thdata *)param;

    //seperate pointer that starts at the first integer in the array
    int *num = data->nums + 1;

    //set the sum to 0
    int sum = 0;

    //iterate through the array and add each integer to the sum
    for (int i = 0; i < (data->nums)[0]; i++){
        sum += num[i];
    }

    //set the mean value (nums[0] stores the count of integers) and exit
    *(data->fStatistic) = (float)sum / (data->nums)[0];
    pthread_exit(0);
}

//*********************************************************
//
// Median Thread Runner
// 
// Thread function that sorts an array of integers and
// calculates and returns the median of the array (via a
// pointer in the thdata struct).
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to the thdata struct, containing the array of integers, its length, and the address to store the median
//
// Local Variables
// ---------------
// data    thdata*     pointer to the thdata struct, containing the array of integers, its length, and the address to store the median
// num     int*        array of integers to process, minus the count
// median  float       stores the median of integers in 'num'
//
//*********************************************************
void *medianRunner(void *param){

    //cast the void pointer to a thdata pointer
    thdata *data = (thdata *)param;

    //seperate pointer that starts at the first integer in the array
    int *num = data->nums + 1;
    float median;

    //sort the array of integers using quicksort and the compare function
    qsort(num, (data->nums)[0], sizeof(int), compare);

    //if the number of integers is even, find the average of the two middle integers
    if ((data->nums)[0] % 2 == 0){
        median = (float)(num[(data->nums)[0] / 2] + num[((data->nums)[0] / 2) - 1]) / 2;
        *(data->fStatistic) = median;
    }

    //if the number of integers is odd, find the middle integer
    else{
        median = (int)num[(data->nums)[0] / 2];
        *(data->statistic) = median;
    }

    //exit the thread
    pthread_exit(0);
}

//*********************************************************
//
// Mode Thread Runner
//
// Thread function that finds and returns the mode of a set
// of integers (via a pointer in the thdata struct).
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to the thdata struct, containing the array of integers, its length, and the address to store the mode
//
// Local Variables
// ---------------
// data     thdata*     pointer to the thdata struct, containing the array of integers, its length, and the address to store the mode
// num      int*       array of integers to process, minus the count
// maxCount int        holds the count of the mode of the integers in 'num'
// maxValue int        holds the mode of the integers in 'num', if found
// count    int        loop variable that stores count of current integer
//
//*********************************************************
void *modeRunner(void *param){

    //cast the void pointer to a thdata pointer
    thdata *data = (thdata *)param;

    //seperate pointer that starts at the first integer in the array
    int *num = data->nums + 1;

    //set the max count to 0
    int maxCount = 0, maxValue = 0;

    //iterate through the array and find the mode
    for (int i = 0; i < (data->nums)[0]; i++){
        int count = 0;

        for (int j = 0; j < (data->nums)[0]; j++){
            if (num[j] == num[i]){
                count++;
            }
        }
        if (count > maxCount){
            maxCount = count;
            maxValue = num[i];
        }
    }
    //if the max count is 1, there is no mode
    if (maxCount == 1){
        *(data->modeCheck) = -1;
    }

    //if the max count is greater than 1, there is a mode, so set the mode value
    else{
        *(data->statistic) = maxValue;
    }

    //exit the thread
    pthread_exit(0);
}

//*********************************************************
//
// Sample Standard Deviation Thread Runner
//
// Thread function that calculates and returns the sample 
// standard deviation of a set of integers (via a pointer
// in the thdata struct).
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to the thdata struct
//
// Local Variables
// ---------------
// data    thdata*     pointer to the thdata struct, containing the array of integers, its length, and the address to store the standard deviation
// num     int*        array of integers to process, minus the count
// sum     int         sum of the integers in array 'num'
// stddev  float       sample standard deviation of the integers in array 'num'
// mean    float       mean of integers in array 'num'
//
//*********************************************************
void *stdDevRunner(void *param){

    //cast the void pointer to an thdata pointer
    thdata *data = (thdata *)param;

    //seperate pointer that starts at the first integer in the array
    int *num = data->nums + 1;

    //set the sum and standard deviation to 0
    int sum = 0;
    float stddev = 0.0;

    //calculate the mean
    for (int i = 0; i < (data->nums)[0]; i++){
        sum += num[i];
    }
    float mean = (float)sum / (data->nums)[0];

    //calculate the sample standard deviation
    for (int i = 0; i < (data->nums)[0]; i++){
        stddev += pow(num[i] - mean, 2);
    }

    //return the sample standard deviation and exit the thread
    *(data->fStatistic) = sqrt(stddev / ((data->nums)[0] - 1));
    pthread_exit(0);
}

//*********************************************************
//
// Main Function
//
// Creates 6 threads to calculate the max, mean, median,
// mode, and sample standard deviation of a set of integers, 
// using a dedicated thread for each computation. Once each
// value is computed and returned via the thread data struct,
// the main function prints the results.
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
// threads   pthread_t[]  array of threads to execute, one for each statistic
// data      thdata[]     array of thdata structs, one for each thread, used to pass data and store return values for the threads
// min       int          minimum value of the integers
// max       int          maximum value of the integers
// median    int          median value of the integers (used if the number of integers is odd)
// mode      int          mode of the integers
// modeCheck int          used to check if there is a mode
// median1   float        median value of the integers (used if the number of integers is even)
// stddev    float        sample standard deviation of the integers
// mean      float        mean of the integers
// nums      int*         stores the number of integers to be processed followed by the integers themselves
// count     int          number of integers to be processed
//
//*********************************************************
int main(int argc, char *argv[]){

    //declare the threads and their data
    pthread_t threads[6];
    thdata data[6];

    //declare the variables to store the statistics
    //these will hold the return value for the threads
    //'count' stores the number of integers the user enters
    int min, max, median, mode, modeCheck, count = 0;
    float median1, stddev, mean;

    //check if the user entered any integers
    if (argc < 2){
        printf("Usage: pthreadstatcalc <1 or more integers>\n");
    }

    //otherwise...
    else{

        //check if the user entered any non-integer values
        for (int i = 1; i < argc; i++){
            if (!atoi(argv[i])){
                printf("Usage: pthreadstatcalc <1 or more integers>\n");
                exit(1);
            }
        }

        //allocate memory for the array of integers and their count
        int *nums = (int *)malloc(sizeof(int) * (argc));
        nums[0] = 0;

        //store the integers in the array
        for (int i = 1; i < (argc); i++){
            nums[i] = strtol(argv[i], NULL, 10);
            count++;
        }

        //store the count of integers in the first index of the array
        //I needed to do this because Linux was accessing garbage values
        //beyond the values the user entered.
        nums[0] = count;

        //set the thdata struct for each thread to have the array of integers
        for(int i = 0; i < 6; i++){
            data[i].nums = nums;
        }

        //give each thdata struct the address it needs to store statistics
        //this is where the return value(s) for each thread gets stored
        data[0].statistic = &min;
        data[1].statistic = &max;
        data[2].fStatistic = &mean;
        data[3].statistic = &median;
        data[3].fStatistic = &median1;
        data[4].statistic = &mode;
        data[4].modeCheck = &modeCheck;
        data[5].fStatistic = &stddev;

        //create a thread for each statistic to calculate
        //one by one, passing the array of integers, its length, and
        //the address to store the statistic to each thread
        pthread_create(&threads[0], NULL, minRunner, (void *)&data[0]);
        pthread_create(&threads[1], NULL, maxRunner, (void *)&data[1]);
        pthread_create(&threads[2], NULL, meanRunner, (void *)&data[2]);
        pthread_create(&threads[3], NULL, medianRunner, (void *)&data[3]);
        pthread_create(&threads[4], NULL, modeRunner, (void *)&data[4]);
        pthread_create(&threads[5], NULL, stdDevRunner, (void *)&data[5]);
        
        //wait for each thread to finish
        for(int i = 0; i < 6; i++){
            pthread_join(threads[i], NULL);
        }

        //print the statistics
        printf("MIN: %d\n", min);
        printf("MAX: %d\n", max);
        printf("MEAN: %.3f\n", mean);
        
        if(count % 2 == 0){
            printf("MEDIAN: %.3f\n", median1);
        }
        else{
            printf("MEDIAN: %d\n", median);
        }

        if(modeCheck != -1){
            printf("MODE: %d\n", mode);
        }
        else{
            printf("MODE: NO MODE\n");
        }

        printf("STD. DEV.: %.3f\n", stddev);

        //free the memory allocated for the array of integers
        free(nums);
        nums = NULL;
    }
    return 0;
}