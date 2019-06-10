#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"


int dictionary_lookup_payload(unsigned char *payload, size_t plen, int *hit_num, int *word_num)
{
    char *pos = NULL, *pend = NULL;
    char *posw_s = NULL, *posw_e = NULL;
    int wlen;
    int i, j;

    pend = payload + plen - 1;
    pos = payload;

    printf("payload:[%.*s]\n", plen, payload);

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
                if (isspace(*(posw_s + j))) {
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
            printf("parse word:[%.*s]\n", wlen, posw_s);
            (*word_num)++;
            if (dictionary_lookup(posw_s, wlen)) {
                (*hit_num)++;
            }
        }

        pos = posw_e + 1;
    }

    return 0;
}


#if 0
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
#else

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

    int hit_num = 0, word_num = 0;
    char payload[1024] =  "this is a test demo x 1122 3322 x3223";

    dictionary_lookup_payload(payload, strlen(payload), &hit_num, &word_num);

    printf("hit_num:[%d] word_num:[%d]\n", hit_num, word_num);

}

#endif
