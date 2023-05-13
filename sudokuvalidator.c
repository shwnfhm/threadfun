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

//*********************************************************
// Typedefs and Structs
//*********************************************************

//*********************************************************
// structure for passing data to threads
//*********************************************************
typedef struct {
    int row;    //row value for thread to check
    int col;    //column value for thread to check
} Parameters;


//*********************************************************
// Global Variables
//*********************************************************
int valid[27] = {0};    //array to hold whether each row, column, and subgrid is valid (0 = invalid, 1 = valid)
int puzzle[9][9];    //2D array to hold the sudoku puzzle

//*********************************************************
// Function Prototypes
//*********************************************************
void *rowRunner(void *param);
void *columnRunner(void *param);
void *subgridRunner(void *param);
void loadPuzzle(char *filename, int puzzle[9][9]);

//*********************************************************
//
// Row Thread Runner
// 
// Thread runner that checks whether a given row is valid.
// That is, whether a row contains all numbers 1-9 exactly
// once.
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to Parameters struct, contains row and column values of row
//
// Local Variables
// ---------------
// params     Parameters*  parameters for thread
// row        int          row value of row
// col        int          column value of row
// validRow   int[9]       array to keep track of duplicates in row
// val        int          current value in row
//
//*********************************************************
void *rowRunner(void *param){

    //cast param to a Parameters pointer
    Parameters *params = (Parameters*) param;

    //get the row and col values from the Parameters struct
	int row = params->row;
    int col = params->col;
    int val;    //used to store current value in row

    //used to check if row has each number 1-9 exactly once
    int validRow[9] = {0};

    //check if row and column value are valid for this thread
    if(row > 8 || col != 0){
		pthread_exit(0);
	}

    //check if row has each number 1-9 exactly once
    for(int i = 0; i < 9; i++){

        //current value in row
        val = puzzle[row][i];

        //if the number has already been seen or is not between 1 and 9, the row is invalid
        //so exit without setting the valid array value to 1
        if(validRow[val - 1] == 1 || val < 1 || val > 9){
            pthread_exit(0);
        }
        //otherwise, indicate that the number is present in the row
        else{
            validRow[val - 1] = 1;
        }
    }

    //if we made it here, that means the row is valid, so set the 
    //corresponding valid array value to 1 since the row is valid
    valid[row + 18] = 1;

    //free the memory allocated for the Parameters struct and exit
    free(params);
    params = NULL;
    pthread_exit(0);
}

//*********************************************************
//
// Column Thread Runner
//
// Thread runner that checks whether a given column is 
// valid. That is, whether a column contains all numbers 1-9 
// exactly once.
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param    void *     reference      pointer to Parameters struct, contains row and column values of column
//
// Local Variables
// ---------------
// params     Parameters*  pointer to Parameters struct, contains row and column of subgrid
// row        int          row value of column
// col        int          column value of column
// validCol   int[9]       array to keep track of duplicates in column
// val        int          current value in column
//
//*********************************************************
void *columnRunner(void *param){

    //cast param to a Parameters pointer
    Parameters *params = (Parameters*) param;

    //get the row and col values from the Parameters struct
    int row = params->row;
	int col = params->col;
    int val;    //used to store current value in column

    //used to check if column has each number 1-9 exactly once
    int validCol[9] = {0};

    //check if row and column value are valid for this function
    if (col > 8 || row != 0){
		pthread_exit(0);
	}

    //check if column has each number 1-9 exactly once
    for(int i = 0; i < 9; i++){

        //current value in column
        val = puzzle[i][col];

        //if the number has already been seen or is not between 1 and 9, the column is invalid
        //so exit without setting the valid array value to 1
        if(validCol[val - 1] == 1 || val < 1 || val > 9){
            pthread_exit(0);
        }
        //otherwise, indicate that the number is present in the column
        else{
            validCol[val - 1] = 1;
        }
    }

    //if we made it here, that means the column is valid, so set the 
    //corresponding valid array value to 1 since the column is valid
    valid[col + 9] = 1;

    //free the memory allocated for the Parameters struct and exit
    free(params);
    params = NULL;
    pthread_exit(0);
}

