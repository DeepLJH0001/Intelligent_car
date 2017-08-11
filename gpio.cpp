/***********************************************
课程：暑期训练营
作者:lc
*********************************************/


#include "gpio.h"
#include <iostream>

/***********************************配置GPIO ********************************************/
int motor::configureGPIO(int gpio,const char *in_out)//因为后面是直接送in或者out字符，最好定义成const char
{
	/*主要实现shell下的这三句
	echo gpio > /sys/class/gpioxx/export
	echo out > /sys/class/gpio/gpioxx/direction
	echo val > /sys/class/gpio/gpioxx/value
	*/
	char buffer[256];
    int fd;
    //创建GPIO文件在export中
    fd = open("/sys/class/gpio/export", O_WRONLY);
	if(ERROR == fd)
    {
        printf("Error: Unable to open /sys/class/gpio/export\n");
		return(-1);
    }
	sprintf(buffer, "%d", gpio);  //功能把格式化的数据写入某个字符串缓冲区
    write(fd, buffer, strlen(buffer));
    close(fd);
	
	//设置GPIO的输入输出
	if(in_out!="none")
	{
		sprintf(buffer, "/sys/class/gpio/gpio%d/direction", gpio);
		fd = open(buffer, O_WRONLY);
		if(ERROR == fd)
		{
			printf("Error: Unable to open /sys/class/gpio/gpio%d/direction\n",gpio);
			return(-1);
		}
		write(fd,in_out,3);
		close(fd);
		return 0;
	}
	
}
int motor::writeGPIO(int gpio,int value)
{
	char buffer[256];
    int fd;
	sprintf(buffer, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buffer,O_WRONLY);
    if(ERROR == fd)
    {
        printf("Error: Unable to open /sys/class/gpio/gpio%d/value\n",gpio);
        return(-1);
    }
	if(value ==  0)
    {
        // Set GPIO low status
        write(fd, "0", 1);
    }
    else
    {
        // Set GPIO high status
        write(fd, "1", 1);
    }
	close(fd);
    return(0);
}

int motor::openGPIO(int gpio)
{
	char buffer[256];
    int fd;
	int err;
	sprintf(buffer, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buffer,O_RDONLY);
    if(ERROR == fd)
    {
        printf("Error: Unable to open /sys/class/gpio/gpio%d/value\n",gpio);
        return(-1);
    }
    return(fd);
}
int motor::closeGPIO(int fd)
{
	close(fd);
}
int motor::readGPIO(int gpio,int fd)
{
	char buf[2];
	int err;
        // read GPIO  status
	err=lseek(fd,0,SEEK_SET);
    if(err == -1)
    {
    	cout << " read gpio error"<<endl;
    	return -1;
    }
    err = read(fd, buf, 1);
    if(err == -1)
    {
    	cout << " read gpio error"<<endl;
    	return -1;
    }

    return(buf[0]-'0');
}
/**********************************初始化GPIO************************************************/
/****************IO0--GPIO11--GPIO32****************************************************/
/****************IO1--GPIO12--GPIO28****************************************************/
/****************IO2--GPIO13--GPIO34****************************************************/
/****************IO3--GPIO14--GPIO16****************************************************/
/****************IO4--GPIO6--GPIO36****************************************************/
/****************IO5--GPIO0--GPIO18****************************************************/
/****************IO6--GPIO1--GPIO20****************************************************/
/****************IO7--GPIO38****************************************************/
/****************IO8--GPIO40****************************************************/
/****************IO9--GPIO4--GPIO22****************************************************/
/****************IO10--GPIO10--GPIO26****************************************************/
/****************IO11--GPIO5--GPIO24****************************************************/
/****************IO12--GPIO15--GPIO42****************************************************/
/****************IO13--GPIO7--GPIO30****************************************************/
/****************IO14--GPIO48****************************************************/

