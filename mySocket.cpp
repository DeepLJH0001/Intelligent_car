/*************************************************************************
    > File Name: mySocket.cpp
    > Author: tww
    > Mail: 331397964@qq.com
    > Created Time: Tue 22 Jul 2014 06:44:42 AM PDT
 ************************************************************************/

#include <iostream>
#include "mySocket.h"
using namespace std;

BaseSock::BaseSock()
{
	m_bUDP = false;
	m_nSockfd = 1;
	m_nPort = 0;
	m_bConnected = false;

	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		m_cHost[i] = 0;
	}
}

BaseSock::~BaseSock()
{
	cout << "~BaseSock() is called" << endl;
	Close();
}

bool BaseSock::Create(bool bUDP)
{
	m_bUDP = bUDP;
	if(!m_bUDP)
	{
		m_nSockfd = socket(AF_INET, SOCK_STREAM, 0);
	}
	else
	{
		m_nSockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if(m_nSockfd != -1)
		{
			m_bConnected = true;
		}
	}
	int on =1;
	setsockopt( m_nSockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

	return (m_nSockfd != -1);
}

bool BaseSock::Bind(unsigned short nPort)
{
	if(m_nSockfd == -1)
	{
		return false;
	}

	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.s_addr = htons(INADDR_ANY);

	if(bind(m_nSockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		cout << "bind to port" << nPort << "failure" << endl;
		return false;
	}

	return true;
}

void BaseSock::Close()
{
	close(m_nSockfd);
}

bool BaseSock::Send(const char *buf, long buflen)
{
	if(m_nSockfd == -1)
	{
		return false;
	}

	send(m_nSockfd, buf, buflen, 0);
	return true;
}

long BaseSock::Recv(char *buf, long buflen)
{
	int length = 0;

	if(m_nSockfd == -1)
	{
		return false;
	}

	length = recv(m_nSockfd, buf, buflen, 0);
	if(length < 0)
	{
		cout << "recieve error"<<length << endl;
		return false;
	}
//	cout << "recieve from IP: " << m_cHost << endl;
//	cout << "Data : " << buf << endl;
	return length;
}

bool BaseSock::Sendto(const char *buf, int len, const struct sockaddr_in *toaddr)
{
	if(m_nSockfd == -1 || len <= 0)
	{
		return false;
	}

	if(sendto(m_nSockfd, buf, len, 0, (struct sockaddr *)toaddr, sizeof(struct sockaddr_in)) != len)
	{
		cout << "sendto fail" << endl;
		Close();
	}
	return true;
}

long BaseSock::RecvFrom(char *buf, int len, struct sockaddr_in *fromaddr)
{
	int length = 0;
	socklen_t addrLen = sizeof(struct sockaddr_in);

	if((length = recvfrom(m_nSockfd, buf, len, 0, (struct sockaddr *)fromaddr,&addrLen)) < 0)
	{
		cout << "recvfrom error" << endl;
		Close();
		return -1;
	}
	else
	{
		cout << "FROM IP : " << inet_ntoa(fromaddr->sin_addr) << endl;
		cout << "DATA :" << buf << endl << endl;
	}

	return length;
}

/*
 * 在此处完成构造函数定义
 */
ServerSock::ServerSock(): BaseSock()
{

}


/*
 * 在此处完成析构函数定义
 */

ServerSock::~ServerSock()
{
	Close();
}

/*
 * 完成RecvFrom的重定义
 */
long ServerSock::RecvFrom(char *buf, int len, struct sockaddr_in *fromaddr)
{
	int length = 0;

	return length;
}

/*
 * 完成Sendto的重定义
 */
bool ServerSock::Sendto(const char *buf, int len, const struct sockaddr_in *toaddr)
{
	return true;
}

bool ServerSock::Listen()
{
	/* 完成监听函数
	 * 可以man listen查看具体的参数说明
	 * 代码如：if(listen() < 0)
	 */
if(listen(m_nSockfd ,10)<0)
	{
		cout << "call listen failure" << endl;
		return false;
	}
	return true;
}

bool ServerSock::Accept(ClientSock &client)
{
	if(m_nSockfd == -1)
	{
		return false;
	}

	struct sockaddr_in cliaddr;

	/* 此处完成接受连接请求
	 * 获取客户机的Socket文件描述符，发送数据
	 * 具体可参考man accept
	 * 如：client.m_nSockfd = accept();
	 */
socklen_t  len=sizeof(client);
	client.m_nSockfd=accept(m_nSockfd,(struct sockaddr *)&cliaddr,&len);
	client.m_bConnected = true;
	strcpy(client.m_cHost, inet_ntoa(cliaddr.sin_addr));
	client.m_nPort = ntohs(cliaddr.sin_port);

	return (client.m_nSockfd != -1);
}

ClientSock::ClientSock() : BaseSock()
{

}

ClientSock::~ClientSock()
{
	Close();
}

bool ClientSock::Connect(const char *host, unsigned short port)
{
	if(m_nSockfd == -1)
	{
		return false;
	}

	strcpy(m_cHost,host);
	m_nPort = port;

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_aton(host, &servaddr.sin_addr);

	/* 服务器的端口地址，注意网络字节序与本机字节序问题
	 * 可参考函数:htons()
	 */
	servaddr.sin_port =htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = inet_addr(host);
	/*
	 * 连接请求
	 * 可根据man connect填入适当参数
	 * 如：    if(connect())
	 */
	if(connect(m_nSockfd,(struct sockaddr *)&servaddr,sizeof(sockaddr))==0)
	{
		cout << "connect to " << host << "success"<<endl;
	}
	else
	{
		cout << "can't connect to " << host << endl;
		Close();
		return false;
	}

	m_bConnected = true;
	return true;
}
