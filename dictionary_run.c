#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

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

