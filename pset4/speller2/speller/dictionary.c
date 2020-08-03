// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

int words(node* trie);
void erase(node* trie);

// Represents a trie
node *root;
node* ptr;
int count1 = 0;

unsigned int h;

unsigned int hash(const char c)
{
    if (c == 39){
        h = 26;
    } else {
        h = tolower(c) - 'a';
    }
    return h;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    node *newRoot;
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        ptr = root;
        int length = strlen(word);
        int position = 0;
        
        for (int i = 0; i < length; i++){
            position = hash(word[i]);
            if (ptr->children[position] == NULL){
                newRoot = malloc(sizeof(node));
                newRoot-> is_word = false;
                for (int j = 0; j < N; j++)
                {
                    newRoot->children[j] = NULL;
                }
                ptr->children[position] = newRoot;
                ptr = ptr->children[position];
            } else {
                ptr = ptr->children[position];
            }
        }
        ptr-> is_word = true;
    }
    
    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words(root);
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    ptr = root;
    int length = strlen(word);
    for (int i = 0; i < length; i++){
        if (ptr->children[hash(word[i])]){
            ptr = ptr->children[hash(word[i])];
        }
        else {
            return false;
        }
    }
    if (ptr->is_word) {        
        return true;
    } else {
        return false;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    erase(root);
//    free(root);
    return true;
}



int words(node* trie)
{
    int count = 0;
    if (trie->is_word){
        count++;
    }
    
    for (int i = 0; i < N; i++){
        if (trie->children[i]){
            count += words(trie->children[i]);
        }
    }
    return count;
}

void erase(node* trie){
    for (int i = 0; i < N; i++){
        if(trie->children[i] != NULL){
            erase(trie->children[i]);
        }
    }
    free(trie);
    count1++;
    return;
}