#ifndef RLE_H
#define RLE_H
#include <stdio.h> //for FILE
/* Consistent array of two bytes */
int rle_encode(unsigned char * dest, unsigned char * src, int len);
int rle_decode(unsigned char * dest, unsigned char * src, int len);

/* With escape character */
int rle_encode_ec(unsigned char * dest, unsigned char * src, int len);
int rle_decode_ec(unsigned char * dest, unsigned char * src, int len);

/* File operations */
void compress_rle_ec(FILE *infile, FILE *outfile);
void extract_rle_ec(FILE *infile, FILE *outfile);
#endif
