#!/bin/sh

INCLUDE = ../inst/include
LIB_DIR = ../inst/lib

#AAC_INC_DIR = ../fdk_aac/install/include
#MP3_INC_DIR = ../mp3lame/install/include
#X264_INC_DIR = ../x264/install/include
#X265_INC_DIR = ../x265/install/include
#AAC_LIB_DIR = ../fdk_aac/install/lib
#MP3_LIB_DIR = ../mp3lame/install/lib
#X264_LIB_DIR = ../x264/install/lib
#X265_LIB_DIR = ../x265/install/lib


#LDFLAGS = -lavformat -lavdevice -lavfilter -lpostproc -lavcodec -lavutil -lswresample -pthread  -ldl -lswscale -lz -lm
LDFLAGS = -lrtmp

#ULDFLAGS = -lmp3lame -lx264 -lx265 -lfdk-aac 


all:  rtmpdump_receive  muxing	remuxing video_decode_H264



rtmpdump_receive:
	gcc -o r_$@.bin -I $(INCLUDE) -L $(LIB_DIR) $@.c $(LDFLAGS)

remuxing:
	gcc -o r_$@.bin -I $(INCLUDE) -I $(AAC_INC_DIR) -I $(MP3_INC_DIR) -I $(X264_INC_DIR) -I $(X265_INC_DIR) -L $(LIB_DIR) -L $(AAC_LIB_DIR) -L $(MP3_LIB_DIR) -L $(X264_LIB_DIR) -L $(X265_LIB_DIR) $@.c  $(LDFLAGS)  $(ULDFLAGS)

video_decode_H264:
	gcc -o r_$@.bin -I $(INCLUDE) -I $(AAC_INC_DIR) -I $(MP3_INC_DIR) -I $(X264_INC_DIR) -I $(X265_INC_DIR) -L $(LIB_DIR) -L $(AAC_LIB_DIR) -L $(MP3_LIB_DIR) -L $(X264_LIB_DIR) -L $(X265_LIB_DIR) $@.c  $(LDFLAGS)  $(ULDFLAGS)




clean:
	rm r_*.bin -rf
	rm yuv*.raw -rf


