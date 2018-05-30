#!/bin/sh

INCLUDE = ../inst/include
LIB_DIR = ../inst/lib


LDFLAGS = -lrtmp


all:  rtmpdump_receive  rtmpdump_receive_analyze


rtmpdump_receive:
	gcc -o r_$@.bin -I $(INCLUDE) -L $(LIB_DIR) $@.c $(LDFLAGS)

rtmpdump_receive_analyze:
	gcc -o r_$@.bin -I $(INCLUDE) -L $(LIB_DIR) $@.c $(LDFLAGS)


clean:
	rm r_*.bin -rf


