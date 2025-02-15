
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
    char *value;

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
* Inserts a word and its meaning into the trie.
*/
int trie_insert(struct trie_node *node, const char *word, char *description)
{
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        int letter = letter_to_int(word[i]);
        if (letter == -1)
        {
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

    /*
    * this will prove very efficient we are only allocating enough memory
    * for every character in the description, which will save a lot of space
    * that would otherwise be allocated to the full MAX_DESC_SIZE
    */
    int len = strlen(description);
    node->value = malloc(len + 1);
    strncpy(node->value, description, len);
    return true;
}

/**
* Traverses through a trie from the root node and searches
* for a word's description based on the individual letters
* that make up the word.
*/
char *trie_get(struct trie_node *node, const char *word, int wlen)
{
    int i;
    for (i = 0; i < wlen; i++)
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
    return node->value;
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
        return false;
    }

    char word[MAX_WORD_SIZE];
    char desc[MAX_DESC_SIZE];

    int count = 0;

    // ensure that at least two items are being parsed (word & desc)
    while (fscanf(file, "%s", word) > 0)
    {
        memset(desc, 0, sizeof(desc));
        memcpy(desc, word, strlen(word));
        if (!trie_insert(&tree.root, word, desc))
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
    return true;
}

int dictionary_lookup(const char *word, int wlen)
{
    // check for invalid letters in input
    int i;
    for (i = 0; i < wlen; i++)
    {
        int letter = letter_to_int(word[i]);
        if (letter == -1)
        {
            return false;
        }
    }

    // grab the description from the tree
    char *description = trie_get(&tree.root, word, wlen);

    if (!description)
    {
        return false;
    }

    return true;
}

int dictionary_lookup_payload(unsigned char *payload, size_t plen, int *hit_num, int *word_num)
{
    char *pos = NULL, *pend = NULL;
    char *posw_s = NULL, *posw_e = NULL;
    int wlen;
    int i, j;

    pend = payload + plen - 1;
    pos = payload;

    while(pos < pend) {
        posw_s = posw_e = NULL;

        if (posw_s == NULL) {
            /* find start */
            i = 0;
            while(pos + i < pend) {
                if (isspace(*(pos + i))) {
                    i++;
                    continue;
                }
                posw_s = pos + i;
                break;
            }
        }

        if (posw_s == NULL) {
            break;
        }

        if (posw_e == NULL) {
            /* find start */
            j = 1;
            while(posw_s + j < pend) {
                if (isspace(*(posw_s + j)) || *(posw_s + j) == ',' || *(posw_s + j) == '.') {
                    posw_e = posw_s + j;
                    break;
                }
                j++;
            }
        }

        if (posw_e == NULL) {
            posw_e = pend;
        }

        wlen = posw_e - posw_s;
        if (wlen == 0) {
            wlen = 1;
        }
        
        if (posw_s && wlen > 0) {
            int hit = 0;
            (*word_num)++;
            if ((hit = dictionary_lookup(posw_s, wlen))) {
                (*hit_num)++;
            }
        }

        pos = posw_e + 1;
    }

    return 0;
}


