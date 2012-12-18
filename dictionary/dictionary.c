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


#ifndef TEST
static keyword* key_head;
static keyword* key_tail;
static int dict_size;
#endif

int decode_dict(char * dest, char * src, int len)
{
  return 0;
}


const char * get_word(int index){
  int i=0;
  keyword * current = key_head;
  while(current->next){
    if(i == index){
      return current->word; 
    }
    current = current->next;
    i++;
  }
  return 0;
}

keyword * find_in_dict(const char * word){
  keyword * current = key_head;
  int i = 1;
  if (!key_head || !key_head->word) {
    return 0;
  }
	while(current->next){ /* any nonzero == true */
		if (0==strcmp(current->word, word)) { /* strcmp returns difference between szStrings*/
			return current; /* just in case, return 1-based index of location -- isn't this the pointer, though? */
		}
    if(current->next) {
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
  current->word=malloc(MAX_WORD_LEN+1*sizeof(char)); /* malloc word size + 0 slace */
  memcpy(current->word, word, len);
  memcpy(current->word+len, "\0", 1); /*null-terminate*/
  //strncpy(current->word, word, len);
  current->length=len;
  current->count = 1;
  current->next = new_keyword(current);     
  key_tail = current;
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
  key_head = new_keyword(0);
  keyword * current = key_head;
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
      if(w_loc==MAX_WORD_LEN ||w_loc>(1+len-i)){
        w_loc=0;
        j--;
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
    key_head = prev; /*let's not break the dict, mmkay?*/
  } else { /*removed only element*/
    prev = 0;    
    key_head = prev;
  }
    dict_size--;
    free(current);
    return prev; /* actually next, regardless now first element */
}

/*remove unused keywords*/
void remove_unused_keys (void){
  keyword * current = key_head;
  while(current) {
    if(current->count==0 || current->length==0) {
      current = remove_keyword(current);
    } else {
      current = current->next;
    }
  }
}

/** count instances of keywords in strings - do not allow overlap */ 
int count_words(const char * src, const int len){
  keyword * current = key_head;
  int i = 0;
  char * word = 0;
  printf("counting words. \n Sauce: %s\nLength: %d\n", src, len);
  while(current) { /* will this miss the last word? */
    if (current->length==0) {
      if(current->next) current=current->next;
    } 
    current->count = 0;
//    printf("comparing [%s(%d)]", current->word, current->length);
    for (i=0; i<=(len-current->length); i++) {
      word = malloc((1+current->length)*sizeof(char));
      strncpy(word, (src+i), current->length);
      memset(word+current->length, '\0', sizeof(char));
      //printf("[%s : %s = %d ]\n", word, current->word, strncmp(word, current->word, current->length));
      if(0 == strncmp(word, current->word, current->length)){
      	/* found a match for a keyword */
        current->count++;
        if (current->length) i += (current->length-1);
      }
      /*if ((0==strncmp(current->word, " ", current->length)) 
          && (0==strncmp(word," ", current->length))) {
        printf("%s|%s-%d,%d/",current->word, word, current->length, current->count);
      }*/
      free(word); 
    }
    current = current->next;  
  }
  remove_unused_keys();
  return 0;
}
/*adjacent ordered swap*/
 
void swap(keyword * current, keyword *next){
  //int flag=0;
  keyword * prev = current->prev;
  keyword * nextnext = next->next;
   if (prev){
    prev->next = next;
    next->prev = prev;
  } else {
    key_head = next;
    next->prev = 0;
  }
  current->prev = next;
  current->next = next->next;
  next->next = current;
  nextnext->prev = current;
}

/*adjacent ordered data swap*/

void swap_d(keyword * current, keyword *next){
  /*data to swap: count, length, word*/
  int tmplen, tmpc;
  char word[MAX_WORD_LEN+1];
  tmplen = current->length;
  tmpc   = current->count;
  strncpy(word, current->word, tmplen);
  //tmp = current
  current->count=next->count;
  current->length=next->length;
  strncpy(current->word, next->word, next->length);
  memset(current->word+current->length, '\0', sizeof(char));
  next->count=tmpc;
  next->length=tmplen;
  strncpy(next->word, word, tmplen);
  memset(next->word+next->length, '\0', sizeof(char));

  /*all done hopefully*/
}



/*universal swap*/
void swap_u(keyword * a, keyword *b){
  //int flag=0;
  keyword * temp = new_keyword(0);
  if(0==a||0==b) return;
  temp->prev = a->prev;
  temp->next = a->next;


  //printf("+");
  if (b==a->next) {
    a->next = b->next;
    a->prev = b;
    a->next->prev = a;
    b->next = a;
    b->prev = temp->prev;
    if(b->prev) b->prev->next = b; else key_head=b;
  } else if (a==b->next){
     a->prev = b->prev;
    a->next = b;
    if(a->prev) a->prev->next = a; else key_head=a;
    b->next = temp->next;
    b->prev = a;
    b->next->prev=b;
  } else {
    a->prev = b->prev;
    if (a->prev) a->prev->next = a; else key_head=a;
    a->next = b->next;
    a->next->prev = a;
    b->prev = temp->prev;
    if (b->prev) b->prev->next = b; else key_head=b;
    b->next = temp->next;
    b->next->prev = b;
  }
  free(temp);
}

int swap_condition(keyword* current, keyword *next) {
      int w_c =0; int w_n=0;
      /* sort condition start*/
      if (current->count>1 ) { 
        w_c = current->count*current->length; 
      } else {
        w_c = 1; //(4*current->length/MAX_WORD_LEN);
      }
      if (next->count>1) { 
        w_n = next->count*next->length;
      } else {
        w_n = 1; //(4*next->length/MAX_WORD_LEN);
      }
      if(w_c == w_n){
        w_c = current->length;
        w_n = next->length;
      }
      /* sort condition end */
      return (w_c-w_n);
}


int slow_sort(){
  /* Sort condition: Longest repeatable block
   * 
   * ASSUMPTION : longest code rep = x. Thus, compression ratio = (x/word->length)
   * Dictionary size is increased by word->length
   * */
  keyword * current = key_head;
  keyword * next = current->next;
  int swaps = 0;
  while(current) {
    next = current->next;
    if(next) {
      /* if current weight is less than next weight swap */
      if(0>swap_condition(current, next)) {
        swap(current, next); /*current->next may be null*/
        //printf("post:[%s<>%s]\n", current->word, next->word);
        //printf("nn%d|c%d|nnn%d|nn%d|c%d\n",current->prev, current, current->next, next, next->next);
        swaps++;
        current = key_head;
        //current = current->next;
      } else { 
        current = current->next;
      }
      //printf("%d\n", current);
      //next = current->next;
    } else {
      current = 0;
    }
    //next = current->next;
  } 
  printf("slow_sort is done. Swapped %d words\n", swaps);
  return 0;
}



int quicksort(keyword *l, keyword *r) {

  int pivot =0;
  int lcur = 0;
  int rcur = 0;
  int swaps = 0;
  keyword * temp;
  //pivot = swap_condition(l, r); /*if pivot<0 swap */
  if(l->count>1) pivot = l->count*l->length; else pivot=1;
  lcur = pivot;
  rcur = pivot;
  printf("pivot: %d\n", pivot);
  while (l->next != r){
    while(rcur <= pivot && l->next!=r) { 
      r = r->prev;
      if(r->count>1) rcur = r->count*r->length; else rcur=1;

      //printf(".1");  
    }
    if (rcur>lcur) {
      swap_u(l, r);
      temp = l;
      l = r->next;
      r = temp->prev;
      swaps++; 
    }
    while (lcur >= pivot && l->next!=r) {
      l=l->next;
      //printf("%s", l); 
      if(0!=l){
      
        if(l->count>1) lcur = l->count*l->length; else lcur=1;
        //printf(".");
      } else (lcur = pivot -1);
    }
    if (lcur<rcur) {
       swap_u(l, r);
      temp = r;
      if (l->prev) r= l->prev; else r = key_tail;
      l = temp->next;
      swaps++;
    }
  //printf("|");
  }
  //if (l->prev && l->prev!=key_head) quicksort(key_head, l->prev);
  if (r->next && r->next!=key_tail) quicksort(r->next, key_tail);
  printf("\nSwaps: %d", swaps);
  return 0;
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
int encode_dict(char * dest, char * src, int len) {
  keyword * current = key_head;
  char * map = malloc(1+len*sizeof(char));
  int i = 0;
//  int j = 0;
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

  current = key_head;
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
    }
    if (i>0) {
      //printf(".");
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
  remove_unused_keys();
  return 0;
  }


