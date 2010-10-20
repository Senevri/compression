/* ex: set tabstop=2 expandtab softtabstop=2 shiftwidth=2:  */
#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <stdio.h>

#define MAX_WORD_LEN 64

/* Consistent array of two bits */ // I think this refers to rle 
int dict_encode(char * dest, char * src, int len);
int dict_decode(char * dest, char * src, int len);

/* File operations */
void compress_dict(FILE *infile, FILE *outfile);
void extract_dict(FILE *infile, FILE *outfile);

typedef struct keyword keyword;
/* Dictionary structs */
struct keyword{
  int count; /* count of how many instances of this word was found */
  int length; /* lenght of word */
  char * word;  /* word data */
  keyword * prev; /* pointer to the previous keyword */
  keyword * next; /* pointer to the next keyword */
}; 


/* struct for textblobs */
typedef struct {
  keyword * keyword;
  int huffbits; /*the huffman code*/
  char hufflen; /*how many bits are relevant in the code*/
  void * left; /*left child blob*/
  void * right; /* right child blob */
} blob;

#endif
