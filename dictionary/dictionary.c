/* ex: set tabstop=2 expandtab softtabstop=2 shiftwidth=2:  */
/* TODO: Probably need to refactor this at some point */

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
int test_swap(void);
int test_keywords(void);
int test_scan_words(void);
int test_encode_dict(void);
int test_decode_dict(const char * src);

int main(void)
{
  printf("Testing Dictionary\n");
  //test_swap();
  //test_encode_dict();
  test_scan_words();
  //test_keywords();
  return 0;
}
#endif

static keyword* head;
static int dict_size;

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

keyword * find_in_dict(const char * word){
  keyword * current = head;
  int i = 1;
  if (!head || !head->word) {
    return 0;
  }
	while(current->next){ /* any nonzero == true */
		if (0==strcmp(current->word, word)) { /* strcmp returns difference between szStrings*/
			return current; /* just in case, return 1-based index of location -- isn't this the pointer, though? */
		}
    if(0!=current->next) {
      current = current->next;
	  	i++;
    } else {
      return 0;
    }
	}
	return 0;
}

keyword * new_keyword(keyword * current){
    keyword * next = malloc(sizeof(keyword)); 
	  next->next = 0;
    next->prev = current;
    next->word = 0;
    next->count = 0;
    next->length = 0;
    return next;
}

void insert_keyword(keyword * current, const char * word, const int len) {
  current->word=malloc(len+1*sizeof(char)); /* malloc word size + 0 slace */
  memcpy(current->word, word, len);
  memcpy(current->word+len, "\0", 1); /*null-terminate*/
  //strncpy(current->word, word, len);
  current->length=len;
  current->count = 1;
  current->next = new_keyword(current);     
  /* set the word, now get a new word  */
}

int scan_words(const char * src, const int len){
  char word[MAX_WORD_LEN+1];
  int w_loc=0;
  int i;
  int j;
  /*
    Dictionary theory: 
    Every character might be a keyword. scan input, keep on adding characters
    to the dictionary. As you progress, prune instaces which do not appear outside longer keywords.
   */
  //head = malloc(sizeof(keyword)); /* alloc single keyword */
  //head->next = 0;
  head = new_keyword(0);
  keyword * current = head;
	keyword * dict_word = 0;
  for(i=0;i<len;i++){
    for(j=i;j<len;j++){
      word[w_loc]=src[j]; 
      word[(w_loc+1)]='\0'; /* must null-terminate string */
      //if (w_loc>1) {
        dict_word = find_in_dict(word);     
        if(!dict_word){/* only do this if the word doesn't already exist */
          insert_keyword(current, word, w_loc+1);
          current = current->next;
          dict_size++;
        } else {
          dict_word->count++;
        }
      //}
      w_loc++;
      if(w_loc==MAX_WORD_LEN ||w_loc>(len-i)){
        w_loc=0;
      }
    }
  }
  return 0;
}

keyword * remove_keyword(keyword * current){
  keyword * prev = current->prev;
  keyword * next = current->next;
  if(prev){
    //printf("%s\n",prev->word);
    prev->next = current->next;
    if (next) next->prev = prev;
  } else if (next) { /* first element */
    prev = current->next; 
    prev->prev = 0;
    head = prev; /*let's not break the dict, mmkay?*/
  } else { /*removed only element*/
    prev = 0;    
    head = prev;
  }
    dict_size--;
    free(current);
    return prev; /* actually next, regardless now first element */
}


