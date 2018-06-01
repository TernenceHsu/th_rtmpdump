/**
 * rtmpdump Receive
 *
 * 本程序用于接收RTMP流媒体并在本地保存成FLV格式的文件。
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "librtmp/rtmp.h"
#include "librtmp/log.h"

// 实时统计帧率 时间间隔 系统最大值
#define  MAX_COUNT_AVG_FRAME_TIME		(120)
// 实时统计帧率 时间间隔 实际统计值
#define  COUNT_TIME_D					(30)

typedef unsigned short   WORD;
#define int64_t signed long long


int64_t OSA_getCurTimeInUsec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

void OSA_print_time()
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p=gmtime(&timep);
	printf("%04d/%02d/%02d",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday);
	printf(" %02d:%02d:%02d\t",p->tm_hour,p->tm_min,p->tm_sec);
}


int InitSockets()
{
#ifdef WIN32
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(1, 1);
	return (WSAStartup(version, &wsaData) == 0);
#else
  return TRUE;
#endif
}

void CleanupSockets()
{
#ifdef WIN32
	WSACleanup();
#endif
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("usage:\n%s %s\n",argv[0],"[rtmp_url]");
		exit(-1);
	}

	InitSockets();

	int i = 0;
	double duration=-1;
	int nRead;
	//is live stream ?
	int bLiveStream=TRUE;

	int bufsize=1024*1024*10;	
	char *buf=(char*)malloc(bufsize);
	memset(buf,0,bufsize);
	long countbufsize=0;

	/* set log level */
	//RTMP_LogLevel loglvl=RTMP_LOGDEBUG;
	//RTMP_LogSetLevel(loglvl);

	RTMP *rtmp=RTMP_Alloc();
	RTMP_Init(rtmp);
	//set connection timeout,default 30s
	rtmp->Link.timeout=10;	
	// HKS's live URL
	if(!RTMP_SetupURL(rtmp,argv[1]))
	{
		RTMP_Log(RTMP_LOGERROR,"SetupURL Err\n");
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}
	if (bLiveStream){
		rtmp->Link.lFlags|=RTMP_LF_LIVE;
	}
	
	//1hour
	RTMP_SetBufferMS(rtmp, 3600*1000);		
	
	if(!RTMP_Connect(rtmp,NULL)){
		RTMP_Log(RTMP_LOGERROR,"Connect Err\n");
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}

	if(!RTMP_ConnectStream(rtmp,0)){
		RTMP_Log(RTMP_LOGERROR,"ConnectStream Err\n");
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}


	int frame_sec = 0;
	int frame_avg = 0;
	int frame_all = 0;
	int frame_avg_ptime = 0;
	
	int64_t cur_time_usec = 0;
	int64_t last_time_usec = 0;
	int64_t start_time_usec = 0;
	int64_t cost_time_sec = 0;

	int frame_array[MAX_COUNT_AVG_FRAME_TIME];
	int count_time = COUNT_TIME_D;

	start_time_usec = OSA_getCurTimeInUsec();

	for(i = 0; i < count_time; i++)
		frame_array[i] = 0;

	while(nRead=RTMP_Read(rtmp,buf,bufsize)){

		cur_time_usec = OSA_getCurTimeInUsec();

		// video
		if(0x09 == buf[0])
		{
			frame_sec++;
			frame_all++;

			/*
			if(cur_time_usec/1000000 != last_time_usec/1000000)
			{
				last_time_usec = cur_time_usec;
				cost_time_sec = (cur_time_usec - start_time_usec)/1000000 + 1;

				frame_array[cost_time_sec%count_time] = frame_sec;
				frame_avg_ptime = 0;
				for(i = 0; i < count_time; i++)
					frame_avg_ptime += frame_array[i];
				frame_avg_ptime = frame_avg_ptime/count_time;

				OSA_print_time();
				printf("cost_time:%lld\tcur_fps:%d\t%ds_avg_fps=%d\tavg_fps:%d\ttotal_frames:%d\n",
					cost_time_sec,frame_sec,count_time,frame_avg_ptime,frame_all/cost_time_sec,frame_all);
				frame_sec = 0;

				//if(abs(frame_avg_ptime - (frame_all/cost_time_sec)) > 4)
				//	printf("-----------------------------------------------------------------------\n");
			}
			*/

			if((cur_time_usec/1000000 - last_time_usec/1000000) >= count_time)
			{
				last_time_usec = cur_time_usec;
				cost_time_sec = (cur_time_usec - start_time_usec)/1000000 + 1;

				frame_avg_ptime = frame_sec/count_time;

				OSA_print_time();
				printf("cost_time:%lld\t%ds_avg_fps=%d\tavg_fps:%d\ttotal_frames:%d\n",
					cost_time_sec,count_time,frame_avg_ptime,frame_all/cost_time_sec,frame_all);
				
				frame_sec = 0;
			}

		}			
	}

	if(buf){
		free(buf);
	}

	if(rtmp){
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		CleanupSockets();
		rtmp=NULL;
	}	
	return 0;
}

