/* Thi Tran
 * RedID: 825223653
 * CS 480 - Operating Systems
 * Professor: Ben Shen
 * Assignment 2 - POSIX Threads
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include "dicttree.h"



#define NUMOFFILES 2
#define DICTSRCFILEINDEX 0
#define TESTFILEINDEX 1

/*default number of progress marks for representing 100% progress */
#define DEFAULT_NUMOF_MARKS 50

/* minimum number of progress marks for representing 100% progress */
#define MIN_NUMOF_MARKS 10

/* place hash marks in the progress bar every N marks */
#define DEFAULT_HASHMARKINTERVAL 10

/* default minimum number of dictionary words starting from a prefix for
printing or writing to the output */
#define DEFAULT_MINIMUM_OFWORDS_WITHAPREFIX 1

typedef struct {

  /**
   * root node of the dictionary tree
   */
  dictNode *dictRootNode = createANewNode();   // Call createANewNode

  /**
   * parameters for printing progress bar
   */
  int numOfProgressMarks;
  int hashmarkInterval;

  /**
   * print a word and its count to the output file only if the
   * number of dictionary words starting from the word is equal to or
   * greater than this number
   */
  int minNumOfWordsWithAPrefixForPrinting;

  /**
   * filePath[0] - file path for the dictionary vocabulary file
   *               providing words to populate the dictionary tree
   * filePath[1] - file path for the test source file
   *               providing words to be used for testing
   */
  const char  *filePath[NUMOFFILES];

  /*
   * totalNumOfCharsInFile[DICTSRCFILEINDEX];
   * totalNumOfCharsInFile[TESTFILEINDEX];
   */
  long  totalNumOfCharsInFile[NUMOFFILES];


   /* numOfCharsProcessedFromFile[DICTSRCFILEINDEX]
   *   - number of chars read in and processed from
   *     the dictionary vocabulary file
   * numOfCharsProcessedFromFile[TESTFILEINDEX]
   *   - number of chars read in and processed from
   *     the test file
   */
  long  *numOfCharsProcessedFromFile[NUMOFFILES];

  /*
   * wordCountInFile[DICTSRCFILEINDEX]
   *   - number of total words in the dictionary vocabulary file.
   * wordCountInFile[TESTFILEINDEX]
   *   - number of total words in the test file
   */
  long  wordCountInFile[NUMOFFILES];

  /*
   * taskCompleted[DICTSRCFILEINDEX]
   * taskCompleted[TESTFILEINDEX]
   */
  bool  taskCompleted[NUMOFFILES];

} EXEC_STATUS;


using namespace std;

const char *delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";   // Declare delimiters


/**
 * During the program, a pointer called 'status' is used to pass the common shared data to thread functions and main function
 * Use insert, search, and count functions which are similar to Assignment 1
 * In addition, there are some changes of these above functions in order to satisfy the requirements of Assignment 2
 **/


/* Function to get size of text file
 * Reference: https://techoverflow.net/2013/08/21/how-to-get-filesize-using-stat-in-cc/
 */
long findSize(const char *filename)
{
    struct stat statbuf;

    if(stat(filename, &statbuf) != 0)   // Get size of filename using stat
    {
        return statbuf.st_size;
    }
}


/*
 * Function to print the progress bar during process of reading text files
 * 'status': shared date pointer
 * index: index of text files
 * Print the bar as smooth as desired using cout.flush()
 */

void progress_bar(EXEC_STATUS *status, int index)
{
long n1, n2;   // Dereference the object pointer
status->numOfCharsProcessedFromFile[index] = &n1;   // # of chars processed from file
status->totalNumOfCharsInFile[index] = n2;   //   total # of chars in file

double percent = 0;   // keep track of current state


// Checking current percent until reaching the # of progress marks
while(percent < status->numOfProgressMarks)
{   // Update current state while reading characters
    percent = (double)(*status->numOfCharsProcessedFromFile[index])/ (double)(status->totalNumOfCharsInFile[index]);

    for(int i = 1; i <= status->numOfProgressMarks; i++)
    {   // Print progress bar along with updating the current state
        if(i % status->hashmarkInterval == 0)
        {
            cout << "#";   // Print a # every hash mark interval
        }
        else
        {
            cout << "-";   // Otherwise, print a -
        }
        cout.flush();   // For smooth printing
    }

}

cout << "\n";
}




