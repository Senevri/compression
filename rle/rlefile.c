#include <stdio.h>
#include <string.h>
#include <stdlib.h> //for malloc
#include "rle.h"

//void compress(FILE *, FILE*);
//void extract(FILE *, FILE*);
//void printBuffer(char *buf, int len);

void compress_rle_ec(FILE *infile, FILE* outfile){
    char *buffer = (char*)malloc(1024);
    char *encbuffer = (char*)malloc(10240); 
    int len, loc, i, changed=0;
    char singlechar=0;

    while(singlechar != EOF){
      len=changed; /* remeber to initialize vars! */

      while(len<1024){
	singlechar = fgetc(infile);
	if(singlechar==EOF){
	  len++;
	  break;
	}
	buffer[len]=singlechar;
	len++;
      }
      /* encode rules:
	 if last two characters are the same, all is OK.
	 else, we do not know if the follow-up character
	 is also the same.

	 else, the character just after our current buffer
	 might also be the same, which would be bad. So, 
	 we go back for one character in those cases, and 
	 do the thingy with the thingy.
      */

      if(buffer[len-2]==buffer[len-1]){
	//printBuffer(buffer, len-1);
	changed=0;
	goto encode;
      } else{
	changed=1;
	}
      /* encode */
    encode:
      loc = rle_encode_ec(encbuffer, buffer, len-changed);

      for(i=0;i<loc;i++){
	fputc(encbuffer[i], outfile);
      }
      
/*
 * 	fwrite() is all fun and games 'til you realize it overwrites,
 * 	doesn't append...
	may be an issue with passed pointer, though


      fwrite(encbuffer, 1, sizeof(encbuffer), outfile);
*/      buffer[0]=buffer[len-changed];
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

  while(singlechar != EOF){
    len=changed;
    while(len<1024){
      singlechar = fgetc(infile);
      if(singlechar==EOF) {
	len++;
	break;
      }
      buffer[len]=singlechar;
      len++;
    }
    
    /* make sure we're not splitting an 'lln' case apart. */
    
    /* First of all, if the last three characters in the buffer 
       are 'lln', everything's kosher.
    */
    if(buffer[len-2]==buffer[len-3]) {
      changed=0;
      goto decode;
    } 
    
    /*if our last two characters are 'll'
      we need to return back by two bytes, then copy 
      those two to the beginning of buffer after write,
      and start inserting subsequent reads after those 2 bytes. 
    */
      if(buffer[len-1]==buffer[len-2]){
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
    loc = rle_decode_ec(encbuffer, buffer, len-changed);
    //if (changed==2) printBuffer(buffer, len-1);
    for(i=0;i!=changed;i++){
      buffer[i]=buffer[(len+i)-changed];
    }

    for(i=0;i<loc;i++){
      fputc(encbuffer[i], outfile);
    } 
  }
  free(buffer);
  free(encbuffer);
  
}

