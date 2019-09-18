/**
 * Declares a dictionary's functionality.
 */

//#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

//User defined types
typedef struct HashNode
{
	unsigned long key;
	int number;
	struct HashNode *next;
} HashNode;

typedef struct HashTable
{
	unsigned int size;
	unsigned int capacityUsed;
	HashNode **table;
} HashTable;

typedef struct boolOrAddress
{
	bool boolean;
	HashTable *tablePointer;
} boolOrAddress;

// maximum length for a word
#define LENGTH 45

// ratio of size of dictionary hash table to capacity used.
#define LOAD_FACTOR 0.95

#define ZERO 0
#define DOUBLE 2

/**
 * Return a pointer to a hashTable
 */

HashTable *DcreateHashTable(int size);

/**
 * Returns true if word is in dictionary else false.
 */

bool dCheck(HashTable *tablePointer, const char *word); //dCheck short for dictionary check

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool dLoad(HashTable *tablePointer, char *dictionary); //dLoad short for dictionary load

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int dSize(HashTable *tablePointer); //dSize short for dictionary size

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool dUnload(HashTable *tablePointer); //dUnload short for dictionary unload

/**
 * Returns key of insert position in hashTable.
 */
bool Dinsert(HashTable *tablePointer, int value);

/**
 * Returns true if value was found in Hashtable otherwise false
 */
bool Dsearch(HashTable *tablePointer, int value);

/**
 * Returns true if value was successfully deleted from hash table
 */
bool Ddelete(HashTable *tablePointer, int value);

/**
 *  Prints out value key pair present in hashtable
 */
void DprintTable(HashTable *tablePointer, FILE *fp);

/**
 * Returns true if saved to file: Creates a file and save key value pair as backup.
 */
bool DsaveHashTable(HashTable *tablePointer, const char *givenName);

/**
 * Search backup location in current directory. creates and load backup if found( true) otherwise it returns false
 */
bool DrestoreHashTable(HashTable *tablePointer, const char *givenName);

/**
 * Return true if HashTable was successfully destroyed
 */
bool DdestroyHashTable(HashTable *tablePointer);

/*
 * Return Hash table of double the size
 */
HashTable *rehash(HashTable *newTable, HashTable *oldTable);
//#endif // DICTIONARY_H
