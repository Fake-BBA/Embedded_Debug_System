#pragma once

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

#define MESSAGE_PACKET_SIZE	100
//enum MessagePacketState {
//	FIRST_BYTE = 0,
//	SENDER,
//	RECEIVER,
//	FUNCTION,
//	DATA
//};
enum MessageCtrFunction {
	HEARTBEAT = 0,
	FIND_DEVICE,
	LIGHT,
	FAN,
	HUMITURE = 4,
	WIFI_TRANSPARENT = 5,		//透传模块
	GET_WIFI_IP = 253,	//获取连接上的IP
	WIFI_CONFIG = 254,
	DEVICE_ERROS = 255
};

struct MessagePacketStruct {
	uint16 sender;
	uint16 receiver;
	uint8 function_word;
	uint8 data[95];
};
union MessagePacketUnion {
	struct MessagePacketStruct messagePacket;
	uint8 p_buff[100];	//协议规定最长包数据为100
};

struct DeviceInfo {
	uint16 devicesNumber;	//设备号
	sockaddr_in address;	//对应IP
	uint8 functionWorld;	//对应功能
	uint8 isExist;			//是否存在
	uint8 loosTimes;		//隔了多少个时间单位没有响应
};