/** not needed anymore --- oh yes it is. bah. */ 
int count_words(const char * src, const int len){
  keyword * current = head;
  int i = 0;
  char * word = 0;
  printf("counting words. \n Sauce: %s\nLength: %d\n", src, len);
  while(current) { /* will this miss the last word? */
    current->count = 0;
//    printf("comparing [%s(%d)]", current->word, current->length);
    for (i=0; i!=(len-current->length); i++) {
      word = malloc((1+current->length)*sizeof(char));
      strncpy(word, (src+i), current->length);
      memset(word+current->length, '\0', sizeof(char));
      //printf("[%s : %s = %d ]\n", word, current->word, strncmp(word, current->word, current->length));
      if(0 == strncmp(word, current->word, current->length)){
      	current->count++;
      }
      free(word); 
    }
    current = current->next;  
  }
  /*remove unused keywords*/
  current = head;
  while(current->next) {
    if(current->count==0 || current->length==0) {
      current = remove_keyword(current);
    } else {
      current = current->next;
    }
  }

  return 0;
}


void swap(keyword * current, keyword *next){
  if (!current || !next || next->length==0) return;
  keyword * prev = current->prev;
  if (prev){
    prev->next = next;
  } else {
    head = next;
  }
  next->prev = prev;
  current->prev = next;
  current->next = next->next;
  next->next = current;
}

