#include <string.h>
//#include "../compress.h"
#include "rle.h"
/*tests:*/
void test1();
void test2();
void test3();
void test4();
void ectest1();
void ectest2();
void ectest3();
void ectest4();

int main(void){
  printf("test 1\n");
  test1();
  printf("test 2\n");
  test2();
  printf("test 3\n");
  test3();
  printf("test 4\n");
  test4();
  printf("test 1ec\n");
  ectest1();
  printf("test 2ec\n");
  ectest2();
  printf("test 3ec\n");
  ectest3();
  printf("test 4ec\n");
  ectest4();

}

void test4(){
  unsigned char str[]="asdklrkraklaskdiekkkdpspkeappsffffidkelllsk";
  unsigned char buf[1024];
  unsigned char buf2[1024];
  int t_l, c_l;
  printf("\ncompressed %d bytes, ", sizeof(str));
  t_l=rle_encode(buf, str, sizeof(str));

  printf("compressed size: %d\nvalue: %s\n", 
  	 t_l, buf);
  c_l=t_l;
  t_l=rle_decode(buf2, buf, c_l);
  printf("string was: %s\ndecoded to: %s\n", str, buf2);

}


void test2(){
  unsigned char str[]="abcdefgh12345678";
  unsigned char buf[256];
  unsigned char buf2[256];
  int t_l, c_l;
  t_l=rle_encode(buf, str, sizeof(str));
  printf("\ncompressed %d bytes, compressed size: %d\n", sizeof(str), t_l);
  c_l=t_l;
  t_l=rle_decode(buf2, buf, c_l);
  printf("string was: %s\ndecoded to: %s\n", str, buf2);

}

void test3(){
  unsigned char str[]="";
  unsigned char buf[256];
  unsigned char buf2[256];
  int t_l, c_l;
  t_l=rle_encode(buf, str, sizeof(str));
  printf("\ncompressed %d bytes, compressed size: %d\n", sizeof(str), t_l);
  c_l=t_l;
  t_l=rle_decode(buf2, buf, c_l);
  printf("string was: %s\ndecoded to: %s\n", str, buf2);

}

void test1(){
  unsigned char testbuffer[256]="";
  unsigned char teststring[33] = "aaaaabbbbbbcccccccdddddddeeeeeee";
  int testbuffer_lenght;
  int compressed_lenght;
  printf("compressing %d bytes\n", sizeof(teststring));
  testbuffer_lenght=rle_encode(testbuffer, teststring, sizeof(teststring));
  printf("%s compressed to\n%s, lenght:%d\n", teststring, testbuffer, 
	 testbuffer_lenght);
  compressed_lenght=testbuffer_lenght;

  memset(teststring, 0, sizeof(teststring));
  strncpy(teststring, testbuffer, testbuffer_lenght);

  testbuffer_lenght=rle_decode(testbuffer, teststring, compressed_lenght);
  printf("%s decompressed to\n%s, lenght:%d\n", teststring, testbuffer, 
	 testbuffer_lenght);  
}

void ectest4(){
  unsigned char str[]="asdklrkraklaskdiekkkdpspkeappsffffidkelllsk";
  unsigned char buf[1024];
  unsigned char buf2[1024];
  int t_l, c_l;
  printf("\ncompressed %d bytes, ", sizeof(str));
  t_l=rle_encode_ec(buf, str, sizeof(str));

  printf("compressed size: %d\nvalue: %s\n", 
  	 t_l, buf);
  c_l=t_l;
  t_l=rle_decode_ec(buf2, buf, c_l);
  printf("string was: %s\ndecoded to: %s\n", str, buf2);

}


void ectest2(){
  unsigned char str[]="abcdefgh12345678";
  unsigned char buf[256];
  unsigned char buf2[256];
  int t_l, c_l;
  t_l=rle_encode_ec(buf, str, sizeof(str));
  printf("\ncompressed %d bytes, compressed size: %d\n", sizeof(str), t_l);
  c_l=t_l;
  t_l=rle_decode_ec(buf2, buf, c_l);
  printf("string was: %s\ndecoded to: %s\n", str, buf2);

}

void ectest3(){
  unsigned char str[]="";
  unsigned char buf[256];
  unsigned char buf2[256];
  int t_l, c_l;
  t_l=rle_encode_ec(buf, str, sizeof(str));
  printf("\ncompressed %d bytes, compressed size: %d\n", sizeof(str), t_l);
  c_l=t_l;
  t_l=rle_decode_ec(buf2, buf, c_l);
  printf("string was: %s\ndecoded to: %s\n", str, buf2);

}

void ectest1(){
  unsigned char testbuffer[256]="";
  unsigned char teststring[33] = "aaaaabbbbbbcccccccdddddddeeeeeee";
  int testbuffer_lenght;
  int compressed_lenght;
  printf("compressing %d bytes\n", sizeof(teststring));
  testbuffer_lenght=rle_encode_ec(testbuffer, teststring, sizeof(teststring));
  printf("%s compressed to\n%s, lenght:%d\n", teststring, testbuffer, 
	 testbuffer_lenght);
  compressed_lenght=testbuffer_lenght;

  memset(teststring, 0, sizeof(teststring));
  strncpy(teststring, testbuffer, testbuffer_lenght);

  testbuffer_lenght=rle_decode_ec(testbuffer, teststring, compressed_lenght);
  printf("%s decompressed to\n%s, lenght:%d\n", teststring, testbuffer, 
	 testbuffer_lenght);  
}
