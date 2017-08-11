/***********************************************
课程：暑期训练营
作者:lc
*********************************************/
#ifndef _GPIO_H_
#define _GPIO_H_
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>                                                                                                                                    
#include<sys/types.h>                                                                                                                                  
#include<sys/stat.h>                                                                                                                                     
#define ERROR (-1)
#define en1 38
#define en2 40
#define en3 13
#define en4 14
#define ECHO  11
#define TRIG  12

#define whole 	20000000
#define low   	 3000000
#define mid   	 7000000
#define hig 	15000000
using namespace std;
class motor
{
	private:	
		
		
		
		int configureGPIO(int gpio,const char *in_out);//伽利略二代pwm端口和gpio的端口初始化时不一样的，具体看GPIO映像图；
		int enablePWM(int pwm_io);
		int setpwm_period(int period_val);//伽利略2代，所有PWM共享周期通道
		
		int initPWM(int pwm_io);
	public:
		int setpwm_duty(int pwm_io,int duty_val);	
		int writeGPIO(int gpio,int value);
		int pwm(int pwm_io,int duty);
		void initgpio(void); 
		void line(void);
		void back(void);
		void right(void);
		void left(void);
		void stop(void);
		int initGPIO_in(int IO,int dir);
		int openGPIO(int gpio);
		int closeGPIO(int fd);
		int readGPIO(int gpio,int fd);
		int initGPIO(int IO);

};
#endif
