// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;
const int ASCII_ORIGIN = 97;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *bucket = table[hash(word)];
    if (bucket == NULL)
    {
        return false;
    }

    while (bucket != NULL)
    {
        if (strcasecmp(bucket->word, word) == 0)
        {
            return true;
        }
        bucket = bucket->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    return tolower(word[0]) - ASCII_ORIGIN;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char ch;
    char new_word[LENGTH + 1];
    int i = 0;

    while (fread(&ch, 1, 1, file) > 0)
    {
        if (isalpha(ch) || ch == '\'')
        {
            new_word[i] = ch;
            i++;
        }
        else if (isspace(ch) || i > LENGTH)
        {
            node *new_node = malloc(sizeof(node));
            new_word[i] = '\0';
            for (int j = 0; j < i; j++)
            {
                new_node->word[i] = new_word[i];
            }
            node *bucket = table[hash(new_word)];
            if (bucket == NULL)
            {
                table[hash(new_word)] = new_node;
            }
            else
            {
                while (bucket->next != NULL)
                {
                    bucket = bucket->next;
                }
                bucket->next = new_node;
            }

            i = 0;
        }
    }


    free(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int count = 0;
    for (int i = 0; i < N; i++)
    {
        node *cur = table[i];
        while (cur != NULL)
        {
            count++;
            cur = cur->next;
        }
    }

    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    if (size() == 0)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        node *cur = table[i];
        node *next = cur->next;
        while (cur != NULL)
        {
            free(cur->word);
            free(cur);
            cur = next;
            next = cur->next;
        }
    }

    return true;
}