/*
 * Function for populatetree thread
 * 'status': shared data pointer
 * Read content of dictionary source file using ifstream
 * Call insert function to build the dictionary tree
 * wordInFile[DICTSRCFILEINDEX]: update total # of words of the file
 */

void * populatetree(void *arg)
{
    EXEC_STATUS* status = (EXEC_STATUS *)arg;   // Pass shared data pointer
    long n1, n2;

    //Dereference the object pointer
    status->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] = &n1;
    status->totalNumOfCharsInFile[DICTSRCFILEINDEX] = n2;


    ifstream inFile(status->filePath[DICTSRCFILEINDEX]);   // Read dictionary source file
    status->totalNumOfCharsInFile[DICTSRCFILEINDEX] = findSize(status->filePath[DICTSRCFILEINDEX]);   // Call findSize to get the total # of chars

    if(inFile)   // If file is opened
    {
        string line;
        while(getline(inFile, line))
        {
         *status->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] += line.length();   // Update # of chars processed line by line

            char *line_c = &line[0];   // Convert string to char
            char *word = strtok(line_c, delimiters);   // Split words using delimiters

            while(word != NULL)
            {
                insert(status->dictRootNode, word);   // Call insert function

                word = strtok(NULL, delimiters);   // Read next word

                status->wordCountInFile[DICTSRCFILEINDEX] += 1;   // Update total # of words in dictionary source file

            }

        }

        cout << "There are " << status->wordCountInFile[DICTSRCFILEINDEX] << " words in " << status->filePath[DICTSRCFILEINDEX] << ".\n";   // print output of word count

        inFile.close();

    }

    else   // print message and exit if failing to open file
    {
        cout << "Unable to open <<" << status->filePath[DICTSRCFILEINDEX] << ">>" << endl;;
        exit(1);
    }

    status->taskCompleted[DICTSRCFILEINDEX] = true;   // set task done

return NULL;

}



/*
 * Function for countwords thread
 * 'status': shared data pointer
 * Read content of test file using ifstream
 * Call search function to search and count words starting from a prefix
 * Save the output of search to a text file
 * wordInFile[TESTFILEINDEX]: update total # of words of the file
 */
void* countwords(void * arg)
{
    EXEC_STATUS* status = (EXEC_STATUS *)arg;   // Pass shared data pointer
    long n1, n2;   // Dereference the object pointer

    status->numOfCharsProcessedFromFile[TESTFILEINDEX] = &n1;
    status->totalNumOfCharsInFile[TESTFILEINDEX] = n2;


    ifstream inFile(status->filePath[TESTFILEINDEX]);   // Read content of test file
    ofstream outFile;
    outFile.open("countwords_output.txt");   // Create a text file to save the output

    status->totalNumOfCharsInFile[TESTFILEINDEX] = findSize(status->filePath[TESTFILEINDEX]);   // Call findSize to get the total # of chars

    if(inFile)   // If file is opened
    {
        string line;
        while(getline(inFile, line))
        {
          *status->numOfCharsProcessedFromFile[TESTFILEINDEX] += line.length();// Update # of chars line by line

            char *line_c = &line[0];   // Convert string to char
            char *word = strtok(line_c, delimiters);   // Split words using delimiters

            while(word != NULL)
            {
                status->wordCountInFile[TESTFILEINDEX]++;   // Update total # of words in test file

                int prefix = search(status->dictRootNode, word);   // Call search function

                if(prefix >= status->minNumOfWordsWithAPrefixForPrinting)  // Consider input from user. If not specified, default is 1
                {
                    outFile << word << " " << prefix << endl;   // Print output to the outFile
                }

                word = strtok(NULL, delimiters);   // Read next word

            }
        }
        cout <<  "There are " << status->wordCountInFile[TESTFILEINDEX] << " words in " << status->filePath[TESTFILEINDEX] << ".\n";   // print output of word count


        // Close files
        inFile.close();
        outFile.close();
    }
    else   // Print message and exit if failing to open file
    {
        cout << "Unable to open <<" << status->filePath[TESTFILEINDEX] << ">>" << endl;;
        exit(1);
    }

    status->taskCompleted[TESTFILEINDEX] = true;   // set task done

    return NULL;
}





