/**
 * Declares a logarithmic's functionality.
 */

#ifndef LOGARITHMIC_H
#define LOGARITHMIC_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define DOUBLE 2

//test case. THIS SHOULD BE A POINTER WITH NOT FIXED SIZE IN THE FUTURE
#define DATABINSIZE 25

#define ZERO 0


typedef struct dataBin
{
    int localDepth;
    int insertPosition;
    int arraysize;
    int *array;
} dataBin;

typedef struct rootTableNode
{
    dataBin *binPointer;
} rootTableNode;

typedef struct rootHashTable
{
    int size;
    unsigned int capacityUsed;
    int originalSize;
    int previousSize;
    int globalDepth;
    rootTableNode **binPointerArray;
} rootHashTable;

/**
 * Returns a deterministic hash value for every value passed.
 */
unsigned long intHash(int valueToHash);

/*
 * Return a string of 1's and 0's representing the binary conversion of a decimal to binary
 */
char *decimal_to_binary(int numberOfSignificantFigures, int decimal);

/*
 * Returns binary representing the n least significant bit of a string of 0's and 1's
 */
//int significantBit(char *bitString , int globalDepth);

/*
 * Returns decimal equivalent of a string of bits
 */
int binary_to_decimal(char *bitString);

/**
 * Returns address if hashtable is created else false.
 */
rootHashTable *LcreateHashTable(rootHashTable *tablePointer, int size);

/*
 * Returns true if all pointers in hashtable were updated.
 */
bool updatePointers(rootHashTable *tablePointer);

/**
 * Returns true if value was successfully inserted  else false
 */
bool Linsert(rootHashTable *tablePointer, int value);

/**
 * Returns true if value was found in hashTable else false
 */
bool Lsearch(rootHashTable *tablePointer, int value);

/**
 * Returns true if value was successfully deleted else false
 */
bool Ldelete(rootHashTable *tablePointer, int value);

/**
 * Returns True if Hashtable was successfully saved to file else false
 */
bool LsaveHashTable(rootHashTable *tablePointer, const char *givenName);

/**
 * Returns true if Hashtable was successfully loaded from file else false
 */
bool LrestoreHashTable(rootHashTable *tablePointer, const char *giveName);

/**
 * Prints out value in hashTable
 */
void LprintTable(rootHashTable *tablePointer, FILE *fp);

/**
 * Returns true if hash Table is deleted from memory else false
 */
bool LdestroyHashTable(rootHashTable *tablePointer);

/**
 * Returns true if word is in dictionary else false.
 */
bool lCheck(rootHashTable *tablePointer, const char *word); //LCheck  is short Logarithmic check

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool lLoad(rootHashTable *tablePointer, const char *dictionary); //lLoad is short Logarithmic load

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int lSize(rootHashTable *tablePointer); //lSize is short for Logarithmic size

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool lUnload(rootHashTable *tablePointer); //lUnload is short for logarithmic unload
/*
 * Return number equivalent to a given base raised to a given power
 */
int myPow(int base, int exp);

/*
 * Return an array of
 */
int *getAllUpdateIndex(int globalDepth, int localDepth, int index);
/**
 * Returns number of seconds between b and a.
 */
#endif // LOGARITHMIC_H
