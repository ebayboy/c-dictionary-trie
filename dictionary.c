/**
* @author Michael Bull
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

#define ALPHABET_SIZE 26

#define false 0
#define true 1

struct trie tree;

struct trie_node
{
    // *2 as we must account for a-z as well as A-Z
    struct trie_node *children[ALPHABET_SIZE * 2];
};

struct trie
{
    struct trie_node root;
};

/**
* Converts a character to an integer value based upon its position in
* the alphabet, or returns -1 if the character is not in the alphabet.
*/
int letter_to_int(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A' + 26;
    }
    else if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }
    return -1;
}

/**
* Prints and identifies an invalid character in a word
* based upon its index within the word.
*
* e.g.
*   word: "plat%eau"
*              ^
*/
void print_invalid_word(const char *word, int index)
{
    printf("  word: \"%s\"\n", word);
    printf("         ");
    int i;
    for (i = 0; i < index; i++)
    {
        printf(" ");
    }
    printf("^\n");
}

/**
* Inserts a word and its meaning into the trie.
*/
int trie_insert(struct trie_node *node, const char *word)
{
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        int letter = letter_to_int(word[i]);
        if (letter == -1)
        {
            // invalid character in the string, cannot be inserted into the trie
            printf("failed to insert due to invalid character in word\n");
            print_invalid_word(word, i);
            return false;
        }

        struct trie_node *parent = node;
        node = node->children[letter];

        if (!node)
        {
            node = malloc(sizeof(struct trie_node));
            parent->children[letter] = node;
        }
    }

    return true;
}

/**
* Traverses through a trie from the root node and searches
* for a word's description based on the individual letters
* that make up the word.
*/
int trie_get(struct trie_node *node, const char *word)
{
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        int letter = letter_to_int(word[i]);
        if (letter == -1)
        {
            return false;
        }

        node = node->children[letter];
        if (!node)
        {
            return false; // not found
        }
    }

    return true;
}

void dictionary_initialise()
{
    tree = (struct trie) {}; // zeroes the tree
}

int dictionary_read_from_file(const char * filename)
{
    // attempts to open the file in read only mode
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        printf("could not find/open file \"%s\"\n", filename);
        return false;
    }

    char word[MAX_WORD_SIZE];

    int count = 0;

    while (fscanf(file, "%s", word) > 0) {
        printf("word:[%s]\n", word);

        if (!trie_insert(&tree.root, word))
        {
            fclose(file);
            return false;
        }
        else
        {
            count++;
        }
    }

    fclose(file);
    printf("parsed file \"%s\" with %i entries\n", filename, count);
    return true;
}

int dictionary_lookup(const char *word)
{
    // check for invalid letters in input
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        int letter = letter_to_int(word[i]);
        if (letter == -1)
        {
            printf("invalid character in word\n");
            print_invalid_word(word, i);
            return false;
        }
    }

    return true;
}
