#ifndef _BASESOCKET_H
#define _BASESOCKET_H

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256
#define WELCOME_MESSAGE "Welcome to server!"
#define SERVER_PORT 9998
#define SERVER_PORT2 9997
#define CLIENT_PORT 20001

using namespace std;

class BaseSock
{
protected:
	bool m_bUDP;
	bool m_bConnected;
	char m_cHost[BUFFER_SIZE];
	unsigned short m_nPort;
	int m_nSockfd;

public:
	BaseSock();
	virtual ~BaseSock();

	bool Create(bool bUDP=false);
	virtual bool Bind(unsigned short nPort);
	virtual void Close();

	virtual bool Send(const char *buf, long buflen);
	virtual long Recv(char *buf, long buflen);
	virtual bool Sendto(const char *buf, int len, const struct sockaddr_in* toaddr);
	virtual long RecvFrom(char *buf, int len, struct sockaddr_in *fromaddr);
};

class ClientSock :public BaseSock
{
public:
	ClientSock();
	virtual ~ClientSock();

	virtual bool Connect(const char *host, unsigned short port);

	friend class ServerSock;
};

/* 接下来完成ServerSock类的派生
 * 需要加入的有Listen与Accept成员函数
 * 对RecvFrom、SendTo进行重新定义
 *
 * Listen、Accept函数的声明如下：
 *
 * virtual bool Listen();
 * virtual bool Accept(Client &client);
 */
class ServerSock :public BaseSock
{
public:
	ServerSock();
	virtual ~ServerSock();
	friend class ClientSock;
    virtual bool Listen();
    virtual bool Accept(ClientSock &client);
	virtual bool Sendto(const char *buf, int len, const struct sockaddr_in* toaddr);
	virtual long RecvFrom(char *buf, int len, struct sockaddr_in *fromaddr);

};

#endif
