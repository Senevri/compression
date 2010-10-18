#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <stdio.h>

#define MAX_WORD_LEN 200

/* Consistent array of two bits */ // I think this refers to rle 
int dict_encode(char * dest, char * src, int len);
int dict_decode(char * dest, char * src, int len);

/* File operations */
void compress_dict(FILE *infile, FILE *outfile);
void extract_dict(FILE *infile, FILE *outfile);

/* Dictionary structs */
typedef struct {
  int count; /* count of how many instances of this word was found */
  int length; /* lenght of word */
  char * word;  /* word data */
  void * next; /* pointer to the next keyword */
} keyword;

#endif
