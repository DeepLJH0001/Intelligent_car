#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "get_luv.h"
#include <iostream>

using namespace std;
// 最大数据大小
#define MAXDATASIZE 1000000
// 服务器监听端口
#define SERVERPORT 9997
// 最大连接数
#define MAXCONN_NUM 10

static   int      fd;
static   struct   v4l2_capability   cap;
struct v4l2_fmtdesc fmtdesc;
struct v4l2_format fmt,fmtack;
struct v4l2_streamparm setfps;  
struct v4l2_requestbuffers req;
struct v4l2_buffer buf;
enum v4l2_buf_type type;
unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3];
char picture[38400];  //修改3

struct buffer* buffers;
 
int init_v4l2(void)
{
	int i;
	int ret = 0;
	
	//opendev
	if ((fd = open(FILE_VIDEO, O_RDWR)) == -1) 
	{
		printf("Error opening V4L interface\n");
		return (FALSE);
	}

	//query cap
	if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) 
	{
		printf("Error opening device %s: unable to query device.\n",FILE_VIDEO);
		return (FALSE);
	}
	else
	{
     	printf("driver:\t\t%s\n",cap.driver);
     	printf("card:\t\t%s\n",cap.card);
     	printf("bus_info:\t%s\n",cap.bus_info);
     	printf("version:\t%d\n",cap.version);
     	printf("capabilities:\t%x\n",cap.capabilities);
     	
     	if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) 
     	{
			printf("Device %s: supports capture.\n",FILE_VIDEO);
		}

		if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) 
		{
			printf("Device %s: supports streaming.\n",FILE_VIDEO);
		}
	} 
	
	//emu all support fmt
	fmtdesc.index=0;
	fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("Support format:\n");
	while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
	{
		printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
		fmtdesc.index++;
	}
	
    //set fmt
  	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.height =IMAGEHEIGHT; 
	fmt.fmt.pix.width = IMAGEWIDTH;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	
	if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
	{
		printf("Unable to set format\n");
		return FALSE;
	} 	
	if(ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
	{
		printf("Unable to get format\n");
		return FALSE;
	} 
	{
     	printf("fmt.type:\t\t%d\n",fmt.type);
     	printf("pix.pixelformat:\t%c%c%c%c\n",fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF,(fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
     	printf("pix.height:\t\t%d\n",fmt.fmt.pix.height);
     	printf("pix.width:\t\t%d\n",fmt.fmt.pix.width);
     	printf("pix.field:\t\t%d\n",fmt.fmt.pix.field);
	}
	//set fps
	setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	setfps.parm.capture.timeperframe.numerator = 10;
	setfps.parm.capture.timeperframe.denominator = 10;
	
	printf("init %s \t[OK]\n",FILE_VIDEO);
	    
	return TRUE;
}

int v4l2_grab(ClientSock client)
{
	unsigned int n_buffers;
	bool err;	
	//request for 4 buffers 
	req.count=4;
	req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory=V4L2_MEMORY_MMAP;
	if(ioctl(fd,VIDIOC_REQBUFS,&req)==-1)
	{
		printf("request for buffers error\n");
	}

	//mmap for buffers
	buffers = new buffer[(req.count*sizeof (*buffers))];
	if (!buffers) 
	{
		printf ("Out of memory\n");
		return(FALSE);
	}
	
	for (n_buffers = 0; n_buffers < req.count; n_buffers++) 
	{
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		//query buffers
		if (ioctl (fd, VIDIOC_QUERYBUF, &buf) == -1)
		{
			printf("query buffer error\n");
			return(FALSE);
		}

		buffers[n_buffers].length = buf.length;
		//map
		buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ |PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
		if (buffers[n_buffers].start == MAP_FAILED)
		{
			printf("buffer map error\n");
			return(FALSE);
		}
	}
		
	//queue
	for (n_buffers = 0; n_buffers < req.count; n_buffers++)
	{
		buf.index = n_buffers;
		ioctl(fd, VIDIOC_QBUF, &buf);
	} 
	
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl (fd, VIDIOC_STREAMON, &type);
	int i;
	while(1)	
	{
		ioctl(fd, VIDIOC_DQBUF, &buf);

//		printf("grab yuyv OK\n");
		memcpy(&picture, buffers[1].start, IMAGEHEIGHT*IMAGEWIDTH*2);

	if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {

     	return -1;
	
	}
	
	err=client.Send("aa",2);
	if(err==false)
	{
		cout<<"TCP  disconnect"<<endl;
		exit(0);
	}
	client.Send("00038400",8);
	client.Send("160,120",7);
//	char img_mean[160*120];
	char deal_picture[160*120*2];
//	imgage_mean(picture,img_mean);
	sobel(picture,deal_picture,2);
	client.Send(deal_picture,160*120*2);
	}
	return(TRUE);
}
void imgage_mean(char src[],char dst[])
{
	int i,j;
	char temp1,temp2,temp3;
	char temp4,temp5,temp6;
	char temp7,temp8,temp9;
	int index = 0;
	
	int temp;
	for (i=0;i<120;i++)
	{

		for (j=0;j<320;j+=2)
		{
			temp5=src[i*320+j];
			temp1=src[i*320+j-322];
			temp2=src[i*320+j-320];
			temp3=src[i*320+j-318];
			
			temp4=src[i*320+j-2];
			temp5=src[i*320+j];
			temp6=src[i*320+j+2];
			
			temp7=src[i*320+j+318];
			temp8=src[i*320+j+320];
			temp9=src[i*320+j+322];

			temp=(temp1+temp2+temp3+temp4+temp5+temp6+temp7+temp8+temp9)/9;

			dst[index++] = (char)temp;
		}
	}
}
void sobel(char src[],char dst[],int widdepth)
{
	int i,j;
	char temp1,temp2,temp3;
	char temp4, temp5;
	char temp6,temp7,temp8;
	int index = 0;
	
	int tempx,tempy,temp;
	for (i=0;i<120;i++)
	{
		bool bsetU = true;
		for (j=0;j<320;j+=2)
		{
			temp =src[(i*320+j)];
			temp1=src[(i*320+j-322)];
			temp2=src[(i*320+j-320)];
			temp3=src[(i*320+j-318)];
			
			temp4=src[(i*320+j-2)];
			temp5=src[(i*320+j+2)];
			
			temp6=src[(i*320+j+318)];
			temp7=src[(i*320+j+320)];
			temp8=src[(i*320+j+322)];
			
			tempx=temp1+2*temp2+temp3-temp6-temp7*2-temp8;
			tempy=temp1+temp4*2+temp6-temp3-temp5*2-temp8;
			temp=tempx*tempx+tempy*tempy;
			bool flag=(temp>10000)?0:1;
			dst[index++] = (flag)?255:16;
			if (bsetU) //
			{
				dst[index++] = (flag)?203:128;
				bsetU = false;
			}
			else
			{
				dst[index++] = 128;
				bsetU = true;
			}
		}
	}
}
/* void sobel(char src[],char dst[],int widdepth)
{
	int i,j;
	char temp1,temp2,temp3;
	char temp4, temp5;
	char temp6,temp7,temp8;
	int index = 0;
	
	int tempx,tempy,temp;
	for (i=0;i<120;i++)
	{
		bool bsetU = true;
		for (j=0;j<160;j++)
		{
			temp =src[(i*160+j)*widdepth];
			temp1=src[(i*160+j-161)*widdepth];
			temp2=src[(i*160+j-160)*widdepth];
			temp3=src[(i*160+j-159)*widdepth];
			
			temp4=src[(i*160+j-1)*widdepth];
			temp5=src[(i*160+j+1)*widdepth];
			
			temp6=src[(i*160+j+159)*widdepth];
			temp7=src[(i*160+j+160)*widdepth];
			temp8=src[(i*160+j+161)*widdepth];
			
			tempx=temp1+2*temp2+temp3-temp6-temp7*2-temp8;
			tempy=temp1+temp4*2+temp6-temp3-temp5*2-temp8;
			temp=tempx*tempx+tempy*tempy;
			bool flag=(temp>10000)?0:1;
			dst[index++] = (flag)?255:16;
			if (bsetU) //
			{
				dst[index++] = (flag)?203:128;
				bsetU = false;
			}
			else
			{
				dst[index++] = 128;
				bsetU = true;
			}
		}
	}
} */


int close_v4l2(void)
{
     if(fd != -1) 
     {
         close(fd);
         return (TRUE);
     }
     return (FALSE);
}


int init_video()
{
   FILE * fp1,* fp2;

    fp1 = fopen(BMP, "w");
    if(!fp1)
	{
		printf("open "BMP"error\n");
		return(FALSE);
	}
	
	fp2 = fopen(YUV, "w");
    if(!fp2)
	{
		printf("open "YUV"error\n");
		return(FALSE);
	}

	if(init_v4l2() == FALSE) 
	{
     	return(FALSE);
	}
return(TRUE);
}




int close_video()
{
   
 
    close_v4l2();
 	return 0;
}







static int end_i=0;

