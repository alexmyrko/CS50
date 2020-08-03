// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Cannot\n");
        unload();
        return false;
    }
    
    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        strcpy (n->word, word);
        n->next = NULL;
        
        if (hashtable[hash(word)] != NULL){
            for (node *ptr = hashtable[hash(word)]; ptr != NULL; ptr = ptr->next){
                if (ptr->next == NULL){
                    ptr->next = n;
                    break;
                }
            }
        } else {
            hashtable[hash(word)] = n;
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    for (int i = 0; i < N; i++){
        for (node *ptr = hashtable[i]; ptr != NULL; ptr = ptr->next){
//            printf("%s\n", ptr->word);
            count++;
        }
    }
    return count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    bool same = false;
    for (node *ptr = hashtable[hash(word)]; ptr != NULL; ptr = ptr->next){
        same = true;
        for(int i = 0; i < strlen(word); i++){
            if (tolower(word[i]) != ptr->word[i])     
            { 
              same = false;
              break;
            }
        }
        if (same == true && strlen(ptr->word) == strlen(word) ){
            return true;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    node* tmp;
    node* ptr;
    for (int i = 0; i < N; i++){
        ptr = hashtable[i];
        while (ptr != NULL){
            tmp = ptr;
            ptr = ptr->next;
            free(tmp);
        }
    }
    return true;
}
