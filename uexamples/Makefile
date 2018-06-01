#!/bin/sh

INCLUDE = ../inst/include
LIB_DIR = ../inst/lib


LDFLAGS = -lrtmp


all:  rtmpdump_receive  rtmpdump_receive_analyze rtmpdump_send rtmpdump_send_2

rtmpdump_receive:
	gcc -o r_$@.bin -I $(INCLUDE) -L $(LIB_DIR) $@.c $(LDFLAGS)

rtmpdump_receive_analyze:
	gcc -o r_$@.bin -I $(INCLUDE) -L $(LIB_DIR) $@.c $(LDFLAGS)

rtmpdump_send:
	gcc -o r_$@.bin -I $(INCLUDE) -L $(LIB_DIR) $@.c $(LDFLAGS)

rtmpdump_send_2:
	gcc -o r_$@.bin -I $(INCLUDE) -L $(LIB_DIR) $@.c $(LDFLAGS)

clean:
	rm r_*.bin -rf