//*********************************************************
//
// 3x3 Thread Runner
//
// Thread runner that checks whether a given 3x3 subgrid is 
// valid. That is, whether a subgrid contains all numbers 
// 1-9 exactly once.
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// param      void *       reference        pointer to Parameters struct, contains row and column values of subgrid
//
// Local Variables
// ---------------
// params     Parameters*  pointer to Parameters struct
// row        int          row value of subgrid
// col        int          column value of subgrid
// valid3x3   int[9]       array to check for duplicates in subgrid
// val        int          current number in subgrid
//
//*********************************************************
void *subgridRunner(void *param){

    //cast param to a Parameters pointer
    Parameters *params = (Parameters*) param;

    //get the row and col values from the Parameters struct
	int row = params->row;
	int col = params->col;
    int val;    //used to store current value in 3x3 grid

    //used to check if subgrid has each number 1-9 exactly once
    int valid3x3[9] = {0};

    //check if row and col are valid subgrid values
    if (row % 3 != 0 || row > 6 || col % 3 != 0 || col > 6) {
		pthread_exit(0);
	}

    //check if subgrid has each number 1-9 exactly once
    for(int i = row; i < row + 3; i++){
        for(int j = col; j < col + 3; j++){

            //current value in subgrid
            val = puzzle[i][j];

            //if the number has already been seen or is not between 1 and 9, the subgrid is invalid
            //so exit without setting the valid array value to 1
            if(valid3x3[val - 1] == 1 || val < 1 || val > 9){
                pthread_exit(0);
            }
            //otherwise, indicate that the number is present in the subgrid
            else{
                valid3x3[val - 1] = 1;
            }
        }
    }

    //if we made it here, that means the subgrid is valid, so set the 
    //corresponding valid array value to 1 since the subgrid is valid
    valid[(row/3) + col] = 1;

    //free the memory allocated for the Parameters struct and exit
    free(params);
    params = NULL;
    pthread_exit(0);
}

//*********************************************************
//
// Load Puzzle Function
//
// Loads the sudoku puzzle from the file named 'filename'
// into the global 2D array 'puzzle' for threads to check 
// the validity of.
//
// Return Value
// ------------
// none
//
// Function Parameters
// -------------------
// filename char*      reference   name of the puzzle file to load
// puzzle   int[9][9]  reference   array to store puzzle
//
// Local Variables
// ---------------
// fp           FILE*      pointer to puzzle file
// overCheck    int        used to check if the puzzle is larger than a 9x9 grid
//
//*********************************************************
void loadPuzzle(char *filename, int puzzle[9][9]){

    int overCheck = 0; //used to check if the puzzle file has excessive values
    //try to open the puzzle file
    FILE *fp;
    fp = fopen(filename, "r");

    //if the file cannot be opened, print an error message and exit
    if (fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    //read the puzzle file into the puzzle array, one number at a time
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){

            //confirm that a number was read
            if(fscanf(fp, "%d", &puzzle[i][j]) > 0){
                continue;
            }
            
            //if the number cannot be read, print an error message and exit
            else{
                printf("Error reading puzzle (too few values or non-numeric entry)\n");
                fclose(fp);
                exit(1);
            }
        }
    }

    //check if more than a 9x9 grid's worth of values are in the file
    if(fscanf(fp, "%d", &overCheck) > 0){

        //if so, exit
        printf("Error reading puzzle (too many values)\n");
        fclose(fp);
        exit(1);
    }


    fclose(fp);
}

//*********************************************************
//
// Main Function
//
// Creates 27 threads that check the validity of the sudoku
// puzzle solution in the file specified by the user. The 
// threads check the rows, columns, and 3x3 subgrids of the 
// puzzle. The main function then waits for all threads to 
// finish before printing the result (TRUE if the puzzle 
// solution is valid, FALSE if not).
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
// threads      pthread_t[27]   array of threads
// threadsIndex int             index in 'threads', used to store threads in 'threads' array
// data         Parameters*     reference to parameters
//
//*********************************************************
int main(int argc, char *argv[]){

    //create an array of 27 threads
    pthread_t threads[27];
    int threadsIndex = 0;

    //check that the user has specified a puzzle file
    if(argc != 2){
        printf("Usage: sudokuvalidator <filename>\n");
        exit(1);
    }

    //load the puzzle from the file specified by the user
    loadPuzzle(argv[1], puzzle);
    
    //create 27 threads to check the validity of the puzzle
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){

            //if we hit a column index, create a thread to check the validity of the column
            if(i == 0){

                //create a Parameters struct with the indices and pass it to the column check thread
                Parameters *data = (Parameters *)malloc(sizeof(Parameters));
                data->row = i;
                data->col = j;
                pthread_create(&threads[threadsIndex++], NULL, columnRunner, (void *)data);
            }

            //if we hit a row index, create a thread to check the validity of the row
            if(j == 0){

                //create a Parameters struct with the indices and pass it to the row check thread
                Parameters *data = (Parameters *)malloc(sizeof(Parameters));
                data->row = i;
                data->col = j;
                pthread_create(&threads[threadsIndex++], NULL, rowRunner, (void *)data);
            }

            //if we hit 3x3 indices, create a thread to check the validity of the subgrid
            if(i % 3 == 0 && j % 3 == 0){

                //create a Parameters struct with the indices and pass it to the 3x3 check thread
                Parameters *data = (Parameters *)malloc(sizeof(Parameters));
                data->row = i;
                data->col = j;
                pthread_create(&threads[threadsIndex++], NULL, subgridRunner, (void *)data);
            }
        }
    }

    //wait for all threads to complete
    for(int i = 0; i < 27; i++){
        pthread_join(threads[i], NULL);
    }

    //if any of the threads found an invalid row, column, or subgrid, print FALSE and exit
    for(int i = 0; i < 27; i++){
        if(valid[i] == 0){
            printf("FALSE\n");
            exit(0);
        }
    }

    //otherwise, print TRUE and exit
    printf("TRUE\n");
    return 0;
}