/*************************************************************************
    > File Name: tcpsrv.cpp
    > Author: tww
    > Mail: 331397964@qq.com
    > Created Time: Tue 22 Jul 2014 04:58:04 PM PDT
 ************************************************************************/

#include<iostream>
#include <stdio.h>
#include "gpio.h"
#include "get_luv.h"
#include <sys/prctl.h>
#include <signal.h>
#include <sys/time.h>

using namespace std;
enum ctrl{
low_forw,
mid_forw,
qui_forw,
low_back,
mid_back,
qui_back,
turn_left,
turn_right,
stop,
photo,
steer_engine
};



void steer_eng(int angle,motor pwm);

void start(motor led)
{
	usleep(10);
	led.writeGPIO(TRIG,1);
	usleep(15);
	led.writeGPIO(TRIG,0);
}
double get_distance(motor led,int fd)
{
	double distance;
	struct timeval tv1,tv2;
	int value;
	unsigned long interval;
	start(led);
	for(unsigned long i=0;i<10000;i++)
	{
		
		value = led.readGPIO(ECHO,fd);
		if(value == 1)
		{
			gettimeofday(&tv1,0);
			while(1)
			{
				value = led.readGPIO(ECHO,fd);
				if(value == 0)
				{
					gettimeofday(&tv2,0);
					interval=tv2.tv_usec-tv1.tv_usec;
					distance=(double)interval*0.0340/2;
					cout << " distance = "<<distance<<"cm"<<endl;
					return distance;
				}
			}
		}
	}
}
int mystrcmp(const char *str1,const char *str2)
{
	if(str1[0]==str2[0]  && str1[1] == str2[1])
		return 0;
	else
		return 1;
}
int analyze_cmd(const char * buf )
{
    if(mystrcmp(buf,"11") == 0)
        return 1;
    if(mystrcmp(buf,"12") == 0)
        return 2;
    if(mystrcmp(buf,"13") == 0)
        return 3;
    if(mystrcmp(buf,"21") == 0)
        return 4;
    if(mystrcmp(buf,"22") == 0)
        return 5;
    if(mystrcmp(buf,"23") == 0)
        return 6;
    if(mystrcmp(buf,"33") == 0)
        return 7;
    if(mystrcmp(buf,"44") == 0)
        return 8;
    if(mystrcmp(buf,"55") == 0)
        return 9;
    if(mystrcmp(buf,"66") == 0)
        return 10;
    if(mystrcmp(buf,"77") == 0)
        return 11;
}
void exec_cmd(int cmd,motor pwm)
{
static int angle=0;
    switch (cmd)
    {
        case 1: {cout<<"low_speed"		<<endl;pwm.line ();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);}
    break;
        case 2: {cout<<"mid_speed"		<<endl;pwm.line ();pwm.setpwm_duty(3,mid);pwm.setpwm_duty(5,mid);}
    break;
        case 3: {cout<<"qui_speed"		<<endl;pwm.line ();pwm.setpwm_duty(3,hig);pwm.setpwm_duty(5,hig);}
    break;
        case 4: {cout<<"low_speed bb "	<<endl;pwm.back ();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);}
    break;
        case 5: {cout<<"mid_speed bb "	<<endl;pwm.back ();pwm.setpwm_duty(3,mid);pwm.setpwm_duty(5,mid);}
    break;
        case 6: {cout<<"qui_speed bb "	<<endl;pwm.back ();pwm.setpwm_duty(3,hig);pwm.setpwm_duty(5,hig);}
    break;
        case 8: {cout<<"ledt"			<<endl;pwm.left ();pwm.setpwm_duty(3,mid);pwm.setpwm_duty(5,mid);}
    break;
        case 7: {cout<<"righ"			<<endl;pwm.right();pwm.setpwm_duty(3,mid);pwm.setpwm_duty(5,mid);}
    break;
        case 9: {cout<<"stop"			<<endl;pwm.stop ();pwm.setpwm_duty(3,mid);pwm.setpwm_duty(5,mid);}
    break;
        case 10:{cout<<"photo"			<<endl;}
    break;
        case 11:{cout<<"strreing"		<<endl;angle+=45;if(angle>180)angle=0;steer_eng(angle,pwm);}
    break;
        default:
    break;
    }
}
void steer_eng(int angle,motor pwm)
{
	switch(angle)

	{
		case 0  :pwm.setpwm_duty(7,100000);break;			
		case 45 :pwm.setpwm_duty(7,low);break;
		case 90 :pwm.setpwm_duty(7,mid);break;
		case 135:pwm.setpwm_duty(7,hig);break;
		case 180:pwm.setpwm_duty(7,100000);break;
	}
  }