int motor::initGPIO(int IO)
{
	if(IO==11)//IO0
	{
		configureGPIO(11,"out");
		configureGPIO(32,"out");
		writeGPIO(32,0);
	}
	if(IO==12)//IO1
	{
		configureGPIO(12,"out");
		configureGPIO(28,"out");
		writeGPIO(28,0);
	}
	if(IO==13)//IO2
	{
		configureGPIO(13,"out");
		configureGPIO(34,"out");
		writeGPIO(34,0);
	}
	if(IO==14)//IO3
	{
		configureGPIO(14,"out");
		configureGPIO(16,"out");
		writeGPIO(16,0);
	}
	if(IO==6)//IO4
	{
		configureGPIO(6,"out");
		configureGPIO(36,"out");
		writeGPIO(36,0);
	}
	if(IO==0)//IO5
	{
		configureGPIO(0,"out");
		configureGPIO(18,"out");
		writeGPIO(18,0);
	}
	if(IO==1)//IO6
	{
		configureGPIO(1,"out");
		configureGPIO(20,"out");
		writeGPIO(20,0);
	}
	if(IO==38)//IO7
	{
		configureGPIO(38,"out");
	}
	if(IO==40)//IO8
	{
		configureGPIO(40,"out");
	}
	if(IO==4)//IO9
	{
		configureGPIO(4,"out");
		configureGPIO(22,"out");
		writeGPIO(22,0);
	}
	if(IO==10)//IO10
	{
		configureGPIO(10,"out");
		configureGPIO(26,"out");
		writeGPIO(26,0);
	}
	if(IO==5)//IO11
	{
		configureGPIO(5,"out");
		configureGPIO(24,"out");
		writeGPIO(24,0);
	}
	if(IO==15)//IO12
	{
		configureGPIO(15,"out");
		configureGPIO(42,"out");
		writeGPIO(42,0);
	}
	if(IO==7)//IO13
	{
		configureGPIO(7,"out");
		configureGPIO(30,"out");
		writeGPIO(30,0);
	}
	if(IO==48)//IO14
	{
		configureGPIO(48,"out");
	}
	return 0;
}
int motor::initGPIO_in(int IO,int dir)
{
	configureGPIO(IO,"in");
//	configureGPIO(32,"in");
	return 0;
}
/**********************************初始化PWM************************************************/
/****************IO3--PWM1--16--17--64****************************************************/
/****************IO5--PWM3--18--19--66****************************************************/
/****************IO6--PWM5--20--21--68****************************************************/
/****************IO9--PWM7--22--23--70****************************************************/
/****************IO10--PWM11--26--27--74**************************************************/
/****************IO11--PWM9--24--25--72***************************************************/
int motor::initPWM(int pwm_io)
{
	if(pwm_io==1)
	{
		configureGPIO(16,"out");
		configureGPIO(17,"out");
		configureGPIO(64,"none");
		writeGPIO(17,1);
		writeGPIO(64,1);		
	}
	if(pwm_io==3)
	{
		configureGPIO(18,"out");
		configureGPIO(19,"out");
		configureGPIO(66,"none");
		writeGPIO(19,1);
		writeGPIO(66,1);		
	}
	if(pwm_io==5)
	{
		configureGPIO(20,"out");
		configureGPIO(21,"out");
		configureGPIO(68,"none");
		writeGPIO(21,1);
		writeGPIO(68,1);		
	}
	if(pwm_io==7)
	{
		configureGPIO(22,"out");
		configureGPIO(23,"out");
		configureGPIO(70,"none");
		writeGPIO(70,1);
		writeGPIO(23,1);		
	}
	if(pwm_io==9)
	{
		configureGPIO(24,"out");
		configureGPIO(25,"out");
		configureGPIO(72,"none");
		writeGPIO(25,1);
		writeGPIO(72,1);		
	}
	if(pwm_io==11)
	{
		configureGPIO(26,"out");
		configureGPIO(27,"out");
		configureGPIO(74,"none");
		writeGPIO(27,1);
		writeGPIO(74,1);		
	}
	return 0;	
}
int motor::enablePWM(int pwm_io)
{
	char buffer[256];
    int fd;
	fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    if(ERROR == fd)
    {
       printf("Error: Unable to open /sys/class/pwm/pwmchip0/export\n");
       return(-1);
    }
    sprintf(buffer, "%d", pwm_io);
    write(fd, buffer, strlen(buffer));
    close(fd);
	sprintf(buffer, "/sys/class/pwm/pwmchip0/pwm%d/enable", pwm_io);
    fd = open(buffer, O_WRONLY);
    if(ERROR == fd)
    {
       printf("Error: Unable to open /sys/class/pwm/pwmchip0/pwm%d/enable\n",pwm_io);
       return(-1);
    }
    sprintf(buffer, "%d", 1);
    write(fd, buffer, strlen(buffer));
    close(fd);
	return 0;	
}
int motor::setpwm_period(int period_val)
{
	char buffer[256];
    int fd;
    fd = open("/sys/class/pwm/pwmchip0/device/pwm_period", O_WRONLY);
    if(ERROR == fd)
    {
		printf("Error: Unable to open /sys/class/pwm/pwmchip0/device/pwm_period\n");
        return(-1);
    }
	sprintf(buffer, "%d",period_val);
	write(fd,buffer,strlen(buffer));
	close(fd);
	return 0;
}
int motor::setpwm_duty(int pwm_io,int duty_val)
{
	char buffer[256];
    int fd;
	sprintf(buffer, "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", pwm_io);
    fd = open(buffer, O_WRONLY);
    if(ERROR == fd)
    {
        printf("Error: Unable to open /sys/class/pwm/pwmchip0/pwm%d/duty_cycle\n",pwm_io);
        return(-1);
    }
	sprintf(buffer, "%d", duty_val);
	write(fd, buffer, strlen(buffer));
	close(fd);
	return 0;
}
int motor::pwm(int pwm_io,int duty)
{
	initPWM(pwm_io);
	enablePWM(pwm_io);
	setpwm_period(whole);
	setpwm_duty(pwm_io,duty);
	puts("pwm---50");
	
	
	return 0;
}


void motor::initgpio(void)
{
	initGPIO(en1);	
	initGPIO(en2);
	initGPIO(en3);
	initGPIO(en4);
}
void motor::line(void)
{
	puts("line");	
	writeGPIO(en1,1);
	writeGPIO(en2,0);
	writeGPIO(en3,1);
	writeGPIO(en4,0);
}
void motor::back(void)
{
	puts("back");
	writeGPIO(en1,0);
	writeGPIO(en2,1);
	writeGPIO(en3,0);
	writeGPIO(en4,1);
}
void motor::right(void)
{
	puts("right");	
	writeGPIO(en1,0);
	writeGPIO(en2,1);
	writeGPIO(en3,0);
	writeGPIO(en4,0);
}
void motor::left(void)
{
	puts("left");	
	writeGPIO(en1,0);
	writeGPIO(en2,0);
	writeGPIO(en3,0);
	writeGPIO(en4,1);
}
void motor::stop(void)
{
	puts("stop");
	writeGPIO(en1,0);
	writeGPIO(en2,0);
	writeGPIO(en3,0);
	writeGPIO(en4,0);
}

