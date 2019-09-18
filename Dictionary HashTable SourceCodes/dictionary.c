/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <asm/errno.h>
#include <errno.h>
#include "dictionary.h"

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define PATHNAMEMAXLENGHT 100

bool del(HashTable *tablePointer, HashNode *Headlink);
unsigned long hash(unsigned char *str);
unsigned int hashInt(unsigned int x);

HashTable *DcreateHashTable(int size)
{
    if (size < 1)
        return NULL;
    HashTable *hashtable = malloc(sizeof(HashTable));
    //printf("DEBUG: sizeof Hashtable struct is %d\n", sizeof(hashtable));
    if (hashtable == NULL)
    {
        printf("\nERROR:(MALLOC) HashTable of size %d could not be created\n", size);
        return NULL;
    }
    else
    {
        hashtable->size = size;
        hashtable->capacityUsed = 0;
        hashtable->table = (HashNode **)malloc(sizeof(HashNode **) * size);
        if (hashtable->table == NULL)
        {
            printf("HashTable->table was not successfully created :(\n");
            return NULL;
        }
        for (int i = 0; i < size; i++)
        {
            hashtable->table[i] = NULL;
        }
    }
    return hashtable;
}

bool tableExist(HashTable *n)
{
    if (n != NULL)
        return true;
    return false;
}

bool Dinsert(HashTable *tablePointer, int value)
{
    if (tablePointer == NULL)
    {
        printf("Hash Table doesn't exit, Create one first :(\n");
        return false;
    }

    if (!Dsearch(tablePointer, value))
    {

        unsigned key = hashInt((unsigned)value) % tablePointer->size;

        if (tablePointer->table[key] == NULL)
        {
            tablePointer->table[key] = malloc(sizeof(HashNode));
            if (tablePointer->table[key] == NULL)
            {
                printf("Memory was not allocated :(\n");
                return false;
            }

            tablePointer->table[key]->number = value;
            tablePointer->table[key]->key = key;
            tablePointer->table[key]->next = NULL;
            tablePointer->capacityUsed++;

            return true;
        }
        else
        {
            HashNode *newbin = malloc(sizeof(HashNode));
            newbin->number = value;
            newbin->key = key;
            newbin->next = tablePointer->table[key]->next;
            tablePointer->table[key] = newbin;
            tablePointer->capacityUsed++;
            return true;
        }
    }
    else
    {
        printf("%d is already in the HashTable. Dictionary Hashtable doesn't support duplicate data :(", value);
        return true;
    }
}

bool Dsearch(HashTable *tablePointer, int value)
{
    if (tablePointer == NULL)
    {
        printf("Hash table doesn't exit, Please create one first :(\n");
        return false;
    }
    unsigned key = hashInt((unsigned)value) % tablePointer->size;
    HashNode *bin = tablePointer->table[key];
    if (bin != NULL)
    {
        if (bin->number == value)
            return true;
        else
        {
            while (bin->next != NULL)
            {
                bin = bin->next;
                if (bin->number == value)
                    return true;
            }
        }
        return false;
    }

    return false;
}

bool Ddelete(HashTable *tablePointer, int value)
{
    if (tablePointer == NULL)
    {
        printf("Hash table doesn't exit, Please create one first :(\n");
        return false;
    }
    unsigned key = hashInt(value) % tablePointer->size;
    HashNode *bin = tablePointer->table[key];
    if (bin != NULL)
    {
        if (bin->number == value)
        { //deleting at the head
            HashNode *newTail = bin->next;
            free(bin);
            tablePointer->table[key] = newTail;
            printf("Value was succesfully deleted :)\n");
            tablePointer->capacityUsed--;
            return true;
        }
        else
            while (bin->number != value && bin->next != NULL)
            { //deleting at any other position
                HashNode *tmpTail, *tmpHead = bin;
                bin = bin->next;
                tmpTail = bin->next;
                if (bin->number == value)
                {
                    free(bin);
                    tmpHead->next = tmpTail;
                    printf("Value was succesfully deleted :)\n");
                    tablePointer->capacityUsed--;
                    return true;
                }
            }
    }
   // printf("Value is not in Hash table:(\n");
    return false;
}

void DprintTable(HashTable *tablePointer, FILE *fp)
{
    if (tablePointer == NULL)
    {
        printf("No Hash table exit :( Restore or create one first\n");
        return;
    }
    int size = tablePointer->size;
    for (int i = 0; i < size; i++)
    {
        HashNode *bin = tablePointer->table[i];
        if (fp == NULL)
        {
            printf("(DICTIONARY)Value(s) with key %d : ", i);
        }
        else
        {
            printf("(DICTIONARY)Value(s) with key %d : ", i);
            fprintf(fp, "\n(DICTIONARY)Value(s) with key %d : ", i);
        }
        if (bin == NULL)
        {
            if (fp == NULL)
            {
                printf("None\n");
            }
            else
            {
                printf("None\n");
                fprintf(fp, "None\n");
            }
            continue;
        }
        while (bin != NULL)
        {
            if (fp == NULL)
            {
                printf("%d, ", bin->number);
            }
            else
            {
                printf("%d, ", bin->number);
                fprintf(fp, "%d, ", bin->number);
            }
            bin = bin->next;
        }
        if (fp == NULL)
            printf("\n");
        else
        {
            printf("\n");
            fprintf(fp, "\n");
        }
    }
    if (fp == NULL)
        printf("\n");
    else
    {
        printf("\n");
        fprintf(fp, "\n");
    }
}

