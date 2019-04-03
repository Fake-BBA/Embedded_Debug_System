#pragma once
#include "BBA_CWnd.h"
#include "resource.h"
#include <afxtempl.h>
#include "BBA_SOCKET.h"
#include "smartHomeTypedef.h"

enum FlightFuntionWord
{
	VER = 0,
	STATUS=1,
	SENSER=2,
	RCDATA=3,
	GPSDATA=4,
	POWER=5,
	MOTO=6,
	SENSER2=7,
	LOOP_TIME=200,
};

struct SendData {
	int16 Throttle;
	int16 Yaw;
	int16 Roll;
	int16 Pitch;
	int16 AUX1;
	int16 AUX2;
	int16 AUX3;
	int16 AUX4;
	int16 AUX5;
	int16 AUX6;
};

class BBA_FlightStatus :public BBA_CWnd {
public:
	BBA_FlightStatus();
	~BBA_FlightStatus();
protected:
	DECLARE_MESSAGE_MAP();
public:
	/******************text 相关****************/
#define STATIC_MAX_LINE_TEXT_NUM	10	//每一行最多有
#define STATIC_MAX_LIST_TEXT_NUM	10	//每一列
#define STATIC_MAX_STRLENGTH	50	//最多可以输出50个字符

	CPoint textStartPoint;
	CPoint textPoint[STATIC_MAX_LINE_TEXT_NUM][STATIC_MAX_LIST_TEXT_NUM];	//TEXT输出的位置
	char textStr[STATIC_MAX_LINE_TEXT_NUM][STATIC_MAX_LIST_TEXT_NUM][STATIC_MAX_STRLENGTH];
	int textLineNum;	//每一行的text数目
	int textListNum;	//每一列的text数目
	int textLineInterval;	//每一行Text的间隔
	int textListInterval;	//每一列Text的间隔
	/******************************************/
	/******************Slider相关****************/
#define STATIC_MAX_LINE_SLIDER_NUM	2	//每一行最多有
#define STATIC_MAX_LIST_SLIDER_NUM	15	//每一列
#define SLIDER_NUM	STATIC_MAX_LINE_SLIDER_NUM+STATIC_MAX_LIST_SLIDER_NUM
#define IDC_SLIDER_MSG_START	3000	//消息ID起始值
	CPoint sliderStartPoint;	//滑条起点
	CPoint sliderPoint[STATIC_MAX_LINE_SLIDER_NUM][STATIC_MAX_LIST_SLIDER_NUM];
	int sliderLineNum;	//每一行的slider数目
	int sliderListNum;	//每一列的slider数目
	int sliderLineInterval;	//每一行Slider的间隔
	int sliderListInterval;	//每一列Slider的间隔
	int sliderLength;	//滑条长度
	int sliderWidth;	//滑条宽度
	CSliderCtrl slider[STATIC_MAX_LINE_SLIDER_NUM][STATIC_MAX_LIST_SLIDER_NUM];

	struct SendData sendData;
	/******************************************/
	CPaintDC *m_pPDC;	//
	CRect m_RectClientWindow;	//可绘制的区域,由OnPaint消息实时更新
	BBA_UDP *bba_Station_udp;	//UDP类对象指针
public:
	int CreatFlightStatusWindow(CString wndTitle, CRect *rect = NULL, CWnd* pParent = NULL, DWORD id = 0, DWORD style = WS_VISIBLE | WS_CHILD);
	int UpdataFlightStatusWindows(char *buff,int len);
	void SendData();
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
