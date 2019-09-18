//
// Created by fonkam on 6/3/19.
//
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

short actionList()
{
    system("clear"); // let clear the terminal each time will call this
    short choice;
    printf("\n\t\t\t\b\b#############################################################\n\
			*&&&&&&&&&&&&&&&&&--CHOSE AN OPERATION TO PERFORM--&&&&&&&&\n \
			* \n\
			* 0  -Create HashTable. \n\
			* 1  -Insert value into HashTable. \n\
			* 2  -Search if value exist in hashTable \n\
			* 3  -Delete value from hashTable\n\
			* 4  -Print value all hashTable\n\
			* 5  -Flood table with big data\n\
			* 6  -Save hashTalbe to file\n\
			* 7  -Load saved hashTable from file(backup folder)\n\
			* 8  -Destroy hashTable\n\
            * 9  -Show number of data in Hash-Table\n\
			*    \
			*                    \n\
			* 10 -Mamaul/Documentation\n\
			* \n\
			* -1  -to terminate the program.\n\
            \t\t\b\b########################################################### \n\
			\nChoice: ");
    scanf("%hi", &choice);
    if (choice == -1)
    {
        printf("\n\nProgram terminated by user. Bye!");
        exit(EXIT_FAILURE);
    }
    else if (choice == 0)
    {
        printf("Creating a Hash Table...\n");
    }
    else if (choice == 1)
    {
        printf("OPTION: insert value into hashtable selected\n");
    }
    else if (choice == 2)
    {
        printf("OPTION: check if value exits selected\n");
    }
    else if (choice == 3)
    {
        printf("OPTION: delete a value selected\n");
    }
    else if (choice == 4)
    {
        printf("OPTION: print values in a hashtable selected\n");
    }
    else if (choice == 5)
    {
        printf("Flooding Tables with random values...\n");
    }
    else if (choice == 6)
    {
        printf("Your Hash table data is being saved...\n");
    }
    else if (choice == 7)
    {
        printf("Checking Hash tables in ./HashTableBackups directory...\n");
    }
    else if (choice == 8)
    {
        printf("Destroying Hash tables...\n");
    }
    else if (choice == 9)
    {
        printf("Printing number of entries in hash table...\n");
    }
    else if(choice == 10){
        printf("Printing manual...");
    }
    else if (choice < -1 || choice > 10)
    {
        printf("\n\nInvalid choise. Try again...");
        sleep(1);
        return actionList();
    }
    return choice;
}

/**
 * Returns number of seconds between b and a.
 */
double calculate(const struct rusage *b, const struct rusage *a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec))) /
                1000000.0);
    }
}


/*
 * Prints usage of log_hashTable driver program
 */
void usage(){
    printf("LOG_HASHTABLE(1) \t\t\tUser Commands\t\t\t\t\n\n"
           "NAME\n"
           "\t\t./log_hashTable\t-create a dynamic hash table and load data into it\n"
           "SYNOPSIS\n"
           "\t\t ./log_hashTable\n"
           "\t\t ./log_hashTable\t-s TABLE_SIZE\n"
           "\t\t ./log_hashTable\t-l INPUT_SIZE\n"
           "\t\t ./log_hashTable\t-s TABLE_SIZE -l INPUT_SIZE\n"
           "DESCRIPTION\n"
           "\t\tCreate Hash Table of initial size TABLE_SIZE and loads INPUT_SIZE number of data(integers)\n\n"
           "\t\t-s\n"
           "\t\t Flag to specify size of hash Table. If not provided DEFAULT SIZE is 10000\n"
           "\t\t-l\n"
           "\t\t Flag to specify size of input to be loaded. If not provided DEFAULT SIZE is 1000000\n"
           "\n"
           "AUTHOR\n"
           "\t\tWritten by FONKAM loic\n\n");
}

/*
 * Return int from 0  to INT_MAX and resets if arg1 is false
 */
long floodCount(bool reset){
    static long count = 0;
    if (reset){
        return count++;
    }else
    {
        count = 0;
        return count;
    }
}
