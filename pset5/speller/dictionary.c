// Implements a dictionary's functionality

#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int word_count;

// Number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // get hash of word
    int index = hash(word);
    // assign cursor used to traverse linked list
    node *cursor = table[index];
    while (cursor)
    {
        // compare words in each linked list
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    int hash_sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        // SELF MADE HASH
        hash_sum += pow(tolower(word[i]), 2) + tolower(word[i]);
    }
    return hash_sum % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    // Open dictionary file
    FILE *dict_data = fopen(dictionary, "r");
    if (dict_data == NULL)
    {
        fprintf(stderr, "Failed to open dictionary.\n");
        return false;
    }
    char word[LENGTH + 1];
    int index;

    // read each line
    while (fscanf(dict_data, "%s", word) != EOF)
    {
        // hash and assign to linked list
        // create node for word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for node.\n");
            return false;
        }
        // copy word to node
        strcpy(n->word, word);
        // hash the word to get array index
        index = hash(word);
        // set pointers in correct order
        // first from new node to current first word in index
        n->next = table[index];
        // then set the index to point at the new node
        table[index] = n;
        // increase count for each word added
        word_count++;
    }
    fclose(dict_data);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    node *temp;
    node *cursor;
    // for each index in the array
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        // traverse until the end is reached (cursor becomes NULL)
        while(cursor)
        {
            temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