/* Main function */
int main(int argc, char** argv)
{
    if(argc < 3)
    {   // The number of arguments must be equal or greater than 3 (including mandatory arguments and optional arguments)

        cout << "Too less arguments\n";   // Exit if there're less than 3
        exit(1);
    }

EXEC_STATUS* status = new EXEC_STATUS;   // Pass shared data pointer


status->filePath[DICTSRCFILEINDEX] = argv[1];   // Dictionary source file is corresponding to argv[1]
status->filePath[TESTFILEINDEX] = argv[2];   // Test file is corresponding to argv[2]


/* Set up variables for optional arguments using getopt */
int Option;
bool BADFLAG;

// Default value if optional arguments is not specified
status->numOfProgressMarks = DEFAULT_NUMOF_MARKS;
status->hashmarkInterval = DEFAULT_HASHMARKINTERVAL;
status->minNumOfWordsWithAPrefixForPrinting = DEFAULT_MINIMUM_OFWORDS_WITHAPREFIX;



while((Option = getopt(argc, argv, "p:h:n:?")) != -1)
{
    switch(Option)
    {
        case 'p':   // # of progress marks

        if(atoi(optarg) < MIN_NUMOF_MARKS)   // Use atoi(optarg) to convert string to int
        {
            cout << "Number of progress marks must be a number and at least 10 \n";
            exit(1);

        }
        else
        {
            status->numOfProgressMarks = atoi(optarg);   // Assign optarg to # of progress marks
        }

        break;


        case 'h':   // Hash mark interval

        if(atoi(optarg) > 10 || atoi(optarg) <= 0)
        {
            cout << "Hash mark interval for progress must be a number, greater than 0, and  less than or equal to 10 \n";
            exit(1);

        }
        else
        {
            status->hashmarkInterval = atoi(optarg);   // Assign optarg to hash mark interval
        }
        break;


        case 'n':   // Minimum prefix for printing

            status->minNumOfWordsWithAPrefixForPrinting = atoi(optarg);   // Assign optarg to minimum prefix for printing

            break;


        default:
            exit(BADFLAG);
            break;
    }
}



ifstream inFile1(status->filePath[DICTSRCFILEINDEX]), inFile2(status->filePath[TESTFILEINDEX]);   // Read input files
pthread_t thread1, thread2;   // Initialize threads


// Check if input files are invalid to create threads
// Print message if file is unable to open
if(!inFile1)
{
    cout << "Unable to open <<" << status->filePath[0] << ">>" << endl;
    exit(1);
}
if(!inFile2)
{
    cout << "Unable to open <<" << status->filePath[1] << ">>" << endl;
    exit(1);
}


// Create thread if dictionary source file is read

/* Thread for populatetree
 * Thread ID: thread1
 * Call populatetree function
 * Pass 'status' pointer
 */
pthread_create(&thread1, NULL, &populatetree, (void *)status);
progress_bar(status, DICTSRCFILEINDEX);   // Call progress bar concurrently


while(!status->taskCompleted[DICTSRCFILEINDEX])
{
    // Busy waiting until task completed
}


// Create thread if test file is read

/* Thread for countwords
* Thread ID: thread2
* Call countwords function
* Pass 'status' pointer
*/
pthread_create(&thread2, NULL, &countwords, (void *)status);
progress_bar(status,TESTFILEINDEX);   // Call progress bar concurrently


while(!status->taskCompleted[TESTFILEINDEX])
{
    // Busy waiting until task completed
}


delete(status);   // To avoid memory leak

return 0;
}
