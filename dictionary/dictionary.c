#include "dictionary.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>
/*
  Contents: Dictionary functions: handling the dictionary and it's keys.
  we need: 
  - Function to load the dictionary, 
  - Function to decode a given buffer using a certain dictionary, 
  - Function to generate a dictionary.
  - Function to encode a given buffer using the generated dictionary. 
  - Dictionary data structures (separate file, perhaps?)
*/


/**
 * TODO: Add REAL indexing!
 */


#ifdef TEST
/*For testing*/
int test_encode_dict(void);
int test_decode_dict(const char * src);

int main(void)
{
  printf("Testing Dictionary\n");
  test_encode_dict();
  return 0;
}
#endif

static keyword* head;

int decode_dict(char * dest, char * src, int len)
{
  return 0;
}

const char * get_word(int index){
  int i=0;
  keyword * current = head;
  index = index-1;
  while(current->next){
    if(i == index){
      return current->word;
    }
  }
  return 0;
}

int find_in_dict(const char * word){
  keyword * current = head;
  int i = 1;
  if (!head->word) {
//    printf("no dictionary\n");
    return 0;
  }
	while(current->next){ /* any nonzero == true */
//    printf("has dictionary: ");
		if (0==strcmp(current->word, word)) { /* strcmp returns difference between szStrings*/
//      printf("found word\n");
			return i; /* just in case, return 1-based index of location */
		}
//    printf("no word\n");
    if(0!=current->next) {
      current = current->next;
	  	i++;
    } else {
      return 0;
    }
	}
//  printf("return");
	return 0;
}

keyword * new_keyword(){
    keyword * next = malloc(sizeof(keyword)); 
	  next->next = 0;
    next->word = 0;
    next->count = 0;
    next->length = 0;
    return next;
}

void insert_keyword(keyword * current, const char * word, const int len) {
  current->word=malloc(len+1*sizeof(char)); /* malloc word size + 0 slace */
  memcpy(current->word, word, len);
  current->length=len;
  current->count = 1;
  current->next = new_keyword();     
  /* sete the word, now get a new word  */
}

int scan_words(const char * src, const int len){
  char word[MAX_WORD_LEN+1];
  int w_loc=0;
  int i;
  /*
    Dictionary theory: 
    Every character might be a keyword. scan input, keep on adding characters
    to the dictionary. As you progress, prune instaces which do not appear outside longer keywords.
   */
  head = malloc(sizeof(keyword)); /* alloc single keyword */
  head->next = 0;
  keyword * current = head;
  
  for(i=0;i<len;i++){
    word[w_loc]=src[i]; 
    word[(w_loc+1)]='\0'; /* must null-terminate string */
    if(!find_in_dict(word)){/* only do this if the word doesn't already exist */
      printf(">");
      insert_keyword(current, word, w_loc+1);
      current = current->next;
    } else {
      printf("#");
    }
    w_loc++;
    if(w_loc==MAX_WORD_LEN){
      w_loc=0;
      //strcpy (word, "");
    }
  }
  return 0;
}

int count_words(const char * src, const int len){
  keyword * current = head;
  int i = 0;
  char * word = 0;
  printf("counting words. \n Sauce: %s\nLength: %d\n", src, len);
  while(0!=current->next) {
//    printf("comparing [%s(%d)]", current->word, current->length);
    for (i=0; i!=len; i++) {
      word = malloc((current->length)*sizeof(char));
      memcpy(word, (src+i), current->length);
      word[current->length] = '\0';
//      printf("[%s : %s = %d ]", word, current->word, strncmp(word, current->word, current->length));
      if(0 == strncmp(word, current->word, current->length)){
//	printf("++");
      	current->count++;
      }
      free(word); 
    }
    current = current->next;  
  }
  return 0;

}

void swap(keyword * current, keyword *next){
  char swp[MAX_WORD_LEN];
  int tmp = 0;
  tmp = current->count;
  current->count = next->count;
  next->count = tmp;
//  printf("swapped %d for %d\n", next->count, current->count);


//  if(current->word && next->word) {
    memcpy(&swp, current->word, current->length);
    free(current->word);
    current->word = malloc(next->length*sizeof(char));
    memcpy(current->word, next->word, next->length);
    free(next->word);
    next->word = malloc(current->length*sizeof(char));
    memcpy(next->word, &swp, current->length);
//  }
  tmp =current->length;
  current->length = next->length;
  next->length = tmp;
}

void slow_sort(){
  keyword * current = head;
  keyword * next = 0;
  int w_c, w_n;
  while(current->next) {
    next = current->next;
    w_c = ((current->count+1) * current->length);
    w_n = ((next->count+1) * next->length);
    if((w_c) < (w_n)) {
      swap(current, next);
      printf("-");
      current = head;
      //current = current->next;
    } else { 
      current = current->next;
    }
  } 
  printf("slow_sort is done\n");
}

/**
 * encode_dict
 * @param dest : pointer to a char array to which output the encoded
 * string
 * @param src : source
 * @param len : length of source
 * 
 */
