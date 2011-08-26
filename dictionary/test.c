#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h> //memset
#include "dictionary.h"


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
  test_encode_dict();
  //test_scan_words();
  //test_keywords();
  return 0;
}
#endif
/*int main(void) {
	FILE * f;
	f = fopen("test.log", "a");
	fprintf(f, "hello, world\n");
	fclose(f);
	return 0;
}*/

#ifdef TEST

enum { FORMATTED_LEN = 24 };

void dump_dict(void){ 
  char * formatted = malloc(MAX_WORD_LEN+2*sizeof(char));
  int i = 0;
  //strcpy(formatted, "                    ");
  memset(formatted, ' ', FORMATTED_LEN);
  memset(formatted+FORMATTED_LEN, '\0', sizeof(char));

  keyword * current = key_head;
  if (current == 0) return;
  printf("\n");
  while (current){
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
    if(current->next == 0 && (i+1)<dict_size){
      printf("\nERROR:%d|%d|%d|%s\n", (int)current->prev, (int)current, (int)current->next, current->word);
      break;
    }
    current = current->next;

  }
  printf("\ndictionary size: %d", dict_size);
}
int test_keywords(void){
  key_head = new_keyword(0);
  keyword * current = key_head;
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
  //keyword * current = key_head;
  keyword * current = find_in_dict("bar");
  keyword * next = find_in_dict("talo"); 
  //keyword * next = find_in_dict("tupa6");
  printf("\nstart test");
  swap_u(current, next);
  /*
  while(next){
    next = current->next;
    if (next && next->length>0) {
      swap_u(next, current);
    } else {
      next = 0;
    }
  }
  */
  dump_dict();
  return 0;
}

int test_scan_words(void){
  char src[]="talo talon talona taloa taloksi talossa talosta taloon talolla talolta talolle talotta taloineen taloin"; 
  //char src[]="talo talon talona taloa taloksi talossa talosta"; // taloon talolla talolta talolle talotta taloineen taloin"; 
  //char src[]="talo talon taloin";
  //char src[]="talo talo talo";
  scan_words(src, sizeof(src));
  count_words(src, sizeof(src));
  printf("\nwords counted.\n");
  slow_sort();
  //quicksort(key_head, key_tail);
  printf("sorted\n");
  dump_dict();
  return 0;
}

int test_encode_dict(void){
  char src[]="talo talon talona taloa taloksi talossa talosta taloon talolla talolta talolle talotta taloineen taloin";
  char * dest = malloc(1000*sizeof(char));
  encode_dict(dest, src, sizeof(src) );
  dump_dict();
  //printf("%s\n" , dest);
//  test_decode_dict(dest);
  free(dest);
  return 0;
}

/** for testing purposes only */
int test_decode_dict(const char * src){
  char * dest = malloc(1000*sizeof(char));
  dest[0] = '\0';
  /*char * word =0;
  int i=0;
  int index = 0;
  int j=0;
  */
  //int key = 0;
  /* stoopid ! */
/*  while(!memcmp("\0", src+i, sizeof(char))){
    if (memcmp("#", src+i, sizeof(char))){
      index = (int)*(src+i+1);
      word = get_word(index);
      memcpy(dest+j, word, strlen(word));
      j+=strlen(word);
    } 
    i++;
  }*/
  printf("\nDecoded: %s", dest);
  return 0;
}
#endif
