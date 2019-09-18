/**
 *Todo
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <bits/types/struct_rusage.h>
#include <sys/resource.h>
#include "logarithmic.h"
#include "dependencies.h"

struct rusage before, after;


rootHashTable *LcreateHashTable(rootHashTable *tablePointer, int size) {
    if (size < 1)
        return NULL;
    /*
    * We need to assign a global depth proportional  to the size of the hashTable
    */

    int initialGlobalDepth = ceil(log(size) / log(2));

    //for an efficient grow of the structure, it can have size of 2^numberofbits(size)
    size = myPow(2, initialGlobalDepth);

    if (tablePointer == NULL) {
        rootHashTable *hashtable = malloc(sizeof(rootHashTable));
        if (hashtable == NULL) { // check it malloc returned valid memory address
            printf("Hashtable could not be created :(\n");
            return NULL;
        }

        hashtable->globalDepth = initialGlobalDepth; //initializing the initial global the to the minimum number of bit of size
        hashtable->size = size;
        hashtable->capacityUsed = 0;
        hashtable->previousSize = size;
        hashtable->originalSize = size; // will never be updated

        hashtable->binPointerArray = malloc(sizeof(rootTableNode *) * size);
        if (hashtable->binPointerArray == NULL) {
            free(hashtable); // release the memory that was just allocated for the table.
            printf("Hashbin could not be created :( \n");
            return NULL;
        }

        for (int i = 0; i < size; i++) {
            hashtable->binPointerArray[i] = (rootTableNode *) calloc(sizeof(rootTableNode), 1);

        }
        return hashtable;
    } else {
        tablePointer->size = size;
        tablePointer->previousSize = size;
        tablePointer->globalDepth = initialGlobalDepth;
        tablePointer->binPointerArray = reallocarray(tablePointer->binPointerArray, sizeof(rootTableNode *), size);
        for (int i = 0; i < size; i++) {
            tablePointer->binPointerArray[i] = (rootTableNode *) calloc(sizeof(rootTableNode), 1);


        }
        return tablePointer;
    }
}

