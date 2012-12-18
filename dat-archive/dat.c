/* ex: set tabstop=2 expandtab softtabstop=2 shiftwidth=2:  */
/* DAT-ARCHIVE $(DATE)
 * creates a single file out of multiple files.
 * @author esa karjalainen
 * */

/* This could be extended to full TAR, readily enough... But, 
 * it's more of my rite-of-passage code, so probably not. 
 */

/* Format: 
 * One needs to choose whether to include index, or not. 
 * Anyway, 255 bytes - or maybe they should be wchar_t:s, for name, 
 * followed by a single 32-bit number... Is there a way to assure that
 * in modern environment, though? In any case, 32 bits is enough to 
 * tell us the number of bytes of the stored file. 
 *
 * So, basically: 
 * 256 characters - name
 * 1 large number - bytes
 *(variable number of bytes) file data 
 */

/* Loop that until we hit EoF. */

/* we could add a header telling us the number of files in archive, 
 * or an index with direct pointing to files, but let's not get 
 * too complicated yet. */

/* I don't recall the libraries we'll need without reference. Oh well.*/
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#include <unistd.h>
#include <sys/types.h> 
#else
#include <io.h>
#endif
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <string.h>


struct datablock{
	char filename[256];
	unsigned long bytes;
  unsigned char * data;
  struct datablock * next;
};

/* get file size  --- from snippet */ 
int filesize(char * filename) {
  int fp, rval;
  struct stat buf;
  long file_size;

  if ((fp = open(filename, O_RDONLY)) == -1) {
    printf("Error opening the file \n");
  } else {
    //file_size = filelength(fp);
    if (fstat(fp, &buf) < 0 ) printf( "fstat error\n");
    file_size = buf.st_size;
    printf("The file %s size in bytes is %ld\n", filename, file_size);
    close(fp);
  } 
  return file_size;
}

/* read file to mem */
unsigned char * fileread(char *filename, unsigned long size) {
  FILE * fp;
  unsigned char * data;
  data = malloc(size*sizeof(unsigned char));
  fp = fopen(filename, "rb");
  fread(data, sizeof(unsigned char), size, fp);
  fclose(fp);
  return data;
}



/* Calculate size of data, create appropriate header, datablock */
struct datablock * create_block(char * filename) {
	struct datablock *h = malloc(sizeof(struct datablock));
    strcpy(h->filename, filename);
    h->bytes=filesize(filename);
    h->data = fileread(filename, h->bytes);
    h->next = NULL;
	return h;
}


/* append by default */
int write_to_file(char * filename, struct datablock * hd, void * data){
  return -1;
}

/* append by default */
int write_datablocks_to_file(char * filename, struct datablock * hd){
  FILE * file;
  file=fopen(filename, "wb");
  struct datablock * h = hd;
  while(h != NULL) {
    printf("writing %s...\n", h->filename);
    fwrite(h->filename, sizeof(char), 255, file); 
    fwrite(&h->bytes, sizeof(unsigned long), 1, file); 
    fwrite(h->data, sizeof(unsigned char), h->bytes, file); 
    h = h->next;
  }
  fclose(file);
  return 0;
}

/* extract */
int extract_file(char * filename){
  return -1;
}

int main (int argc, char* argv[]) {
  struct datablock *head;
  struct datablock *h = NULL;
	if (argc>2) {
    if (argv[1][1] == 0 && argc>2) { /* single-char command */
      if (argv[1][0] == 'x') {
        extract_file(argv[2]);
        return -1;
      }
    }
		//should we assume outfile infiles? sure, why not...
    head = h;
    int i;
    for (i=2; i<argc; i++) {
      if (NULL == h) {
        h = create_block(argv[i]);
        if (NULL == head) {
          head = h;
        }
      } else {
        h->next = create_block(argv[i]);
        h = h->next;
      }
    }
    /* works...*/
    if (argc>=3) {
      write_datablocks_to_file(argv[1], head);
    }
    
    //cleanup
    h = head;
    while(h != NULL) {
      free(h->data);
      h = h->next;
    }
    free(h);
    return 0;
	} else {
		printf("usage: outfile infile[1..n]\n");
    return -1;
	}
}



