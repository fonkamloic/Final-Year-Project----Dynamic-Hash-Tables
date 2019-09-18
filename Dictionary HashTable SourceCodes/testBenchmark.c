/**
 * Created by Fonkam Loic on 6/3/19.
 *
 * Implements a test/driver file for comparison between the dictionary vs logarithmic implementation of a dynamic hash table.
 *
 * How will this be done?
 *  -The Interface will show options of operations that can be performed a hash table.
 *  -For every option that is selected, the operation will be performed on two separate instances of the different type of dynamic hash tables symultanously.
 *  
 *  -This code will also benchmark the the execution/calling of each function and recording the time use by each call.
 *  
 */

#include <ctype.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#include "dictionary.h"
#include "logarithmic.h"
#include "dependencies.h"

int main(int argc, char *argv[])
{
    // benchmark variables
    double time_insertD = 0.0, time_searchD = 0.0, time_deleteD = 0.0, time_createD = 0.0, time_printD = 0.0, time_destroyD = 0.0, time_rehash = 0.0;
    double time_insertL = 0.0, time_searchL = 0.0, time_deleteL = 0.0, time_createL = 0.0, time_printL = 0.0, time_destroyL = 0.0;

    // structs for timing data
    struct rusage before, after;

    //Root pointers of both type of hash table
    HashTable *rootTable = NULL;
    rootHashTable *LrootTable = NULL;

    /*
     * EMPIRICAL DESIGN FOR COMPARISON BETWEEN THE LOGARITHMIC VS DICTIONARY IMPLEMENTATIONS OF DYNAMIC HASHTABLES
     */
    int reusableVariable = 0;
    bool success1, success2;
    bool useBuffer = true;  //flag to check if we need to print output in a file and display will a less command or not.
    bool didRehash = false; // flag to check if we need to rehash the dictionary hash table.

    FILE *filePointer = NULL; //We will print large output here, so that we can display with less command.

    if (argc == 1)
    {
        //We get the option the user wants to perform
        short choiceOfAction = actionList();

        //We keep iterating and ask for user option until s/he choose to terminate the execution of the program.
        while (choiceOfAction != -1)
        {
            switch (choiceOfAction)
            {
            case 0:

                printf("Enter size of hash table please: ");
                scanf("%d", &reusableVariable);

                /* Getrusage() RUSAGE_SELF: 
                 *  Return resource usage statistics for the calling process, which is                                                                                                                
                 * the sum of resources used by all threads in the process.
                 */
                getrusage(RUSAGE_SELF, &before);
                rootTable = DcreateHashTable(reusableVariable);
                //getrusage starts another timing
                getrusage(RUSAGE_SELF, &after);

                time_createD = calculate(&before, &after);

                getrusage(RUSAGE_SELF, &before);
                LrootTable = LcreateHashTable(LrootTable, reusableVariable);
                getrusage(RUSAGE_SELF, &after);

                time_createL = calculate(&before, &after);

                //we  stop the execution if any of the hash table was not created.
                if (rootTable == NULL || LrootTable == NULL)
                {
                    printf("Hash Table could not be created :(\n");
                    exit(EXIT_FAILURE);
                }

                printf("Two Hash tables (LOGARITHMIC $ DICTIONARY) of size %d created successfully :)\n", reusableVariable);
                printf("\nTIME TO CREATE DICTIONARY HASHTABLE : %.4fs\nTIME TO CREATE LOGORITHMIC HASHTABLE : %.4fs\n", time_createD, time_createL);
                break;

            case 1:
                printf("\nEnter an integer to add(enter '-1' to quit): ");
                scanf("%d", &reusableVariable);

                while (reusableVariable != -1)
                {

                    getrusage(RUSAGE_SELF, &before);
                    success1 = Dinsert(rootTable, reusableVariable);
                    getrusage(RUSAGE_SELF, &after);

                    //We sum the execution time for each insert function call.
                    time_insertD += calculate(&before, &after);

                    //Rehash Dictionary Hashtable if LOAD_FACTOR is exceed.
                    if (rootTable != NULL && (float)rootTable->capacityUsed / rootTable->size > LOAD_FACTOR)
                    {
                        HashTable *biggerTable = NULL;
                        getrusage(RUSAGE_SELF, &before);
                        //rehash create a bigger table, rehash all data in the old table into the new table and delete the old table.
                        rootTable = rehash(biggerTable, rootTable);
                        getrusage(RUSAGE_SELF, &after);

                        time_rehash = calculate(&before, &after);
                        didRehash = true;
                    }

                    getrusage(RUSAGE_SELF, &before);
                    success2 = Linsert(LrootTable, reusableVariable);
                    getrusage(RUSAGE_SELF, &after);

                    time_insertL += calculate(&before, &after);

                    if (success1 && success2)
                    {
                        printf("\nEnter another integer to add(enter '-1' to quit: ");
                        scanf("%d", &reusableVariable);
                    }
                    else
                    {
                        printf("Sorry there was an error inserting %d in one of the hash tables :(\n", reusableVariable);
                        break;
                    }
                }
                didRehash == true ? printf("TIME TO REHASH THE DICTIONARY HASHTABLE : %.4fs\n", time_rehash) : printf("\n");
                didRehash = false;
                printf("\nTIME TO INSERT INTO DICTIONARY HASHTABLE : %.4fs\nTIME TO INSERT INTO LOGORITHMIC HASHTABLE : %.4fs\n", time_insertD, time_insertL);
                time_insertD = 0.0, time_insertL = 0.0;
                break;

            case 2:
                printf("\nEnter an integer to search: ");
                scanf("%d", &reusableVariable);

                getrusage(RUSAGE_SELF, &before);
                success1 = Dsearch(rootTable, reusableVariable);
                getrusage(RUSAGE_SELF, &after);

                time_searchD = calculate(&before, &after);

                getrusage(RUSAGE_SELF, &before);
                success2 = Lsearch(LrootTable, reusableVariable);
                getrusage(RUSAGE_SELF, &after);

                time_searchL = calculate(&before, &after);

                if (success1 && success2)
                {
                    printf("The number %d is in both Hash table\n", reusableVariable);
                    printf("\nTIME TO SEARCH IN DICTIONARY HASHTABLE : %.4fs\nTIME TO SEARCH IN LOGORITHMIC HASHTABLE : %.4fs\n", time_searchD, time_searchL);
                }
                else
                {
                    printf("This number is not in the hashTables\n");
                }
                break;

            case 3:
                printf("Enter the integer you wish to delete: ");
                scanf("%d", &reusableVariable);

                getrusage(RUSAGE_SELF, &before);
                success1 = Ddelete(rootTable, reusableVariable);
                getrusage(RUSAGE_SELF, &after);

                time_deleteD = calculate(&before, &after);

                getrusage(RUSAGE_SELF, &before);
                success2 = Ldelete(LrootTable, reusableVariable);
                getrusage(RUSAGE_SELF, &after);

                time_deleteL = calculate(&before, &after);

                if (success1 && success2)
                {
                    printf("Value deleted successfully\n");
                    printf("\nTIME TO DELETE FROM DICTIONARY HASHTABLE : %.4fs\nTIME TO DELETE FROM LOGORITHMIC HASHTABLE : %.4fs\n", time_deleteD, time_deleteL);
                }
                else
                {
                    printf("The is no %d in the hashTables\n", reusableVariable);
                }
                break;

            case 4:
                //The output of printing can overflow the terminal buffer, so we create a buffer.txt file an log the output.
                filePointer = fopen("buffer.txt", "w");
                if (filePointer == NULL)
                {
                    useBuffer = false;
                }

                getrusage(RUSAGE_SELF, &before);
                DprintTable(rootTable, filePointer);
                getrusage(RUSAGE_SELF, &after);

                time_printD = calculate(&before, &after);

                getrusage(RUSAGE_SELF, &before);
                LprintTable(LrootTable, filePointer);
                getrusage(RUSAGE_SELF, &after);

                time_printL = calculate(&before, &after);
                if (useBuffer)
                {
                    printf("\nTIME TO PRINT DICTIONARY HASHTABLE : %.4fs\nTIME TO PRINT LOGORITHMIC HASHTABLE : %.4fs\n", time_printD, time_printL);
                    printf("Complete output has been logged in the ./buffer.txt\n");
                }
                if (useBuffer)
                {
                    fprintf(filePointer, "\nTIME TO PRINT DICTIONARY HASHTABLE : %.4fs\nTIME TO PRINT LOGORITHMIC HASHTABLE : %.4fs\n", time_printD, time_printL);
                    fclose(filePointer);
                    // sleep(1);
                    // system("echo \"$(cat \"$buffer.txt\")\" | \"less\"");
                }

                break;

            case 5:

                //Here we get the amount of data we want to flood the hash table with from the user.
                printf("Enter the date set size (e.g 500, 1000, 4000, go higher if you have more memory): ");
                scanf("%d", &reusableVariable);

                //We will keep track of the random value that was generated, since our hash table doesn't support duplicate data.
                bool *arr = calloc(sizeof(bool), reusableVariable);
                time_t t;

                //Let seed the random function with the current system time.
                srand((unsigned)time(&t));

                for (int i = 0; i < reusableVariable; i++)
                {
                    //The values to flood are randomly generated.
                    int value = rand() % reusableVariable;


                    //Rehash Dictionary Hashtable if need be
                    if (rootTable != NULL && (float)rootTable->capacityUsed / rootTable->size > LOAD_FACTOR)
                    {
                        HashTable *biggerTable = NULL;
                        getrusage(RUSAGE_SELF, &before);
                        rootTable = rehash(biggerTable, rootTable);
                        getrusage(RUSAGE_SELF, &after);

                        time_rehash = calculate(&before, &after);
                        didRehash = true;
                    }

                    if (!arr[value])
                    {
                        getrusage(RUSAGE_SELF, &before);
                        success1 = Dinsert(rootTable, value);
                        getrusage(RUSAGE_SELF, &after);

                        time_insertD += calculate(&before, &after);

                        getrusage(RUSAGE_SELF, &before);
                        success2 = Linsert(LrootTable, value);
                        getrusage(RUSAGE_SELF, &after);

                        time_insertL += calculate(&before, &after);

                        if (!success1 || !success2)
                        {
                            printf("Error occurred while flooding Hash Table with random data... Flooding aborted :(\n");
                            break;
                        }

                    }
                    else //we backtrack if rand() generate a number we already have.
                    {
                        i--;
                    }
                    arr[value] = 1;




                }
                free(arr);
                didRehash == true ? printf("\nTIME TO REHASH THE DICTIONARY HASHTABLE : %.4fs\n", time_rehash) : printf(" 0");
                didRehash = false;
                printf("\nTIME TO FLOOD DICTIONARY HASHTABLE : %.4fs\nTIME TO FLOOD LOGORITHMIC HASHTABLE : %.4fs\n", time_insertD, time_insertL);

                break;

                //                case 6:
                //                    break;

            case 8:

                getrusage(RUSAGE_SELF, &before);
                success1 = DdestroyHashTable(rootTable);
                getrusage(RUSAGE_SELF, &after);

                time_destroyD = calculate(&before, &after);

                getrusage(RUSAGE_SELF, &before);
                success2 = LdestroyHashTable(LrootTable);
                getrusage(RUSAGE_SELF, &after);

                time_destroyL = calculate(&before, &after);

                if (success1 && success2)
                {
                    rootTable = NULL, LrootTable = NULL;
                    printf("HashTables were destroyed successfully :)\n");
                    printf("\nTIME TO DESTROY DICTIONARY HASHTABLE : %.4fs\nTIME TO DESTROY LOGORITHMIC HASHTABLE : %.4fs\n", time_destroyD, time_destroyL);
                }
                else
                {
                    printf("HashTables could not be destroyed :(\n");
                }
                break;

                //                case 7:
                //
                //
                //                    break;

            default:
                printf("Sorry the option you choose is still in development...:(\n");
                break;
            }
            choiceOfAction = actionList();
        }
//        free(rootTable);
//        free(LrootTable);
        DdestroyHashTable(rootTable);
        LdestroyHashTable(LrootTable);
    }

    else if (argc > 1)
    {
        printf("Usage: ./testBenchmark ");
        return 1;
    }
    // And we are done!
    return 0;
}
