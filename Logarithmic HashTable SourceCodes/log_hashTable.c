/**
 * Created by Fonkam Loic on 6/3/19.
 *
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
#include <wctype.h>

#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#include "logarithmic.h"
#include "dependencies.h"

int main(int argc, char *argv[]) {
    // benchmark variables
    double time_insertL = 0.0, time_searchL = 0.0, time_deleteL = 0.0, time_createL = 0.0, time_printL = 0.0, time_destroyL = 0.0;

    // structs for timing data
    struct rusage before, after, r_usage;

    //Root pointer for hashtable
    rootHashTable *LrootTable = NULL;

    //table capacity used
    long capUsed = 0;

    /*
     * EMPIRICAL DESIGN FOR COMPARISON BETWEEN THE LOGARITHMIC VS DICTIONARY IMPLEMENTATIONS OF DYNAMIC HASHTABLES
     */
    int reusableVariable = 0;
    bool success;
    bool useBuffer = true;  //flag to check if we need to print output in a file(creating a log of output).

    FILE *filePointer = NULL; //We will print large output here, so that we can display with less command.

    //OutputFile to store bench-mark results
    FILE *foutput = fopen("Benchmark.txt", "a+");
    if(foutput == NULL){
        printf("Error creating bench-mark file to save output :(...Aborting\n");
        return 10;
    }

    if (argc == 1) {
        short choiceOfAction = actionList();

        //We keep iterating and ask for user option until s/he choose to terminate the execution of the program.
        while (choiceOfAction != -1) {
            switch (choiceOfAction) {
                case 0:

                    printf("Enter size of hash table please: ");
                    scanf("%d", &reusableVariable);

                    /* Getrusage() RUSAGE_SELF:
                     *  Return resource usage statistics for the calling process, which is
                     * the sum of resources used by all threads in the process.
                     */

                    getrusage(RUSAGE_SELF, &before);
                    LrootTable = LcreateHashTable(LrootTable, reusableVariable);
                    getrusage(RUSAGE_SELF, &after);

                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In create func) = %ld\n", r_usage.ru_maxrss);


                    time_createL = calculate(&before, &after);

                    if (LrootTable == NULL) {
                        printf("Hash Table could not be created :(\n");
                        exit(EXIT_FAILURE);
                    }

                    printf("LOGARITHMIC  Hash-table of size %d created successfully :)\n", reusableVariable);
                    printf("\nTIME TO CREATE LOGARITHMIC HASHTABLE : %.4fs\n", time_createL);
                    break;

                case 1:
                    printf("\nEnter an integer to add(enter '-1' to quit): ");
                    scanf("%d", &reusableVariable);

                    while (reusableVariable != -1) {
                        getrusage(RUSAGE_SELF, &before);
                        success = Linsert(LrootTable, reusableVariable);
                        getrusage(RUSAGE_SELF, &after);
                        getrusage(RUSAGE_CHILDREN, &r_usage);

                        time_insertL += calculate(&before, &after);

                        if (success) {
                            printf("\nEnter another integer to add(enter '-1' to quit: ");
                            scanf("%d", &reusableVariable);
                        } else {
                            printf("Sorry there was an error inserting %d in one of the hash tables :(\n",
                                   reusableVariable);
                            break;
                        }
                    }
                    printf("Memory usage(In insert func) = %ld\n", r_usage.ru_maxrss);
                    printf("\nTIME TO INSERT INTO LOGARITHMIC HASHTABLE : %.4fs\n", time_insertL);
                    time_insertL = 0.0;
                    break;

                case 2:
                    printf("\nEnter an integer to search: ");
                    scanf("%d", &reusableVariable);

                    getrusage(RUSAGE_SELF, &before);
                    success = Lsearch(LrootTable, reusableVariable);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In search func) = %ld\n", r_usage.ru_maxrss);

                    time_searchL = calculate(&before, &after);

                    if (success) {
                        printf("The number %d is in Hash table\n", reusableVariable);
                        printf("\nTIME TO SEARCH IN LOGARITHMIC HASHTABLE : %.4fs\n", time_searchL);
                    } else {
                        printf("This number is not in the hashTables\n");
                    }
                    break;

                case 3:
                    printf("Enter the integer you wish to delete: ");
                    scanf("%d", &reusableVariable);

                    getrusage(RUSAGE_SELF, &before);
                    success = Ldelete(LrootTable, reusableVariable);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In delete func) = %ld\n", r_usage.ru_maxrss);

                    time_deleteL = calculate(&before, &after);

                    if (success) {
                        printf("Value deleted successfully\n");
                        printf("\nTIME TO DELETE FROM LOGARITHMIC HASHTABLE : %.4fs\n", time_deleteL);
                    } else {
                        printf("The is no %d in the hashTables\n", reusableVariable);
                    }
                    break;

                case 4:
                    //The output of printing can overflow the terminal buffer, so we create a buffer.txt file an log the output.
                    filePointer = fopen("buffer.txt", "w");
                    if (filePointer == NULL) {
                        useBuffer = false;
                    }

                    getrusage(RUSAGE_SELF, &before);
                    LprintTable(LrootTable, filePointer);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In print func) = %ld\n", r_usage.ru_maxrss);

                    time_printL = calculate(&before, &after);
                    if (useBuffer) {
                        printf("\nTIME TO PRINT LOGARITHMIC HASHTABLE : %.4fs\n", time_printL);
                        printf("Complete output has been logged in the ./buffer.txt\n");
                    }
                    if (useBuffer) {
                        fprintf(filePointer, "\nTIME TO PRINT LOGARITHMIC HASHTABLE : %.4fs\n", time_printL);
                    }

                    break;

                case 5:

                    //Here we get the amount of data we want to flood the hash table with from the user.
                    printf("Enter the date set size (e.g 4000,...,50000000, go higher if you have more memory): ");
                    scanf("%d", &reusableVariable);

                    //We will keep track of the random value that was generated, since our hash table doesn't support duplicate data.
                    //bool *arr = calloc(sizeof(bool), reusableVariable);
                    //time_t t;

                    //Let seed the random function with the current system time.
                    //srand((unsigned) time(&t));

                    for (int i = 0; i < reusableVariable; i++) {
                        //The values to flood are randomly generated.
                       // int value = rand() % reusableVariable;
                        long value = floodCount(true);

                            getrusage(RUSAGE_SELF, &before);
                            success = Linsert(LrootTable, value);
                            getrusage(RUSAGE_SELF, &after);
                            getrusage(RUSAGE_CHILDREN, &r_usage);

                            time_insertL += calculate(&before, &after);

                            if (!success) {
                                printf("Error occurred while flooding Hash Table with random data... Flooding aborted :(\n");
                                break;
                            }



                    }
                    //free(arr);
                    floodCount(false);
                    printf("Memory usage(In insert func) = %ld\n", r_usage.ru_maxrss);
                    printf("\nTIME TO FLOOD LOGARITHMIC HASHTABLE : %.4fs\n", time_insertL);

                    break;

                case 8:

                    getrusage(RUSAGE_SELF, &before);
                    success = LdestroyHashTable(LrootTable);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In destroy func) = %ld\n", r_usage.ru_maxrss);

                    time_destroyL = calculate(&before, &after);

                    if (success) {
                        LrootTable = NULL;
                        printf("HashTable has been destroyed successfully :)\n");
                        printf("\nTIME TO DESTROY LOGARITHMIC HASHTABLE : %.4fs\n", time_destroyL);
                    } else {
                        printf("HashTable could not be destroyed :(\n");
                    }
                    break;

                            case 9:
                                //size
                                capUsed = LrootTable == NULL? 0 : lSize(LrootTable);
                    printf("\nNUMBER OF DATA LOADED IN HASHTABLE : %ld\n", capUsed);
                    break;
                case 10:
                    usage();
                    break;

                default:
                    printf("Sorry the option you choose is still in development...:(\n");
                    break;
            }
            printf(".");   //prints three waiting dots.
            fflush(stdout);
            sleep(1);
            printf(".");
            fflush(stdout);
            sleep(1);
            printf(".");
            fflush(stdout);
            sleep(1);
            printf("\n");
            choiceOfAction = actionList();
        }
        LdestroyHashTable(LrootTable);
    } else if (argc == 2) {
        usage();
    } else if (argc == 3) {
        char number[50], c[3];
        strncpy(number, argv[2], sizeof(number));
        strncpy(c, argv[1], sizeof(c));
        int check = strcmp(c, "-s");
        wint_t num = atoi(number);
        if (strncmp(c, "-s", sizeof(char) * 2) != 0 && strncmp(c, "-l", sizeof(char) * 2) != 0) {
            usage();
            return 3;
        } else if (check== 0 && num > 0) {

            //Create
            getrusage(RUSAGE_SELF, &before);
            LrootTable = LcreateHashTable(LrootTable, num);
            getrusage(RUSAGE_SELF, &after);


            time_createL = calculate(&before, &after);
            if (LrootTable == NULL) {
                printf("Hash Table could not be created :(\n");
                exit(EXIT_FAILURE);
            }

            //insert
            for (int i = 0; i < DEFAULTFLOOD; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                    getrusage(RUSAGE_SELF, &before);
                    success = Linsert(LrootTable, value);
                    getrusage(RUSAGE_SELF, &after);



                    time_insertL += calculate(&before, &after);
                    if (!success) {
                        printf("Sorry there was an error inserting %d in one of the hash tables :(\n",value);
                        continue;
                    }

                }
            floodCount(false);


            //size
            capUsed = lSize(LrootTable);

            //search
            for (int i = 0; i < DEFAULTFLOOD ; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Lsearch(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_searchL += calculate(&before, &after);

                if(!success){
                  //  printf("The number %d is not in the hashTables\n", value);
                }
            }
            //destroy
            for (int i = 0; i < DEFAULTFLOOD ; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Ldelete(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_deleteL += calculate(&before, &after);

                if(!success){
                   // printf("The number %d is not in the hashTables\n", value);
                }
            }


            //print to output file
            fprintf(foutput, "%12ld\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12ld\n", capUsed, time_insertL, time_searchL, time_deleteL, after.ru_maxrss);


            //printout
            printf("\n\n\nLOGARITHMIC  Hash-table of size %d created successfully :)\n", num);
            printf("\nTIME TO CREATE LOGARITHMIC HASHTABLE : %.4fs\n", time_createL);
            printf("\nTIME TO FLOOD LOGARITHMIC HASHTABLE : %.4fs\n", time_insertL);
            printf("\nNUMBER OF DATA LOADED IN HASHTABLE : %ld\n", capUsed);
            printf("\nTIME TO SEARCH IN LOGARITHMIC HASHTABLE : %.4fs\n", time_searchL);
            printf("\nTIME TO DELETE FROM LOGARITHMIC HASHTABLE : %.4fs\n", time_deleteL);
            printf("Memory usage = %ld\n", after.ru_maxrss);


        }

         else if (strncmp(argv[1], "-l", sizeof(char) * 2) == 0 && num > 0) {
//Create
            getrusage(RUSAGE_SELF, &before);
            LrootTable = LcreateHashTable(LrootTable, DEFAULTSIZE);
            getrusage(RUSAGE_SELF, &after);

            time_createL = calculate(&before, &after);

            if (LrootTable == NULL) {
                printf("Hash Table could not be created :(\n");
                exit(EXIT_FAILURE);
            }
            //insert
            for (int i = 0; i < num; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Linsert(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);



                time_insertL += calculate(&before, &after);
                if (!success) {
                    printf("Sorry there was an error inserting %d in one of the hash tables :(\n",value);
                    continue;
                }


            }
            floodCount(false);

            //size
            capUsed = lSize(LrootTable);


            //search
            for (int i = 0; i < num; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Lsearch(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_searchL += calculate(&before, &after);
                if(!success){
                   // printf("The number %d is not in the hashTables\n", value);
                }
            }
            //destroy
            for (int i = 0; i < num; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Ldelete(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_deleteL += calculate(&before, &after);

                if(!success){
                   // printf("The number %d is not in the hashTables\n", value);
                }
            }

            //print to output file
            fprintf(foutput, "%12ld\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12ld\n", capUsed, time_insertL, time_searchL, time_deleteL, after.ru_maxrss);

            //printout
            printf("\n\n\nLOGARITHMIC  Hash-table of size %d created successfully :)\n", num);
            printf("\nTIME TO CREATE LOGARITHMIC HASHTABLE : %.4fs\n", time_createL);
            printf("\nTIME TO FLOOD LOGARITHMIC HASHTABLE : %.4fs\n", time_insertL);
            printf("\nNUMBER OF DATA LOADED IN HASHTABLE : %ld\n", capUsed);
            printf("\nTIME TO SEARCH IN LOGARITHMIC HASHTABLE : %.4fs\n", time_searchL);
            printf("\nTIME TO DELETE FROM LOGARITHMIC HASHTABLE : %.4fs\n", time_deleteL);
            printf("Memory usage = %ld\n", after.ru_maxrss);


        } else {
            usage();
            return 4;
        }
    } else if (argc == 4) {
        usage();
        return 5;
    } else if (argc == 5) {
        char number1[50], number2[50];
        strncpy(number1, argv[2], sizeof(number1));
        strncpy(number2, argv[4], sizeof(number2));
        int num = atoi(number1), num2 = atoi(number2);
        if (num  < 1 ) {
            usage();
            return 6;
        } else if (strncmp(argv[1], "-s", sizeof(char) * 2) == 0 && strncmp(argv[3], "-l", sizeof(char) * 2) == 0) {

            if(num2 < 1){
                usage();
                return 7;
            }
            //Create
            getrusage(RUSAGE_SELF, &before);
            LrootTable = LcreateHashTable(LrootTable, num);
            getrusage(RUSAGE_SELF, &after);


            time_createL = calculate(&before, &after);
            if (LrootTable == NULL) {
                printf("Hash Table could not be created :(\n");
                exit(EXIT_FAILURE);
            }

            //insert
            for (int i = 0; i < num2; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Linsert(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);



                time_insertL += calculate(&before, &after);
                if (!success) {
                    printf("Sorry there was an error inserting %d in one of the hash tables :(\n",value);
                    continue;
                }

            }
            floodCount(false);


            //size
            capUsed = lSize(LrootTable);


            //search
            for (int i = 0; i < num2 ; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Lsearch(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_searchL += calculate(&before, &after);

                if(!success){
                 //   printf("The number %d is not in the hashTables\n", value);
                }
            }
            //destroy
            for (int i = 0; i < num2 ; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Ldelete(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_deleteL += calculate(&before, &after);

                if(!success){
                  //  printf("The number %d is not in the hashTables\n", value);
                }
            }

            //print to output file
            fprintf(foutput, "%12ld\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12ld\n", capUsed, time_insertL, time_searchL, time_deleteL, after.ru_maxrss);


            //printout
            printf("\n\n\nLOGARITHMIC  Hash-table of size %d created successfully :)\n", num);
            printf("\nTIME TO CREATE LOGARITHMIC HASHTABLE : %.4fs\n", time_createL);
            printf("\nTIME TO FLOOD LOGARITHMIC HASHTABLE : %.4fs\n", time_insertL);
            printf("\nNUMBER OF DATA LOADED IN HASHTABLE : %ld\n", capUsed);
            printf("\nTIME TO SEARCH IN LOGARITHMIC HASHTABLE : %.4fs\n", time_searchL);
            printf("\nTIME TO DELETE FROM LOGARITHMIC HASHTABLE : %.4fs\n", time_deleteL);
            printf("Memory usage = %ld\n", after.ru_maxrss);



        } else if (strncmp(argv[1], "-s", sizeof(char) * 2) == 0 && strncmp(argv[3], "-f", sizeof(char) * 2) == 0) {

            //Create
            getrusage(RUSAGE_SELF, &before);
            LrootTable = LcreateHashTable(LrootTable, num);
            getrusage(RUSAGE_SELF, &after);


            time_createL = calculate(&before, &after);
            if (LrootTable == NULL) {
                printf("Hash Table could not be created :(\n");
                exit(EXIT_FAILURE);
            }

            //insert
            FILE *inputFile = fopen(number2, "r");
            if(inputFile == NULL){
                printf("Error: File could not be open OR invalid file name :(\n");
                return 9;
            }
            long value;
            while(fscanf(filePointer, "%ld\n", &value) != EOF){
                //The values to flood are randomly generated.

                getrusage(RUSAGE_SELF, &before);
                success = Linsert(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);



                time_insertL += calculate(&before, &after);
                if (!success) {
                    printf("Sorry there was an error inserting %ld in one of the hash tables :(\n", value);
                    continue;
                }
            }

            floodCount(false);

            //size
            capUsed = lSize(LrootTable);



            //search
            for (int i = 0; i < num ; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Lsearch(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_searchL += calculate(&before, &after);

                if(!success){
                   // printf("The number %d is not in the hashTables\n", value);
                }
            }
            //destroy
            for (int i = 0; i < num ; i++) {
                //The values to flood are randomly generated.
                int value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Ldelete(LrootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_deleteL += calculate(&before, &after);

                if(!success){
                   // printf("The number %d is not in the hashTables\n", value);
                }
            }
            //print to output file
            fprintf(foutput, "%12ld\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12ld\n", capUsed, time_insertL, time_searchL, time_deleteL, after.ru_maxrss);


            //printout
            printf("\n\n\nLOGARITHMIC  Hash-table of size %d created successfully :)\n", num);
            printf("\nTIME TO CREATE LOGARITHMIC HASHTABLE : %.4fs\n", time_createL);
            printf("\nTIME TO FLOOD LOGARITHMIC HASHTABLE : %.4fs\n", time_insertL);
            printf("\nNUMBER OF DATA LOADED IN HASHTABLE : %ld\n", capUsed);
            printf("\nTIME TO SEARCH IN LOGARITHMIC HASHTABLE : %.4fs\n", time_searchL);
            printf("\nTIME TO DELETE FROM LOGARITHMIC HASHTABLE : %.4fs\n", time_deleteL);
            printf("Memory usage = %ld\n", after.ru_maxrss);


        } else {
            usage();
            return 6;
        }
    } else if (argc > 5) {
        usage();
        return 7;
    }

    // And we are done!
    return 0;
}
