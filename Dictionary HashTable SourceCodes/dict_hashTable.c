/**
 * Created by Fonkam Doic on 6/3/19.
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
#include "dependencies.h"

int main(int argc, char *argv[]) {
    // benchmark variables
    double time_insertD = 0.0, time_searchD = 0.0, time_deleteD = 0.0, time_createD = 0.0, time_printD = 0.0, time_destroyD = 0.0, time_rehash = 0.0;

    // structs for timing data
    struct rusage before, after, r_usage;

    //Root pointers of both type of hash table
    HashTable *rootTable = NULL;

    //capacity of hash table used
    long capUsed = 0;


    //OutputFile to store bench-mark results
    FILE *foutput = fopen("Benchmark.txt", "a+");
    if(foutput == NULL){
        printf("Error creating bench-mark file to save output :(...Aborting\n");
        return 10;
    }

    /*
     * EMPIRICAL DESIGN FOR COMPARISON BETWEEN THE LOGARITHMIC VS DICTIONARY IMPLEMENTATIONS OF DYNAMIC HASHTABLES
     */
    int reusableVariable = 0;
    bool success;
    bool useBuffer = true;  //flag to check if we need to print output in a file and display will a less command or not.
    bool didRehash = false; // flag to check if we need to rehash the dictionary hash table.

    FILE *filePointer = NULL; //We will print large output here, so that we can display with less command.

    if (argc == 1) {
        //We get the option the user wants to perform
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
                    rootTable = DcreateHashTable(reusableVariable);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In create func) = %ld\n", r_usage.ru_maxrss);

                    time_createD = calculate(&before, &after);



                    //we  stop the execution if any of the hash table was not created.
                    if (rootTable == NULL) {
                        printf("Hash Table could not be created :(\n");
                        exit(EXIT_FAILURE);
                    }

                    printf("Dictionary Hash table of size %d created successfully :)\n", reusableVariable);
                    printf("\nTIME TO CREATE DICTIONARY HASHTABLE : %.4fs\n", time_createD);
                    break;

                case 1:
                    printf("\nEnter an integer to add(enter '-1' to quit): ");
                    scanf("%d", &reusableVariable);

                    while (reusableVariable != -1) {
                        getrusage(RUSAGE_SELF, &before);
                        success = Dinsert(rootTable, reusableVariable);
                        getrusage(RUSAGE_SELF, &after);
                        getrusage(RUSAGE_CHILDREN, &r_usage);
                        printf("Memory usage(In insert func) = %ld\n", r_usage.ru_maxrss);

                        //We sum the execution time for each insert function call.
                        time_insertD += calculate(&before, &after);

                        //Rehash Dictionary Hashtable if LOAD_FACTOR is exceed.
                        if (rootTable != NULL && (float) rootTable->capacityUsed / rootTable->size > LOAD_FACTOR) {
                            HashTable *biggerTable = NULL;
                            getrusage(RUSAGE_SELF, &before);
                            //rehash create a bigger table, rehash all data in the old table into the new table and delete the old table.
                            rootTable = rehash(biggerTable, rootTable);
                            getrusage(RUSAGE_SELF, &after);
                            getrusage(RUSAGE_CHILDREN, &r_usage);
                            printf("Memory usage(In rehash func) = %ld\n", r_usage.ru_maxrss);

                            time_rehash += calculate(&before, &after);
                            didRehash = true;
                        }


                        if (success) {
                            printf("\nEnter another integer to add(enter '-1' to quit: ");
                            scanf("%d", &reusableVariable);
                        } else {
                            printf("Sorry there was an error inserting %d in one of the hash tables :(\n",
                                   reusableVariable);
                            break;
                        }
                    }
                    didRehash == true ? printf("TIME TO REHASH THE DICTIONARY HASHTABLE : %.4fs\n", time_rehash)
                                      : printf("\n");
                    didRehash = false;
					time_rehash = 0.0;
                    printf("\nTIME TO INSERT INTO DICTIONARY HASHTABLE : %.4fs\n", time_insertD);
                    time_insertD = 0.00;
                    break;

                case 2:
                    printf("\nEnter an integer to search: ");
                    scanf("%d", &reusableVariable);

                    getrusage(RUSAGE_SELF, &before);
                    success = Dsearch(rootTable, reusableVariable);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In search func) = %ld\n", r_usage.ru_maxrss);
                    time_searchD = calculate(&before, &after);


                    if (success) {
                        printf("The number %d is in Hash table\n", reusableVariable);
                        printf("\nTIME TO SEARCH IN DICTIONARY HASHTABLE : %.4fs\n", time_searchD);
                    } else {
                        printf("This number is not in the hashTables\n");
                    }
                    break;

                case 3:
                    printf("Enter the integer you wish to delete: ");
                    scanf("%d", &reusableVariable);

                    getrusage(RUSAGE_SELF, &before);
                    success = Ddelete(rootTable, reusableVariable);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In delete func) = %ld\n", r_usage.ru_maxrss);

                    time_deleteD = calculate(&before, &after);

                    if (success) {
                        printf("Value deleted successfully\n");
                        printf("\nTIME TO DELETE FROM DICTIONARY HASHTABLE : %.4fs\n", time_deleteD);
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
                    DprintTable(rootTable, filePointer);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In print func) = %ld\n", r_usage.ru_maxrss);

                    time_printD = calculate(&before, &after);


                    if (useBuffer) {
                        printf("\nTIME TO PRINT DICTIONARY HASHTABLE : %.4fs", time_printD);
                        printf("Complete output has been logged in the ./buffer.txt\n");
                    }
                    if (useBuffer) {
                        fprintf(filePointer, "\nTIME TO PRINT DICTIONARY HASHTABLE : %.4fs\n", time_printD);
                        fclose(filePointer);
                    }

                    break;

                case 5:

                    //Here we get the amount of data we want to flood the hash table with from the user.
                    printf("Enter the date set size (e.g 500, 1000, 4000, go higher if you have more memory): ");
                    scanf("%d", &reusableVariable);

                    //We will keep track of the random value that was generated, since our hash table doesn't support duplicate data.
                   // bool *arr = calloc(sizeof(bool), reusableVariable);
                    //time_t t;

                    //Let seed the random function with the current system time.
                    //srand((unsigned) time(&t));

                    for (int i = 0; i < reusableVariable; i++) {
                        //The values to flood are randomly generated.
                 //       int value = rand() % reusableVariable;
    long value = floodCount(true);

                        //Rehash Dictionary Hashtable if need be
                        if (rootTable != NULL && (float) rootTable->capacityUsed / rootTable->size > LOAD_FACTOR) {
                            HashTable *biggerTable = NULL;
                            getrusage(RUSAGE_SELF, &before);
                            rootTable = rehash(biggerTable, rootTable);
                            getrusage(RUSAGE_SELF, &after);
                            getrusage(RUSAGE_CHILDREN, &r_usage);
                            printf("Memory usage(In rehash func) = %ld\n", r_usage.ru_maxrss);

                            time_rehash += calculate(&before, &after);
                            didRehash = true;
                        }
                            getrusage(RUSAGE_SELF, &before);
                            success = Dinsert(rootTable, value);
                            getrusage(RUSAGE_SELF, &after);
                            getrusage(RUSAGE_CHILDREN, &r_usage);
                            printf("Memory usage(In insert func) = %ld\n", r_usage.ru_maxrss);

                            time_insertD += calculate(&before, &after);


                            if (!success) {
                                printf("Error occurred while flooding Hash Table with random data... Flooding aborted :(\n");
                                break;
                            }




                    }
                    //free(arr);
                    floodCount(false);
                    didRehash == true ? printf("\nTIME TO REHASH THE DICTIONARY HASHTABLE : %.4fs\n", time_rehash)
                                      : printf(" 0");
                    didRehash = false;
					time_rehash = 0.0;
                    printf("\nTIME TO FLOOD DICTIONARY HASHTABLE : %.4fs\n", time_insertD);

                    break;

                    //                case 6:
                    //                    break;

                case 8:

                    getrusage(RUSAGE_SELF, &before);
                    success = DdestroyHashTable(rootTable);
                    getrusage(RUSAGE_SELF, &after);
                    getrusage(RUSAGE_CHILDREN, &r_usage);
                    printf("Memory usage(In destroy func) = %ld\n", r_usage.ru_maxrss);


                    time_destroyD = calculate(&before, &after);


                    if (success) {
                        rootTable = NULL;
                        printf("HashTables were destroyed successfully :)\n");
                        printf("\nTIME TO DESTROY DICTIONARY HASHTABLE : %.4fs\n", time_destroyD);
                    } else {
                        printf("HashTables could not be destroyed :(\n");
                    }
                    break;

                case 9:
                    capUsed = rootTable == NULL ? 0 : dSize(rootTable);
                    printf("\nTHE NUMBER OF DATA LOADED INTO HASHTABLE IS %ld\n",capUsed);
                 break;
                case 10:
                    usage();
                    break;
                default:
                    printf("Sorry the option you choose is still in development...:(\n");
                    break;
            }
            printf(".");   //prints three waiting dots.
            fflush(stdout); // print everything in the stdout buffer
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
//        free(rootTable);
        DdestroyHashTable(rootTable);
    } else if (argc == 2) {
        usage();
    } else if (argc == 3) {
        //variable to store number and flags
        char number[50], c[3];

        //copy number and flags from command line argument array to store
        strncpy(number, argv[2], sizeof(number));
        strncpy(c, argv[1], sizeof(c));

        //check keeps flag of table_size
        int check = strcmp(c, "-s");

        //convert string of number to integer
        int num = atoi(number);

        //check for correct command line arguments
        if (strncmp(c, "-s", sizeof(char) * 2) != 0 && strncmp(c, "-l", sizeof(char) * 2) != 0) {
            usage();
            return 3;
        } else if (check == 0 && num > 0) {

            //Create
            getrusage(RUSAGE_SELF, &before);
            rootTable = DcreateHashTable(num);
            getrusage(RUSAGE_SELF, &after);


            time_createD = calculate(&before, &after);
            if (rootTable == NULL) {
                printf("Hash Table could not be created :(\n");
                exit(EXIT_FAILURE);
            }

            //insert
            for (int i = 0; i < DEFAULTFLOOD; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Dinsert(rootTable, value);
                getrusage(RUSAGE_SELF, &after);


                time_insertD += calculate(&before, &after);


                if (!success) {
                    printf("Sorry there was an error inserting %ld in one of the hash tables :(\n", value);
                    continue;
                }

                //Rehash Dictionary Hashtable if LOAD_FACTOR is exceed.
                if (rootTable != NULL && (float) rootTable->capacityUsed / rootTable->size > LOAD_FACTOR) {
                    HashTable *biggerTable = NULL;
                    getrusage(RUSAGE_SELF, &before);
                    //rehash create a bigger table, rehash all data in the old table into the new table and delete the old table.
                    rootTable = rehash(biggerTable, rootTable);
                    getrusage(RUSAGE_SELF, &after);

                    time_rehash = +calculate(&before, &after);
                    didRehash = true;
                }

            }
            floodCount(false); //reset counter

            //size
            capUsed = dSize(rootTable);

            //search
            for (int i = 0; i < DEFAULTFLOOD; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Dsearch(rootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_searchD += calculate(&before, &after);

                if (!success) {
                  //  printf("The number %ld is not in the hashTables\n", value);
                }
            }
            //destroy
            for (int i = 0; i < DEFAULTFLOOD; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Ddelete(rootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_deleteD += calculate(&before, &after);

                if (!success) {
                   // printf("The number %ld is not in the hashTables\n", value);
                }
            }


            //print to output file
            fprintf(foutput, "%12ld\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12ld\n", capUsed, time_insertD, time_rehash, time_searchD, time_deleteD, after.ru_maxrss);

            //printout
            printf("\n\n\nDICTIONARY  Hash-table of size %d created successfully :)\n", num);
            printf("\nTIME TO CREATE DICTIONARY HASHTABLE : %.4fs\n", time_createD);
            printf("\nTIME TO FLOOD DICTIONARY HASHTABLE : %.4fs\n", time_insertD);
            printf("\nTHE NUMBER OF DATA LOADED INTO HASHTABLE IS %ld\n",capUsed);
            didRehash == true ? printf("TIME TO REHASH THE DICTIONARY HASHTABLE : %.4fs\n", time_rehash)
                              : printf("\n");
			didRehash = false;
            printf("\nTIME TO SEARCH IN DICTIONARY HASHTABLE : %.4fs\n", time_searchD);
            printf("\nTIME TO DELETE FROM DICTIONARY HASHTABLE : %.4fs\n", time_deleteD);
            printf("Memory usage = %ld\n", after.ru_maxrss);


        } else if (strncmp(argv[1], "-l", sizeof(char) * 2) == 0 && num > 0) {

            //Create
            getrusage(RUSAGE_SELF, &before);
            rootTable = DcreateHashTable(DEFAULTSIZE);
            getrusage(RUSAGE_SELF, &after);

            time_createD = calculate(&before, &after);

            if (rootTable == NULL) {
                printf("Hash Table could not be created :(\n");
                exit(EXIT_FAILURE);
            }

            //insert
            for (int i = 0; i < num; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Dinsert(rootTable, value);
                getrusage(RUSAGE_SELF, &after);


                time_insertD += calculate(&before, &after);

                if (!success) {
                    printf("Sorry there was an error inserting %ld in one of the hash tables :(\n", value);
                    continue;
                }




                //Rehash Dictionary Hashtable if LOAD_FACTOR is exceed.
                if (rootTable != NULL && (float) rootTable->capacityUsed / rootTable->size > LOAD_FACTOR) {
                    HashTable *biggerTable = NULL;
                    getrusage(RUSAGE_SELF, &before);
                    //rehash create a bigger table, rehash all data in the old table into the new table and delete the old table.
                    rootTable = rehash(biggerTable, rootTable);
                    getrusage(RUSAGE_SELF, &after);


                    time_rehash += calculate(&before, &after);
                    didRehash = true;
                }

            }
            floodCount(false); //reset counter

            //size
            capUsed = dSize(rootTable);


            //search
            for (long i = 0; i < num; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Dsearch(rootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_searchD += calculate(&before, &after);
                if (!success) {
                  //  printf("The number %ld is not in the hashTables\n", value);
                }
            }
            //destroy
            for (long i = 0; i < num; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Ddelete(rootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_deleteD += calculate(&before, &after);

                if (!success) {
                    //printf("The number %ld is not in the hashTables\n", value);
                }
            }
            //print to output file
            fprintf(foutput, "%12ld\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12ld\n", capUsed, time_insertD, time_rehash, time_searchD, time_deleteD, after.ru_maxrss);


            //printout
            printf("\n\n\nDICTIONARY  Hash-table of size %d created successfully :)\n", num);
            printf("\nTIME TO CREATE DICTIONARY HASHTABLE : %.4fs\n", time_createD);
            printf("\nTIME TO FLOOD DICTIONARY HASHTABLE : %.4fs\n", time_insertD);
            printf("\nTHE NUMBER OF DATA LOADED INTO HASHTABLE IS %ld\n",capUsed);
            didRehash == true ? printf("TIME TO REHASH THE DICTIONARY HASHTABLE : %.4fs\n", time_rehash)
                              : printf("\n");

			didRehash = false;
            printf("\nTIME TO SEARCH IN DICTIONARY HASHTABLE : %.4fs\n", time_searchD);
            printf("\nTIME TO DELETE FROM DICTIONARY HASHTABLE : %.4fs\n", time_deleteD);
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
        if (num < 1 || num2 < 1) {
            usage();
            return 6;
        } else if (strncmp(argv[1], "-s", sizeof(char) * 2) == 0 && strncmp(argv[3], "-l", sizeof(char) * 2) == 0) {
            //Create
            getrusage(RUSAGE_SELF, &before);
            rootTable = DcreateHashTable(num);
            getrusage(RUSAGE_SELF, &after);


            time_createD = calculate(&before, &after);
            if (rootTable == NULL) {
                printf("Hash Table could not be created :(\n");
                exit(EXIT_FAILURE);
            }

            //insert
            for (long i = 0; i < num2; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Dinsert(rootTable, value);
                getrusage(RUSAGE_SELF, &after);


                time_insertD += calculate(&before, &after);

                if (!success) {
                    printf("Sorry there was an error inserting %ld in one of the hash tables :(\n", value);
                    continue;
                }
                //Rehash Dictionary Hashtable if LOAD_FACTOR is exceed.
                if (rootTable != NULL && (float) rootTable->capacityUsed / rootTable->size > LOAD_FACTOR) {
                    HashTable *biggerTable = NULL;
                    getrusage(RUSAGE_SELF, &before);
                    //rehash create a bigger table, rehash all data in the old table into the new table and delete the old table.
                    rootTable = rehash(biggerTable, rootTable);
                    getrusage(RUSAGE_SELF, &after);


                    time_rehash += calculate(&before, &after);
                    didRehash = true;
                }
            }
            floodCount(false);


            //size
            capUsed = dSize(rootTable);

            //search
            for (long i = 0; i < num2; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Dsearch(rootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_searchD += calculate(&before, &after);

                if (!success) {
                   // printf("The number %ld is not in the hashTables\n", value);
                }
            }
            //destroy
            for (long i = 0; i < num2; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Ddelete(rootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_deleteD += calculate(&before, &after);

                if (!success) {
                  //  printf("The number %ld is not in the hashTables\n", value);
                }
            }

            //print to output file
            fprintf(foutput, "%12ld\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12ld\n", capUsed, time_insertD, time_rehash, time_searchD, time_deleteD, after.ru_maxrss);



            //printout
            printf("\n\n\nDICTIONARY  Hash-table of size %d created successfully :)\n", num);
            printf("\nTIME TO CREATE DICTIONARY HASHTABLE : %.4fs\n", time_createD);
            printf("\nTIME TO FLOOD DICTIONARY HASHTABLE : %.4fs\n", time_insertD);
            printf("\nTHE NUMBER OF DATA LOADED INTO HASHTABLE IS %ld\n",capUsed);
            didRehash == true ? printf("TIME TO REHASH THE DICTIONARY HASHTABLE : %.4fs\n", time_rehash)
                              : printf("\n");
			didRehash = false;
            printf("\nTIME TO SEARCH IN DICTIONARY HASHTABLE : %.4fs\n", time_searchD);
            printf("\nTIME TO DELETE FROM DICTIONARY HASHTABLE : %.4fs\n", time_deleteD);
            printf("Memory usage = %ld\n", after.ru_maxrss);


        } else if (strncmp(argv[1], "-s", sizeof(char) * 2) == 0 && strncmp(argv[3], "-f", sizeof(char) * 2) == 0) {

            //Create
            getrusage(RUSAGE_SELF, &before);
            rootTable = DcreateHashTable(num2);
            getrusage(RUSAGE_SELF, &after);


            time_createD = calculate(&before, &after);
            if (rootTable == NULL) {
                printf("Hash Table could not be created :(\n");
                exit(EXIT_FAILURE);
            }

            //insert
            FILE *inputFile = fopen(argv[4], "r");
            if(inputFile == NULL){
                printf("Error: File could not be open OR invalid file name :(\n");
                return 9;
            }
            while(!feof(filePointer)){
                //The values to flood are randomly generated.
                long value;
                fscanf(filePointer, "%ld", &value);
                getrusage(RUSAGE_SELF, &before);
                success = Dinsert(rootTable, value);
                getrusage(RUSAGE_SELF, &after);


                time_insertD += calculate(&before, &after);

                if (!success) {
                    printf("Sorry there was an error inserting %ld in one of the hash tables :(\n", value);
                    continue;
                }
                //Rehash Dictionary Hashtable if LOAD_FACTOR is exceed.
                if (rootTable != NULL && (float) rootTable->capacityUsed / rootTable->size > LOAD_FACTOR) {
                    HashTable *biggerTable = NULL;
                    getrusage(RUSAGE_SELF, &before);
                    //rehash create a bigger table, rehash all data in the old table into the new table and delete the old table.
                    rootTable = rehash(biggerTable, rootTable);
                    getrusage(RUSAGE_SELF, &after);


                    time_rehash += calculate(&before, &after);
                    didRehash = true;
                }
            }
            floodCount(false);


            //size
            capUsed = dSize(rootTable);


            //search
            for (long i = 0; i < num; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Dsearch(rootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_searchD += calculate(&before, &after);

                if (!success) {
                   // printf("The number %ld is not in the hashTables\n", value);
                }
            }
            //destroy
            for (long i = 0; i < num; i++) {
                //The values to flood are randomly generated.
                long value = floodCount(true);
                getrusage(RUSAGE_SELF, &before);
                success = Ddelete(rootTable, value);
                getrusage(RUSAGE_SELF, &after);

                time_deleteD += calculate(&before, &after);

                if (!success) {
                   // printf("The number %ld is not in the hashTables\n", value);
                }
            }
            //print to output file
            fprintf(foutput, "%12ld\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12.4f\t\t%12ld\n", capUsed, time_insertD, time_rehash, time_searchD, time_deleteD, after.ru_maxrss);



            //printout
            printf("\n\n\nDICTIONARY  Hash-table of size %d created successfully :)\n", num);
            printf("\nTIME TO CREATE DICTIONARY HASHTABLE : %.4fs\n", time_createD);
            printf("\nTIME TO FLOOD DICTIONARY HASHTABLE : %.4fs\n", time_insertD);
            printf("\nTHE NUMBER OF DATA LOADED INTO HASHTABLE IS %ld\n",capUsed);
            didRehash == true ? printf("TIME TO REHASH THE DICTIONARY HASHTABLE : %.4fs\n", time_rehash)
                              : printf("\n");
			didRehash = false;
            printf("\nTIME TO SEARCH IN DICTIONARY HASHTABLE : %.4fs\n", time_searchD);
            printf("\nTIME TO DELETE FROM DICTIONARY HASHTABLE : %.4fs\n", time_deleteD);
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
    //fclose(filePointer);
    fclose(foutput);
    return 0;
}
