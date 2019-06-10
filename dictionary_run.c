#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"


int dictionary_lookup_payload(unsigned char *payload, int *hit_num, int *word_num)
{

    return 0;
}

int main(int argc, char ** argv)
{
    char word[MAX_WORD_SIZE+1];
    char answer[MAX_DESC_SIZE+1];

    dictionary_initialise();

    int i;
    for (i = 1; i < argc; i++)
    {
        dictionary_read_from_file(argv[i]);
    }

    scanf("%s",word);

    while(word[0] != '.')
    {
        if (dictionary_lookup(word, strlen(word)))
        {
            printf("%s:\t%s\n", word, "Found!");
        } else {
            printf("%s:\t%s\n", word, "Not in dictionary");
        }
        scanf("%s",word);
    }
}