bool Linsert(rootHashTable *tablePointer, int value) {


    //File we check if hatable exits
    if (tablePointer == NULL) {
        printf("\nNo Hashtable exits, create one first...\n");
        return false;
    }
    /*
     * This is what we do:
     * -> hash a value and get its bin position.
     * :if bin is available(has available space) and localdepth of bin equal global depth insert in that bin.
     * :else if bin is available but local depth of bin is less than global depth of do bit shifting to remove |global - local depth| number of most
     *  significant bits from the hash key and insert value at new bin position if possible.
     * :else if bin is full, double size of hashtable, increase global depth size by one and update all pointers.
     */

//    if (Lsearch(tablePointer, value))
//    {
//        printf("\n%d is already in the Hashtable. Logarithmic Hashtable doesn't support duplicate data :)\n", value);
//        return true;
//    }

    //Hash value
    long key = intHash(value) % tablePointer->size;


    //create bin array if none exist
    if (tablePointer->binPointerArray[key] == NULL) {
        tablePointer->binPointerArray[key] = (rootTableNode *) malloc(sizeof(rootTableNode));
        // check if space was created
        if (tablePointer->binPointerArray[key] == NULL) {
            printf("\nSpace was not created for a new bin, malloc failed :(\n");
            return false;
        }
        //rootTableNode *binNode = tablePointer->binPointerArray[key];
        dataBin *newBin = calloc(sizeof(dataBin), 1);
        if (newBin == NULL) {
            printf("Error: a new newBin could not be created, malloc return NULL :(\n");
            return false;
        }

        newBin->arraysize = DATABINSIZE;
        newBin->localDepth = tablePointer->globalDepth;
        newBin->array = malloc(sizeof(int) * (newBin->arraysize));
        if (newBin->array == NULL) {
            printf("Error array in bin :(\n");
            return false;
        }
        newBin->array[newBin->insertPosition++] = value;
        tablePointer->binPointerArray[key]->binPointer = newBin;
        tablePointer->capacityUsed++;
        return true;
    }

    //insert into existing bin array onless it is full
    rootTableNode *binNode = tablePointer->binPointerArray[key];
    dataBin *newBin = NULL;
    if (binNode->binPointer == NULL) {
        newBin = calloc(sizeof(dataBin), 1);
        if (newBin == NULL) {
            printf("Error creating new bin :( \n");
            return false;
        }
        newBin->arraysize = DATABINSIZE;
        newBin->localDepth = tablePointer->globalDepth;
        //insertposition has been initialized to 0;
        binNode->binPointer = newBin;
    } else {
        newBin = binNode->binPointer;
    }

    if (newBin->insertPosition < newBin->arraysize) {
        if (newBin->insertPosition == 0) {
            newBin->localDepth = tablePointer->globalDepth;
        }
        if (newBin->array == NULL) {
            newBin->array = calloc(sizeof(int), newBin->arraysize);
            if (newBin->array == NULL) {
                printf("Error: new data array could not be created :(\n");
                return false;
            }

        }
        //we don't need to change the local depth
        newBin->array[newBin->insertPosition++] = value;
        binNode->binPointer = newBin;
        tablePointer->capacityUsed++;
        return true;
    }

    //split bin array, grow hashtable and repartition pointer if bin Array overflows
    if (newBin->insertPosition == newBin->arraysize) {

        /*
         * Let split bin into 2 if local depth is less that global depth or
         * Double Hash table size is local depth equal global depth.
         */
        if (newBin->localDepth == tablePointer->globalDepth) {
            //We need to double table size
            rootTableNode **backupAddress = tablePointer->binPointerArray;
            tablePointer->binPointerArray = reallocarray(tablePointer->binPointerArray, sizeof(rootTableNode *),
                                                         tablePointer->size * DOUBLE);
            //check if this memory was created
            if (tablePointer->binPointerArray == NULL) {
                printf("There was not enough memory to increase size of hashTable sorry :(\n");
                tablePointer->binPointerArray = backupAddress;
                printf("ERROR: MEMORY TO GROW STRUCTURE NOT AVAILABLE :( ...Insertion falsed\n");
                return false;
            }
            for (int i = tablePointer->size - 1; i < tablePointer->size * DOUBLE; i++) {
                tablePointer->binPointerArray[i] = NULL;
            }

            tablePointer->globalDepth++;
            tablePointer->previousSize = tablePointer->size;
            tablePointer->size *= DOUBLE;

        } else if (newBin->localDepth < tablePointer->globalDepth) {
            //We need to split the bin that cased the overflow
            int *array = getAllUpdateIndex(tablePointer->globalDepth, newBin->localDepth, key);


            int arraySize = array[0];
            for (int j = 1; j < arraySize; j++) {
                if (tablePointer->binPointerArray[array[j]] == binNode) {
                    tablePointer->binPointerArray[array[j]] = malloc(sizeof(rootTableNode));
                    if (tablePointer->binPointerArray[array[j]] == NULL) {
                        printf("\nSpace was not created for a new bin, malloc failed :(\n");
                        return false;
                    }
                    //rootTableNode *binNode = tablePointer->binPointerArray[array[j]];
                    dataBin *newBin = calloc(sizeof(dataBin), 1);
                    if (newBin == NULL) {
                        printf("Error: a new newBin could not be created, malloc return NULL :(\n");
                        return false;
                    }
                    newBin->arraysize = DATABINSIZE;
                    newBin->localDepth = tablePointer->globalDepth;
                    newBin->insertPosition = ZERO;
                    tablePointer->binPointerArray[array[j]]->binPointer = newBin;
                }
            }
            free(array);
        }

        //Now we need to redistribute the values of the bin that overflowed.
        int tmpArr[DATABINSIZE + 1];
        newBin->insertPosition = ZERO;

        for (int i = 0; i < DATABINSIZE; i++) {
            tmpArr[i] = newBin->array[i];
        }
        tmpArr[DATABINSIZE] = value;
//lets rehash the bin that case the overflow
        for (int i = 0; i <= DATABINSIZE; i++) {
            if (Linsert(tablePointer, tmpArr[i]) == false) {
                printf("Error: something went wrong redistributing the overflowed bin :(");
                return false;
            }
            tmpArr[i] = 0;
        }

        getrusage(RUSAGE_SELF, &before);
        updatePointers(tablePointer);
        getrusage(RUSAGE_SELF, &after);

        double updateTime = calculate(&before, &after);

        printf("Insertion complete,  pointer updated, time %f :)\n", updateTime);
        tablePointer->capacityUsed++;

        return true;
    }

    return false;
}

bool Lsearch(rootHashTable *tablePointer, int value) {
    if (tablePointer == NULL) {
        printf("Sorry, there is not table to search in. Create one first!");
        return false;
    }
    unsigned long key, key2, hash = intHash(value);
    int currentTableSize = tablePointer->size;
    bool doubleSearch = false;
    while (currentTableSize >= tablePointer->originalSize) {
        key = hash % currentTableSize;
        key2 = hash % (currentTableSize / DOUBLE);
        if (tablePointer->binPointerArray[key] == NULL) {
            key = key2;
            if (tablePointer->binPointerArray[key] == NULL) { continue; }
        }
        if (tablePointer->binPointerArray[key]->binPointer != NULL && tablePointer->binPointerArray[key]->binPointer->array != NULL) {
            if (tablePointer->binPointerArray[key2]->binPointer != NULL &&
                tablePointer->binPointerArray[key2]->binPointer->array != NULL) { doubleSearch = true; }
            for (int i = 0; i < tablePointer->binPointerArray[key]->binPointer->insertPosition; i++) {
                if (tablePointer->binPointerArray[key]->binPointer->array[i] == value || (doubleSearch && tablePointer->binPointerArray[key2]->binPointer->array[i] == value)) {
                    return true;
                }
                if(doubleSearch == true){// Do a double pointer increment
                    i++;
                }

            }
        }
        currentTableSize /= DOUBLE;
    }
    return false;
}


