#! /bin/sh 
g++ gpio.cpp gpio.h mySocket.cpp mySocket.h tcpsrv.cpp get_luv.cpp get_luv.h -o led
./led
