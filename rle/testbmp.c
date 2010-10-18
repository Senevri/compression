/* ex: set tabstop=2 expandtab softtabstop=2 shiftwidth=2:  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "../compress.h"
#include "rle.h"
#define FILENAME ="e0.bmp"
/*tests:*/

static int debugflag=0;
static int alltests=0;
char debug_buffer[2000]="";

void debug_print(const char * src) {
  if(debugflag) {
    printf(src);
  }
}

int test(int status){
  if(status==0){
    printf("\t[OK]\n");
  } else {
    printf("\t[FAIL]: status %d\n", status);
  }
}
/*declarations*/
int test_load_save(void);
int test_load_compress_save(void);
int test_decompress(void);

int main(int argc, char *argv[]){
  int i;
  int alltests=0;
  for (i=0;i<argc;i++){
    char c = (char)*argv[i]; /*get first char of arg*/
    switch(c){
      case 'd':
       debugflag=1;
       break;
      case 'a':
       alltests=1;
    }
  }
  if(alltests) {
  
  }
  test(test_load_save());
  test(test_load_compress_save());
  test(test_decompress());

}

int test_load_save(void){
    FILE *infile;
    FILE *outfile;
    char *buffer=malloc(1024*sizeof(char));
    char singlechar=0;
   
    printf("test_load_save");
    infile = fopen("e0.bmp", "rb");
    
    if(infile==0){
      printf("\nCouldn't open infile\n");
      return -1;
    }
    
    outfile = fopen("e0.unchanged.bmp", "wb");
    if(outfile==0){
      printf("\nCouldn't open infile\n");
      return -1;
    }
    
    /* Read data to buffer, compress, */
    while(!feof(infile)){
      singlechar=fgetc(infile);
      fputc(singlechar, outfile);

    }

    fclose(infile);
    fclose(outfile);
    return 0;
}
int test_load_compress_save(void){
    FILE *infile;
    FILE *outfile;
    char *buffer=malloc(1024*sizeof(char));
    char singlechar=0;
   
    printf("test_load_compress_save");
    infile = fopen("e0.bmp", "rb");
    
    if(infile==0){
      printf("\nCouldn't open infile\n");
      return -1;
    }
    
    outfile = fopen("e0.bmp.rle", "wb");
    if(outfile==0){
      printf("\nCouldn't open infile\n");
      return -1;
    }
    
    /* Read data to buffer, compress, */
    
    compress_rle_ec(infile, outfile);

    fclose(infile);
    fclose(outfile);
    return 0;
}

int test_decompress(void){
    FILE *infile;
    FILE *outfile;
    char *buffer=malloc(1024*sizeof(char));
    char singlechar=0;
   
    printf("test_decompress");
    infile = fopen("e0.bmp.rle", "rb");
    
    if(infile==0){
      printf("\nCouldn't open infile\n");
      return -1;
    }
    
    outfile = fopen("e0.decomp.bmp", "wb");
    if(outfile==0){
      printf("\nCouldn't open infile\n");
      return -1;
    }
    
    /* Read data to buffer, compress, */
    
    extract_rle_ec(infile, outfile); 

    fclose(infile);
    fclose(outfile);
    return 0;
}


