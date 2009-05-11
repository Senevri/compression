CC:=gcc

CARGS:=-g -Wall -O2
LIBS:=

TARGETS:=test

all: test

test: dictionary.c
	$(CC) $(CARGS) -DTEST -o $@ $? $(LIBS)

clean: 
	rm -f $(TARGETS) *~