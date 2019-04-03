#ifndef _BBA_SOCKET_H__
#define _BBA_SOCKET_H__

#include "stdafx.h" 

#define WM_BBA_UDP_RECV	2048
class BBA_UDP
{
public:
	BBA_UDP();
	BBA_UDP(char *ipaddress , short PORT);
	BBA_UDP(char *local_address, short local_port, char *remote_address, short remote_port);
	int Init(char *local_address, short local_port, char *remote_address, short remote_port);
	int SetRemote(char* addrTarge,short PORT);	//发送目的
	int SetRemote(sockaddr_in addrTarge, short PORT);	//发送目的
	int BindLocal(char* addrLocal,short PORT);
	int Send(char *bytes, unsigned int len);
	int Recvfrom(char *buf);
	long OnAP_ServerMsg(WPARAM wParam, LPARAM lParam);
	~BBA_UDP();

private:
	int GetLocalName_IP();

public:
	SOCKET m_socket;
	sockaddr_in addrLocal;			//绑定的本地IP
	sockaddr_in addrTarge;			//发出信息目的地址
	sockaddr_in addrRemote;			//收到信息的一方
	char localIP[17];	//本机IP
	char localName[50];	//本机名字
	int nServerAddrLen = sizeof(sockaddr_in);

};
#endif