void sig_pro(pid_t pid)
{	
	close(9997);
	close(9998);
	kill(100,pid);
	exit(0);
}
void sig_child(int m_nSockfd)
{
	close(m_nSockfd);
	close(9998);
	exit(0);
}


int main(int argc, char ** argv)
{
    int cmd;
	double dis1,dis2,dis;
    char *img=new char[76800];
	ServerSock mServer,mServer2;
	ClientSock mClient,mClient2;
	motor pwm;
	pwm.pwm(3,mid);
	pwm.pwm(5,mid);
	pwm.pwm(7,1500000);	
	pwm.initgpio();
	pwm.initGPIO_in(ECHO,2);
	pwm.initGPIO(TRIG);
	
	int fd;
	fd=pwm.openGPIO(ECHO);
	if(fd == -1)
	{
		return -1;
	}

	mServer.Create(false);
    mServer2.Create(false);

	mServer.Bind(SERVER_PORT);
    mServer2.Bind(SERVER_PORT2);

	mServer.Listen();
    mServer2.Listen();
	
	sighandler_t ret;
	ret=signal(SIGINT,sig_pro);

	init_video();
    pid_t fpid;
	bool err;
    fpid=fork();
    if (fpid < 0)
        printf("error in fork!");
	
    else if (fpid == 0) 
	{
		prctl(PR_SET_PDEATHSIG,SIGKILL);

   		err=mServer2.Accept(mClient2);
		
		sighandler_t ret1;
		ret1=signal(100,sig_child);

		if( err == false)
			cout<<"9997 conected failed  "<<endl;		
		else
			cout<<"9997 sucess conected "<<endl;   
		while(1)
	    {
				char buf[BUFFER_SIZE];

				v4l2_grab(mClient2);
		//		mClient.Recv(buf, BUFFER_SIZE);
				if(getppid()==1)
				{
					close_video();
					mClient2.Close();
					
					exit(0);

				}
	    }

    }
    else
    {
		err = mServer.Accept(mClient);
		if( err == false)
			cout<<"9998 sucess failed  "<<endl;		
		else
			cout<<"9998 sucess conected "<<endl;
        while(1)
	    {
			
			dis=get_distance(pwm,fd);
			if(dis>20)
			{
				
				pwm.pwm(7,500000);	//转动舵机90
				sleep(1);
				dis1=get_distance(pwm,fd);//测距
				pwm.setpwm_duty(7,2500000);//舵机归位
				sleep(1);
				dis2=get_distance(pwm,fd);//测距			
				pwm.pwm(7,1500000);
				
				if(dis1<13)
				{
					pwm.line ();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);
					usleep(200000);
					pwm.left();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);
					usleep(400000);
				}
				else
				if(dis2<13)
				{
					pwm.line ();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);
					usleep(200000);
					pwm.right ();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);	
					usleep(400000);
				}
				else
				{
					pwm.back ();pwm.setpwm_duty(3,2000000);pwm.setpwm_duty(5,low);
					usleep(500000);
				}
			}
			/* else
			if(dis<13)
			{
				pwm.line ();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);
				usleep(200000);
			} */
			else
			{
				pwm.stop ();
				pwm.pwm(7,500000);	//转动舵机90
				sleep(1);
				dis1=get_distance(pwm,fd);//测距
				pwm.setpwm_duty(7,2500000);//舵机归位
				sleep(1);
				dis2=get_distance(pwm,fd);//测距
				
				pwm.pwm(7,1500000);//舵机归位
				
				
				pwm.line ();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);
				usleep(200000);
				if(((dis1-dis2)>10))					//右边障碍
				{
					pwm.right ();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);	
					usleep(300000);
				}
				else
				{
					pwm.left();pwm.setpwm_duty(3,low);pwm.setpwm_duty(5,low);
					usleep(300000);
				}
	
			}
			
			usleep(200000);
			pwm.stop ();
			usleep(200000);
	    }   	
		
    }


	return 0;

}

