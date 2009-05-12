# compiler options
CC=gcc
CFLAGS=
# TARGET_DIR=./bin
TARGET_DIR=bin
SHELL = sh

# source files
SRCS=sae_enc.c rle/rle.c rle/rlefile.c dictionary/dictionary.c
TESTSRCS=rle/rle.c rle/rlefile.c rle/test.c

# targets
enc:
	-mkdir $(TARGET_DIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET_DIR)/enc

clean:
	rm -rf bin *.o
test:
	-mkdir $(TARGET_DIR)
	$(CC) $(CFLAGS) $(TESTSRCS) -o $(TARGET_DIR)/test_rle
