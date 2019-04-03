#include "stdafx.h"
#include "BBA_SOCKET.h"

BBA_UDP::BBA_UDP()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//printf("WSAStartup failed\n");
		//return -1;
	}
	GetLocalName_IP();
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		//printf("create socket failed\n");
		//return -1;
	}
}

BBA_UDP::BBA_UDP(char *ipaddress, short PORT)
{
	//初始化网络环境  
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//printf("WSAStartup failed\n");
		//return -1;
	}
	GetLocalName_IP();
	//建立一个UDP的socket  
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		//printf("create socket failed\n");
		//return -1;
	}
	SetRemote(ipaddress, PORT);

}

BBA_UDP::BBA_UDP(char *local_address, short local_port, char *remote_address, short remote_port)
{
	//初始化网络环境  
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//printf("WSAStartup failed\n");
		//return -1;
	}
	GetLocalName_IP();
	//建立一个UDP的socket  
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		//printf("create socket failed\n");
		//return -1;
	}
	SetRemote(remote_address, remote_port);
	BindLocal(local_address, local_port);
}

BBA_UDP::~BBA_UDP()
{
	WSACleanup();
}

int BBA_UDP::Init(char *local_address, short local_port, char *remote_address, short remote_port)
{
	//建立一个UDP的socket  
	SetRemote(remote_address, remote_port);
	BindLocal(local_address, local_port);
	return 1;
}


int BBA_UDP::SetRemote(char* ipaddress, short PORT)
{
	memset(&addrTarge, 0x00, sizeof(addrTarge));
	addrTarge.sin_family = AF_INET;
	addrTarge.sin_port = htons(PORT);
	addrTarge.sin_addr.S_un.S_addr = inet_addr(ipaddress);
	return 1;
}

int BBA_UDP::SetRemote(sockaddr_in addrTarge, short PORT)
{
	memset(&this->addrTarge, 0x00, sizeof(this->addrTarge));
	this->addrTarge.sin_family = AF_INET;
	this->addrTarge.sin_port = htons(PORT);
	this->addrTarge.sin_addr.S_un.S_addr = addrTarge.sin_addr.S_un.S_addr;
	return 1;
}

int BBA_UDP::BindLocal(char* ipaddress, short PORT)
{
	memset(&addrLocal, 0x00, sizeof(addrTarge));
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port = htons(PORT);
	addrLocal.sin_addr.S_un.S_addr = inet_addr(ipaddress);
	bind(m_socket,(sockaddr*)&addrLocal, sizeof(sockaddr));
	return 1;
}

int BBA_UDP::Send(char *bytes, unsigned int len)
{
	sendto(m_socket, bytes, len, 0, (SOCKADDR *)&addrTarge, sizeof(SOCKADDR));
	int ret2 = WSAGetLastError();
	return ret2;
}

int BBA_UDP::Recvfrom(char *buf)
{
	memset(&addrRemote, 0x00, sizeof(addrRemote));
	return recvfrom(m_socket, buf, 512, 0, (SOCKADDR *)&addrRemote, &nServerAddrLen);
	
}

long BBA_UDP::OnAP_ServerMsg(WPARAM wParam, LPARAM lParam)
{
	return 1;
}

int BBA_UDP::GetLocalName_IP()
{
	gethostname(localName, sizeof(localName));// 获得本机主机名.
	hostent* hn;
	hn = gethostbyname(localName);//根据本机主机名得到本机ip
	CString temp = inet_ntoa(*(in_addr *)hn->h_addr_list[0]);//把ip换成字符串形式
	strcpy(localIP, temp);

	return 1;
}