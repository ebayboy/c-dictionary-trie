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

static char * textFileRead(char* filename, int *flen)
{
    char* text;
    FILE *pf = fopen(filename,"r");

    fseek(pf,0,SEEK_END);
    long lSize = ftell(pf);
    *flen = lSize;
    text=(char*)malloc(lSize+1);
    rewind(pf); 
    fread(text,sizeof(char),lSize,pf);
    text[lSize] = '\0';

    return text;
}

int main(int argc, char ** argv)
{
    char word[MAX_WORD_SIZE+1];
    char answer[MAX_DESC_SIZE+1];

    if (argc < 2) {
        fprintf(stderr, "usage: ./dictionary_run words_file dic_file\n");
        return -1;
    }

    char *dic_file = argv[2];
    char *words_file = argv[1];

    dictionary_initialise();
    dictionary_read_from_file(dic_file);

    int flen = 0;
    char *payload = textFileRead(argv[1], &flen);

    int hit_num = 0, word_num = 0;

    dictionary_lookup_payload(payload, flen, &hit_num, &word_num);

    printf("hit_num:[%d] word_num:[%d]\n", hit_num, word_num);

}

