
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#define MAX_WORD_SIZE   40
#define MAX_DESC_SIZE  200

void dictionary_initialise();

int dictionary_read_from_file(const char * filename);

int dictionary_lookup(const char *word, int wlen);

int dictionary_lookup_payload(unsigned char *payload, size_t plen, int *hit_num, int *word_num);

#endif
