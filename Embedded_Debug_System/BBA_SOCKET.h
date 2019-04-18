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
	int SetRemote(char* addrTarge,short PORT);	//����Ŀ��
	int SetRemote(sockaddr_in addrTarge, short PORT);	//����Ŀ��
	int BindLocal(char* addrLocal,short PORT);
	int Send(char *bytes, unsigned int len);
	int Recvfrom(char *buf);
	long OnAP_ServerMsg(WPARAM wParam, LPARAM lParam);
	~BBA_UDP();

private:
	int GetLocalName_IP();

public:
	SOCKET m_socket;
	sockaddr_in addrLocal;			//�󶨵ı���IP
	sockaddr_in addrTarge;			//������ϢĿ�ĵ�ַ
	sockaddr_in addrRemote;			//�յ���Ϣ��һ��
	char localIP[17];	//����IP
	char localName[50];	//��������
	int nServerAddrLen = sizeof(sockaddr_in);

};
#endif