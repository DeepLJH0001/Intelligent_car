#ifndef _GET_LUV_
#define _GET_LUV_
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/videodev2.h>




#include <string.h>
#include <assert.h>
#include <getopt.h> 


#include <errno.h>
#include <malloc.h>


#include <sys/time.h>


#include <asm/types.h>

#include "mySocket.h"
#ifndef __GET_LUV_H__
#define __GET_LUV_H__


#include <stdio.h>

 
 

#define  FREE(x)       if((x)){free((x));(x)=NULL;}


struct buffer
{
	void * start;
	unsigned int length;
};




#define  TRUE	1
#define  FALSE	0

#define FILE_VIDEO 	"/dev/video0"
#define BMP      	"image.bmp"
#define YUV		"image_yuv.yuv"

#define  IMAGEWIDTH    160  //修改1
#define  IMAGEHEIGHT   120	//修改2


typedef unsigned char  BYTE;
typedef unsigned short	WORD;
typedef unsigned long  DWORD;
#define GetY(R,G,B) (0.257*(R)+0.504*(G)+0.98*(B)+16)
#define GetU(R,G,B) (0.148*(R)-0.291*(G)+0.439*(B)+128)
#define GetV(R,G,B) (0.439*(R)-0.368*(G)-0.071*(B)+128)
/**/
#pragma pack(1)

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

int init_video(); 
int get_yuv();
int close_video();
int v4l2_grab(ClientSock client);
//if successful return 1,or else return 0
int openVideo();
int closeVideo();

void sobel(char src[],char dst[],int widdepth);
void imgage_mean(char src[],char dst[]);
//data 用来存储数据的空间, size空间大小
void getVideoData(unsigned char *data, int size);
 
#if defined(__cplusplus)
}

#endif


#endif //__V4LGRAB_H___

#endif