void slow_sort(){
  /* Sort condition: Longest repeatable block
   * 
   * ASSUMPTION : longest code rep = x. Thus, compression ratio = (x/word->length)
   * Dictionary size is increased by word->length
   * */
  keyword * current = head;
  keyword * next = current->next;
  int swaps = 0;
  int w_c, w_n;
//  if(current->length==0){
//    current=remove_keyword(current);
//  }
  while(current) {
    //next = current->next;
    if(next && next->length>0) {
      w_c = ((MAX_WORD_LEN * (current->count-1)) * current->length);
      w_n = ((MAX_WORD_LEN * (next->count-1)) * next->length);
      //if (w_c<0) w_c=current->length;
      //if (w_n<0) w_n=next->length;
      //w_c = current->count-1;
      //w_n = next->count-1;
      //w_c = ((MAX_WORD_LEN * current->count) * current->length);
      //w_n = ((MAX_WORD_LEN * next->count) * next->length);
      if((w_c) < (w_n)) {
        swap(current, next); /*current->next may be null*/
        swaps++;
        current = head;
        //current = current->next;
      } else { 
        current = current->next;
      }
      next = current->next;
    } else {
      current = 0;
    }
  } 
  printf("slow_sort is done. Swapped %d words\n", swaps);
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
  int j = 0;
  int keywords_removed = 0;
//  printf("len: %d\n", len);
  scan_words(src, len);
//  printf("len: %d\n", len);
  count_words(src, len);
  /* now we have a linked list of potential words;
	 * we also counted the times they appear in the source.
   */
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
	/* TODO: Define test blob struct, use it.
	 *
	 * */

  current = head;
  memset(map, '-', len*sizeof(char));
    memset(map+len, '\0', sizeof(char));
  while(current->next){
    int running = 1;
    //    printf("start map jump\n");
    while((1==running) && (0==memcmp( "+", map+i, sizeof(char) ))){
      i++;
      if(i>=len){
        running=0;
      }
      //printf("!");
    }
    if (i>0) {
      //printf("map jump done: %d characters\n", i);
    }
#ifdef TEST
    if(current->length == 0){
      printf("Error!\n");
      return -1;
    }
#endif //TEST

    if(0==strncmp(src+i, current->word, current->length)){
      //char conv[9];
      memset(src+i, '-', current->length); /*clear from source*/
      //memset(src+i, '#', 1); /*tag*/
      //sprintf(conv, "#%x", i+1);
      //memset(src+i, conv, strlen(conv)-1); /*tag*/
      //      sprintf(src+i+1, "%d",i); 

      memset(map+i, '+', current->length); /*mark area in map*/
      i += current->length; /*jump ahead*/
      printf("removed keyword (%s) from src\n", current->word);
      printf("map: %s\n", map);
      printf("src: %s\n", src);
      keywords_removed++;
    } else {
      i++;
    }
    if(i>=len){ /*if through string with a keyword, get next. */
      i=0;
      current->count = keywords_removed; /* for pruning list for saving */
      keywords_removed =0;
      current = current->next;
    }
  }
  //	memset(src+len, '\0', sizeof(char));
  printf("encoded, dirty\n");
  //slow_sort();
  printf("src: %s\n", src);
  /* clear out all '\0's -- this shouldn't be needed....*/
  //  for(i=0;i<len;i++){
  //    if(memcmp("?", src+i, sizeof(char))){
  //      memcpy((dest+j) ,(src+i), sizeof(char));
  //      j++; 
  //    }    
  //  }
  memset(dest+len, '\0', sizeof(char));
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

#define FORMATTED_LEN 24
void dump_dict(void){ 
  char * formatted = malloc(MAX_WORD_LEN+2*sizeof(char));
  int i = 0;
  //strcpy(formatted, "                    ");
  memset(formatted, ' ', FORMATTED_LEN);
  memset(formatted+FORMATTED_LEN, '\0', sizeof(char));
  keyword * current = head;
  if (current == 0) return;
  printf("\n");
  while (current->next){
    int cpy_len = 0;
    if (current->length<FORMATTED_LEN) {
      cpy_len = current->length;
    } else {
      cpy_len = FORMATTED_LEN;
    }
    memcpy(formatted, current->word, cpy_len);
    i++;
    printf("%d\t|word: ", i); printf(formatted); 
    printf(" | length: %d\t | count: %d\t TEST_weight: %d\n",  current->length, current->count, (current->length*current->count));
    memset(formatted, ' ', FORMATTED_LEN);
    memset(formatted+FORMATTED_LEN, '\0', sizeof(char));
    current = current->next;
  }
  printf("\ndictionary size: %d", dict_size);
}
int test_keywords(void){
  head = new_keyword(0);
  keyword * current = head;
  insert_keyword(current, "talo", 4);
  current=current->next;
  insert_keyword(current, "koti", 4);
  current=current->next;
  insert_keyword(current, "tupa", 4);
  current=current->next;
  insert_keyword(current, "mökki", 5);
  current=current->next;
  insert_keyword(current, "foo", 3);
  current=current->next;
  insert_keyword(current, "bar", 3);
  current=current->next;
  insert_keyword(current, "tupa2", 5);
  current=current->next;
  insert_keyword(current, "tupa3", 5);
  current=current->next;
  insert_keyword(current, "tupa4", 5);
  current=current->next;
  insert_keyword(current, "tupa5", 5);
  current=current->next;
  insert_keyword(current, "tupa6", 5);
  dict_size=11;

  dump_dict();
  current = find_in_dict("koti");
  
  printf("remove keyword 'koti'->\n");
  remove_keyword(current);
  dump_dict();
  return 0;
}

int test_swap(void){
  test_keywords();
  //keyword * current = head;
  keyword * current = find_in_dict("tupa6");
  keyword * next = current->next;
  //keyword * next = find_in_dict("tupa6");
  printf("\nstart test");
  //swap(current, next);
  while(next){
    next = current->next;
    if (next && next->length>0) {
      swap(current, next);
    } else {
      next = 0;
    }
  }
  dump_dict();
  return 0;
}

int test_scan_words(void){
  char src[]="talo talon talona taloa taloksi talossa talosta taloon talolla talolta talolle talotta taloineen taloin";
  scan_words(src, sizeof(src));
  count_words(src, sizeof(src));
  printf("\nwords counted\n");
  slow_sort();
  dump_dict();
  return 0;
}

int test_encode_dict(void){
  char src[]="talo talon talona taloa taloksi talossa talosta taloon talolla talolta talolle talotta taloineen taloin";
  char * dest = malloc(1000*sizeof(char));
  encode_dict(dest, src, sizeof(src) );
  dump_dict();
  printf("%s\n" , dest);
//  test_decode_dict(dest);
  free(dest);
  return 0;
}

/** for testing purposes only */
int test_decode_dict(const char * src){
  char * dest = malloc(1000*sizeof(char));
  char * word =0;
  int i=0;
  int index = 0;
  int j=0;
  int key = 0;
  /* stoopid ! */
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