/* should probably return a pointer to the dictionary, too. */
int encode_dict(char * dest, char * src, int len)
{
  keyword * current = head;
  char * map = malloc(1+len*sizeof(char));
  int i = 0;
  int j=0;
  int keywords_removed = 0;
//  printf("len: %d\n", len);
  scan_words(src, len);
//  printf("len: %d\n", len);
  /* TODO: now we have a linked list of potential words; next step is to
   * prune out duplicates. I guess we could do that at dict-build time,
   * too.
   */

  /*	Steps:
   *	-  Go through our sourcestring, count instances of words
   *	-  Calculate weight for each keyword: count * length, weight
   *	count by, say, two.
   */
  count_words(src, len);
  slow_sort();
  /*
   *  - start removing the highest-valued keys from string, split
   *  remaining string into segments, until each segment's size is zero
   *  at which point we'll have encoded the entire string.
   *  This step could be done 
   */
  /* Intermediary step: encode using the following notation: #%d */
  /* wait, if we just parse from top, find the first match, replace and
   * move source pointer? naw, won't work... I know, we'll use a
   * map. */
  current = head;
  memset(map, '-', len*sizeof(char));
  while(current->next){
		int running = 1;
//    printf("start map jump\n");
    while((1==running) && (0==memcmp( "+", map+i, sizeof(char) ))){
	      i++;
				if(i>=len){
					running=0;
				}
    }
		if (i>0) {
	    //printf("map jump done: %d characters\n", i);
		}
    if(0==strncmp(src+i, current->word, current->length)){
			//char conv[9];
      memset(src+i, (char)i, current->length); /*clear from source*/
      //memset(src+i, '#', 1); /*tag*/
			//sprintf(conv, "#%x", i+1);
      //memset(src+i, conv, strlen(conv)-1); /*tag*/
//      sprintf(src+i+1, "%d",i); 
     
      memset(map+i, '+', current->length); /*mark area in map*/
      i += current->length; /*jump ahead*/
      printf("removed keyword %s from src\n", current->word);
			printf("map: %s\n", map);
      printf("src: %s\n", src);
      keywords_removed++;
    } else {
      i++;
    }
    if(i>=len){ /*if through string with a keyword, get next. */
      i=0;
      if (0==keywords_removed){
        current->count = 0; /* for pruning list for saving */
      }
      keywords_removed =0;
      current = current->next;
    }
  }
//	memset(src+len, '\0', sizeof(char));
  printf("encoded, dirty\n");
  printf("src: %s\n", src);
  /* clear out all '\0's */
  for(i=0;i<len;i++){
    if(memcmp("?", src+i, sizeof(char))){
      memcpy((dest+j) ,(src+i), sizeof(char));
      j++; 
    }    
  }
  memcpy(dest+j, "\0", sizeof(char));
  printf("dest: %s\n", dest);
  printf("\n");
  return 0;
}


#ifdef TEST
/*
bool quickSort() {

#define  MAX_LEVELS  1000


  int  piv, beg[MAX_LEVELS], end[MAX_LEVELS], i=0, L, R ;

  beg[0]=0; end[0]=elements;
  while (i>=0) {
    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L]; if (i==MAX_LEVELS-1) return NO;
      while (L<R) {
        while (arr[R]>=piv && L<R) R--; if (L<R) arr[L++]=arr[R];
        while (arr[L]<=piv && L<R) L++; if (L<R) arr[R--]=arr[L]; }
        arr[L]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L; }
    else {
      i--; }}
    return YES; }
*/


int test_encode_dict(void){
  char src[]="talo talon talona taloa taloksi talossa talosta taloon talolla talolta talolle talotta taloineen taloin";
  char * dest = malloc(1000*sizeof(char));
  char * formatted = malloc(MAX_WORD_LEN+2*sizeof(char));
  strcpy(formatted, "           ");


//  printf("sizeof: %d\n", sizeof(src));
  encode_dict(dest, src, sizeof(src) );
 /*let's not leak*/
  keyword * current = head;
  if (current == 0) return 0;
  keyword * tmp = 0;
  while (current->next!=0){
    memcpy(formatted, current->word, current->length);
//    printf("word: "); printf(formatted); 
//    printf(" | length: %d\t | count: %d\t TEST_weight: %d\n", current->length, current->count, (current->length*current->count));
    strcpy(formatted, "           "); 
    free(current->word);
    tmp = current;
    current = current->next;
    free(tmp);
  }
  printf("%s\n" , dest);
//  test_decode_dict(dest);
  free(dest);
  return 0;
}

int test_decode_dict(const char * src){
  char * dest = malloc(1000*sizeof(char));
  char * word =0;
  int i=0;
  int index = 0;
  int j=0;
  int key = 0;
  while(!memcmp("\0", src+i, sizeof(char))){
    if (memcmp("#", src+i, sizeof(char))){
      index = (int)*(src+i+1);
      word = get_word(index);
      memcpy(dest+j, word, strlen(word));
      j+=strlen(word);
    } 
    i++;
  }
  printf("\nDecoded: %s", dest);
  return 0;
}
#endif