bool Ldelete(rootHashTable *tablePointer, int value) {
    if (tablePointer == NULL) {
        printf("Sorry there is no table to delete from. You have no problem to worry about :)\n");
        return false;
    }
    unsigned long key, key2, hash = intHash(value);
    bool doubleSearch = false;
    int currentTableSize = tablePointer->size;
    while (currentTableSize >= tablePointer->originalSize && currentTableSize > ZERO) {
        key = hash % currentTableSize;
        key2 = hash % (currentTableSize / DOUBLE);
        if (tablePointer->binPointerArray[key] != NULL && tablePointer->binPointerArray[key]->binPointer != NULL &&
            tablePointer->binPointerArray[key]->binPointer->array != NULL) {


            for (int i = 0; i < tablePointer->binPointerArray[key]->binPointer->insertPosition; i++, doubleSearch = false) {
                //activate double search
                if (tablePointer->binPointerArray[key2] != NULL && tablePointer->binPointerArray[key2]->binPointer != NULL && tablePointer->binPointerArray[key2]->binPointer->array != NULL) { doubleSearch = true; }

                if (tablePointer->binPointerArray[key]->binPointer->array[i] == value || (doubleSearch && tablePointer->binPointerArray[key2]->binPointer->array[i] == value)) {

                    if (doubleSearch && tablePointer->binPointerArray[key2]->binPointer->array[i] == value) { key = key2; }

                    //Move the last Item to the position to delete. therefore overwritting the item to be deleted. when we decrement the insert position. and the capacity used of the table.
                    int lastItem = --tablePointer->binPointerArray[key]->binPointer->insertPosition;

                    tablePointer->binPointerArray[key]->binPointer->array[i] = tablePointer->binPointerArray[key]->binPointer->array[lastItem];

                    tablePointer->capacityUsed--;
                    return true;

                }
            }
        }
        currentTableSize /= DOUBLE;
    }
    return false;

}

void LprintTable(rootHashTable *tablePointer, FILE *fp) {
    if (tablePointer == NULL) {
        printf("Sorry there is no table to print, Create one first :)\n");
        return;
    }
    for (int i = 0; i < tablePointer->size; i++) {
        if (fp == NULL) {
            printf("(LOGARITHMIC)Value(s) with key %d : ", i);
        } else {
            printf("(LOGARITHMIC)Value(s) with key %d : ", i);
            fprintf(fp, "\n(LOGARITHMIC)Value(s) with key %d : ", i);
        }
        if (tablePointer->binPointerArray[i] != NULL && tablePointer->binPointerArray[i]->binPointer != NULL) {
            for (int j = 0; j < tablePointer->binPointerArray[i]->binPointer->insertPosition; j++) {
                if (fp == NULL) {
                    printf("%d, ", tablePointer->binPointerArray[i]->binPointer->array[j]);
                } else {
                    printf("%d, ", tablePointer->binPointerArray[i]->binPointer->array[j]);
                    fprintf(fp, "%d, ", tablePointer->binPointerArray[i]->binPointer->array[j]);
                }
            }
        }
        if (fp == NULL)
            printf("\n");
        else {
            printf("\n");
            fprintf(fp, "\n");
        }
    }
}

bool LsaveHashTable(rootHashTable *tablePointer, const char *givenName) {
    //todo
    return false;
}

bool LdestroyHashTable(rootHashTable *tablePointer) {
    if (tablePointer == NULL) {
        printf("No HashTable to destroy...You have no problem to worry about :)\n");
        return true;
    }
    for (int i = 0; i < tablePointer->size; i++) {
        if (tablePointer->binPointerArray[i] == NULL) {
            continue;
        } else if (tablePointer->binPointerArray[i]->binPointer->array == NULL) {
            free(tablePointer->binPointerArray[i]->binPointer);
            free(tablePointer->binPointerArray[i]);
            tablePointer->binPointerArray[i] = NULL;

        } else if (tablePointer->binPointerArray[i]->binPointer->array != NULL) {
            free(tablePointer->binPointerArray[i]->binPointer->array);
            free(tablePointer->binPointerArray[i]->binPointer);
            free(tablePointer->binPointerArray[i]);
            tablePointer->binPointerArray[i] = NULL;
        }
    }

    free(tablePointer->binPointerArray);
    free(tablePointer);
    return true;
}

