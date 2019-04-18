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
	/******************text ���****************/
#define STATIC_MAX_LINE_TEXT_NUM	10	//ÿһ�������
#define STATIC_MAX_LIST_TEXT_NUM	10	//ÿһ��
#define STATIC_MAX_STRLENGTH	50	//���������50���ַ�

	CPoint textStartPoint;
	CPoint textPoint[STATIC_MAX_LINE_TEXT_NUM][STATIC_MAX_LIST_TEXT_NUM];	//TEXT�����λ��
	char textStr[STATIC_MAX_LINE_TEXT_NUM][STATIC_MAX_LIST_TEXT_NUM][STATIC_MAX_STRLENGTH];
	int textLineNum;	//ÿһ�е�text��Ŀ
	int textListNum;	//ÿһ�е�text��Ŀ
	int textLineInterval;	//ÿһ��Text�ļ��
	int textListInterval;	//ÿһ��Text�ļ��
	/******************************************/
	/******************Slider���****************/
#define STATIC_MAX_LINE_SLIDER_NUM	2	//ÿһ�������
#define STATIC_MAX_LIST_SLIDER_NUM	15	//ÿһ��
#define SLIDER_NUM	STATIC_MAX_LINE_SLIDER_NUM+STATIC_MAX_LIST_SLIDER_NUM
#define IDC_SLIDER_MSG_START	3000	//��ϢID��ʼֵ
	CPoint sliderStartPoint;	//�������
	CPoint sliderPoint[STATIC_MAX_LINE_SLIDER_NUM][STATIC_MAX_LIST_SLIDER_NUM];
	int sliderLineNum;	//ÿһ�е�slider��Ŀ
	int sliderListNum;	//ÿһ�е�slider��Ŀ
	int sliderLineInterval;	//ÿһ��Slider�ļ��
	int sliderListInterval;	//ÿһ��Slider�ļ��
	int sliderLength;	//��������
	int sliderWidth;	//�������
	CSliderCtrl slider[STATIC_MAX_LINE_SLIDER_NUM][STATIC_MAX_LIST_SLIDER_NUM];

	struct SendData sendData;
	/******************************************/
	CPaintDC *m_pPDC;	//
	CRect m_RectClientWindow;	//�ɻ��Ƶ�����,��OnPaint��Ϣʵʱ����
	BBA_UDP *bba_Station_udp;	//UDP�����ָ��
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