bool DsaveHashTable(HashTable *tablePointer, const char *givenName)
{
    if (tablePointer == NULL)
    {
        printf("There is no Hash table to save:(\n");
        return false;
    }
    DIR *dir = opendir("./HashtableBackupDir");
    if (ENOENT == errno)
    {
        if (mkdir("./HashtableBackupDir/", 0777))
        {
            printf("HashtableBackupDir created successfully! \n");
        }
    }
    else
        closedir(dir);
    char pathName[PATHNAMEMAXLENGHT];
    strcpy(pathName, "./HashtableBackupDir/");
    strcat(pathName, givenName);
    strcat(pathName, ".txt");
    FILE *fp = fopen(pathName, "wt");
    int size = tablePointer->size;

    for (int i = 0; i < size; i++)
    {
        HashNode *bin = tablePointer->table[i];
        while (bin != NULL)
        {
            fprintf(fp, "%lu\t\t%d\r", bin->key, bin->number);
            bin = bin->next;
        }
    }
    fclose(fp);
    return true;
}

bool DdestroyHashTable(HashTable *tablePointer)
{
    if (tablePointer == NULL)
    {
        printf("Hash table doesn't exits :(\n");
        return false;
    }
    int size = tablePointer->size;
    for (int i = 0; i < size; i++)
    {
//        HashNode *bin = tablePointer->table[i];
        if (tablePointer->table[i] == NULL)
        {
            continue;
        }
        HashNode *nextDel, *nodeToDel = tablePointer->table[i];
        while (nodeToDel != NULL)
        {
            nextDel = nodeToDel->next;
            free(nodeToDel);
            nodeToDel = nextDel;
        }
    }
    free(tablePointer->table);
    free(tablePointer);
    return true;
}

bool DrestoreHashTable(HashTable *tablePointer, const char *backupName)
{
    //todo
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int dSize(HashTable *tablePointer)
{
    if (tableExist(tablePointer))
        return tablePointer->capacityUsed;
    printf("\nThere exit no created hash table\n");
    return -1;
} //dSize short for dictionary size

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool dUnload(HashTable *tablePointer)
{
    HashTable *tmp = tablePointer;
    if (tmp == NULL)
        return false;
    int n = tmp->size;
    for (int i = 0; i < n; i++)
    {
        if (tmp->table[i] != NULL)
        {
            if (!del(tablePointer, tmp->table[i]))
                return false;
        };
    }
    free(tablePointer);
    return true;
}; //dUnload short for dictionary unload

bool del(HashTable *tablePointer, HashNode *Headlink)
{
    HashNode *tmp1, *tmp = Headlink;
    while (tmp->next != NULL)
    {
        tmp1 = tmp;
        tmp = tmp->next;
        free(tmp1);
        tablePointer->capacityUsed--;
    }
    free(tmp);
    tablePointer->capacityUsed--;
    return true;
}

HashTable *rehash(HashTable *biggerTable, HashTable *oldTable)
{
    biggerTable = malloc(sizeof(HashTable));
    if (biggerTable == NULL)
    {
        printf("\nERROR:(MALLOC) A bigger HashTable of size %d could not be created\n", oldTable->size * DOUBLE);
        return false;
    }
    biggerTable->size = oldTable->size * DOUBLE;
    biggerTable->capacityUsed = ZERO;

    biggerTable->table = calloc(sizeof(HashNode *) , oldTable->size * DOUBLE);
    if (biggerTable->table == NULL)
    {
        printf("HashTable->table size could not be double successfully. Rehashing failed :(\n");
        return false;
    }
//    for (int i = 0; i < oldTable->size * DOUBLE; i++)
//    {
//        biggerTable->table[i] = NULL;
//    }

    //let rehash first table into biggerTable
    for (int i = 0; i < oldTable->size; i++)
    {
        if (oldTable->table[i] == NULL)
        {
            continue;
        }
        else if (oldTable->table[i] != NULL)
        {
            while (oldTable->table[i] != NULL)
            {
                Dinsert(biggerTable, oldTable->table[i]->number);
                oldTable->table[i] = oldTable->table[i]->next;
            }
        }
    }
    DdestroyHashTable(oldTable);
    return biggerTable;
}

///dSize short for dictionary size


/*********************************************************************
 * ALL FUNCTION BELOW HAVE BEEN BORROW FROM SOURCES SITED.
 *
 *********************************************************************/

/*HASH FUNCTION FORM MURMURHASH*/
unsigned int hashInt(unsigned int x)
{

    //x = ((x >> 16) ^ x) * 0x45d9f3b;
    //x = ((x >> 16) ^ x) * 0x45d9f3b;
    //x = (x >> 16) ^ x;
    return x * 2147483647; ///multiplication with any big prime
}

/*HASH FUNCTION FROM djb2; Credit to DAN BERNSTEIN*/

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c = *str++;

    while (c)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c = *str++;
    }
    return hash;
}

#endif // DICTIONARY_H