bool LrestoreHashTable(rootHashTable *tablePointer, const char *backupName) {
    //todo
    return false;
}

bool lCheck(rootHashTable *tablePointer, const char *word) {
    //todo
    return false;
}

bool lLoad(rootHashTable *tablePointer, const char *dictionary) {
    //todo
    return false;
}

unsigned int lSize(rootHashTable *tablePointer) {

    return tablePointer->capacityUsed;
}

bool lUnload(rootHashTable *tablePointer) {
    //todo
    return false;
}

int myPow(int base, int exp) {
    int result = 1;
    while (exp >= 1) {
        result *= base;
        exp--;
    }
    return result;
}

int binary_to_decimal(char *bitString) {
    int multiplier = 0;
    int i;
    int sum = 0;
    int length = strlen(bitString);
    for (i = length - 1; i >= 0; i--) {
        sum = sum + ((bitString[i] - '0') * myPow(2, multiplier));
        multiplier += 1;
    }
    return sum;
}

int *getAllUpdateIndex(int globalDepth, int localDepth, int index) {
//    printf("In getIndex\n");
    int global = globalDepth, local = localDepth,
            i = 0, arraySize = myPow(2, global - local) + 1;
    int *indexArray = (int *) malloc(sizeof(int) * arraySize);

    char *index2 = NULL, *stringBit = NULL;

    //the first index of indexArray is use to store the size of the array
    indexArray[0] = arraySize;
    while (global > local) {
        static bool firstPass = true;
        if (index % 2 == 1) {
            firstPass = false;
        }
        stringBit = decimal_to_binary(local++, index);
        int size = strlen(stringBit) + 2;
        index2 = malloc(sizeof(char) * size);
        if (index2 == NULL) {
            printf("Error allocating memory. :(\n");
            exit(EXIT_FAILURE);
        }
        strcpy(index2, "1");
        strcat(index2, stringBit);
        if (!firstPass)
            indexArray[++i] = binary_to_decimal(index2);


        if (global == local) {
            indexArray[0] = ++i;
        }
    }
    stringBit != NULL ? free(stringBit) : printf(" ");
    index2 != NULL ? free(index2) : printf(" ");
//    printf("Out of getIndex\n");
    return indexArray;
}

bool updatePointers(rootHashTable *tablePointer) {
    //check if table exits
    if (tablePointer == NULL) {
        printf("There is no table to update it's pointers. :(\n");
        return false;
    }


    for (int i = 0; i < tablePointer->size / DOUBLE; i++) {
        if (tablePointer->binPointerArray[i] == NULL) {
            continue;
        } else {
            if (tablePointer->binPointerArray[i]->binPointer == NULL) {
                continue;
            } else if (tablePointer->binPointerArray[i]->binPointer->localDepth < tablePointer->globalDepth) {
                int depthDifference =
                        tablePointer->globalDepth - tablePointer->binPointerArray[i]->binPointer->localDepth;

                while (depthDifference > 0) {
                    int local = tablePointer->binPointerArray[i]->binPointer->localDepth;

                    int *upadateIndices = getAllUpdateIndex(tablePointer->globalDepth, local, i);
                    int arraySize = upadateIndices[0];

                    for (int j = 1; j < arraySize; j++) {

                        tablePointer->binPointerArray[upadateIndices[j]] = tablePointer->binPointerArray[i];
                    }
                    depthDifference--;
                    free(upadateIndices);
                }
            }
        }
    }
    return true;
}


/*********************************************************************
 * ALL FUNCTION BELOW HAVE BEEN BORROW FROM SOURCES SITED.
 *
 *********************************************************************/

/*
 * Conversion form int to binary strings borrowed and customized from link below.
 * https://www.programmingsimplified.com/c/source-code/c-program-convert-decimal-to-binary
 */
char *decimal_to_binary(int significantFigure, int decimal) {
    int c, d, count;
    char *pointer;

    count = 0;
    pointer = (char *) malloc(significantFigure + 1);

    if (pointer == NULL)
        exit(EXIT_FAILURE);

    for (c = significantFigure - 1; c >= 0; c--) {
        d = decimal >> c;

        if (d & 1)
            *(pointer + count) = 1 + '0';
        else
            *(pointer + count) = 0 + '0';

        count++;
    }
    *(pointer + count) = '\0';

    return pointer;
}

/*HASH FUNCTION FORM MURMURHASH*/
unsigned long intHash(int x) {
    //    x = ((x >> 16) ^ x) * 0x45d9f3b;
    //    x = ((x >> 16) ^ x) * 0x45d9f3b;
    return x * 2147483647;
}
