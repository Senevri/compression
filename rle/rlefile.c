#include <stdio.h>
#include <string.h>
#include <stdlib.h> //for malloc
#include "rle.h"

//void compress(FILE *, FILE*);
//void extract(FILE *, FILE*);
//void printBuffer(char *buf, int len);

void compress_rle_ec(FILE *infile, FILE* outfile){
    char *buffer = malloc(1024*sizeof(char));
    char *encbuffer = malloc(10240*sizeof(char)); 
    int len, loc, i, changed=0;
    char singlechar=0;
    size_t read_len;

    while(!feof(infile)){
      read_len = fread(buffer, sizeof(char), 1024, infile);
      /* encode rules:
	 if last two characters are the same, all is OK.
	 else, we do not know if the follow-up character
	 is also the same.

	 else, the character just after our current buffer
	 might also be the same, which would be bad. So, 
	 we go back for one character in those cases, and 
	 do the thingy with the thingy.
      */

      if(buffer[read_len-2]==buffer[read_len-1]){
	//printBuffer(buffer, len-1);
	changed=0;
	goto encode;
      } else{
	changed=1;
	}
      /* encode */
    encode:
      loc = rle_encode_ec(encbuffer, buffer, read_len-changed);

      fwrite(encbuffer, sizeof(char), loc, outfile);
      
      buffer[0]=buffer[read_len-changed];
    }
    free(buffer);
    free(encbuffer);
}

void extract_rle_ec(FILE *infile, FILE* outfile){
  char singlechar=0;
  int loc, len, i;
  int changed=0;
  char *buffer = (char*)malloc(1024);
  char *encbuffer = (char*)malloc(10240); 
  size_t read_len;

  while(!feof(infile)){
    len=changed;
    fseek(infile, -changed, SEEK_CUR);
    read_len = fread(buffer, sizeof(char), 1024, infile);
    /* make sure we're not splitting an 'lln' case apart. */
    
    /* First of all, if the last three characters in the buffer 
       are 'lln', everything's kosher.
    */
    if(buffer[read_len-2]==buffer[read_len-3]) {
      changed=0;
      goto decode;
    } 
    
    /*if our last two characters are 'll'
      we need to return back by two bytes, then copy 
      those two to the beginning of buffer after write,
      and start inserting subsequent reads after those 2 bytes. 
    */
      if(buffer[read_len-1]==buffer[read_len-2]){
      changed=2;
      goto decode;
      }
      
    /* if singlechar is not EOF, 
       return back for one bit, to avoid case 
       where we're read in the first l - after decode, copy
       the last byte as first, and start writing from len=1       
    */ 
    changed=1;
    
    /* Decode needs a non-zerobased number-of-bytes as a parameter */
  decode:
    loc = rle_decode_ec(encbuffer, buffer, read_len-changed);
    //if (changed==2) printBuffer(buffer, len-1);
    for(i=0;i!=changed;i++){
      buffer[i]=buffer[(read_len+i)-changed];
    }
    fwrite(encbuffer, sizeof(char), loc, outfile);

  }
  free(buffer);
  free(encbuffer);
  
}

