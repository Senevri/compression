/* SAE_ENC $(DATE)
 * user interface for various compression algorithms
 * @author esa karjalainen
 * */
/*Testing version control*/

#include <stdio.h>
#include <string.h>
#include "compress.h"

void compress(FILE *, FILE*);
void extract(FILE *, FILE*);
void printBuffer(char *buf, int len);

int main(int argc, char *argv[]){

  /*c/x [type] infile outfile*/
  if (argc==4||argc==5){ 
    FILE *infile;
    FILE *outfile;
    int nfilearg;
    if(argc==4){
      nfilearg=2;
    } 
    else {
      nfilearg=3;
    }

    infile = fopen(argv[nfilearg], "r");
    if(infile==0){
      printf("\nCouldn't open infile\n");
      return -1;
    }
    
    outfile = fopen(argv[nfilearg+1], "w");
    if(outfile==0){
      printf("\nCouldn't open infile\n");
      return -1;
    }
    
    /* Read data to buffer, compress, */
    switch((int)*argv[1]){
    case 'c':
      compress(infile, outfile);
      break;
    case 'x':
      extract(infile, outfile);
      break;
    }
    
    fclose(infile);
    fclose(outfile);
  }
    else {
    printf("\nInvalid command line arguments!\n");
    printf("enc (c/x) infile outfile\n");
  } 
    return 0;
}

/*
 don't remember why I did it this way, probably to 
 wrap the compression-format specific file.  
*/
void compress(FILE *infile, FILE* outfile){
  compress_rle_ec(infile, outfile);
}

void extract(FILE *infile, FILE* outfile){
  extract_rle_ec(infile, outfile);
}

/* to check command parameters */
void printBuffer(char *buffer, int len){
  char debug[100];
  sprintf(debug, "%c%c%c", buffer[len-2],buffer[len-1], buffer[len]);
  printf("|%s|\n", debug );
}
