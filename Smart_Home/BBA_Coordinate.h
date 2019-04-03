#pragma once
#include "BBA_CWnd.h"
#include "resource.h"
#include <afxtempl.h>
class BBA_CoordinateWaveform{
public:
	BBA_CoordinateWaveform();
	~BBA_CoordinateWaveform();
	BBA_CoordinateWaveform(BBA_CoordinateWaveform &p);
	BBA_CoordinateWaveform operator=(const BBA_CoordinateWaveform &p);
public:
	CPen pen;
#define MAX_POINT	300
	POINT point[MAX_POINT];
	POINT startPoint;	//起始点
	POINT endPoint;		//最新的点
	COLORREF colour;	//波形颜色
	int CreatWaveform(int x,int y);
	int DrawWaveform(CDC *pDC,int nWidth);
	
};

class BBA_Coordinate :public BBA_CWnd {
public:
	BBA_Coordinate();
	~BBA_Coordinate();

#define IDC_TIMER_WAVEFORM	1329
protected:
	DECLARE_MESSAGE_MAP();
private:
	CPaintDC *m_pPDC;
	CList<BBA_CoordinateWaveform,BBA_CoordinateWaveform&> waveformList;
	

private:
	int DrawCoordinate(CDC *pDC);
public:
	CPen pen;	//画坐标轴专用笔
	COLORREF colour;	//坐标轴颜色
	int width;	//坐标轴粗细
	int m_CoordinateCenter_X;	//X轴与中心偏移值，0为不偏移
	int m_CoordinateCenter_Y;	//Y轴与中心偏移值，0为不偏移
	POINT m_StartX;	//X轴起点坐标
	POINT m_EndX;	//X轴终点坐标
	POINT m_StartY;	//Y轴起点坐标
	POINT m_EndY;	//Y轴终点坐标
	int m_unitOneX;	//多少个像素代表X轴的一个刻度小格
	int m_unitOneY;	//多少个像素代表Y轴的一个刻度小格
	int m_uintLen;	//刻度的长度
	CRect m_RectClientWindow;	//可绘制的区域,由OnPaint消息实时更新

public:
	int CreatCoordinateWindow(CString wndTitle, CRect *rect = NULL, CWnd* pParent = NULL, DWORD id = 0, DWORD style = WS_VISIBLE | WS_CHILD);
	void OnTimer(UINT_PTR nIDEvent);
public:
#define IDC_CHECK_BTN_START	2048
#define IDC_BTN_REFLASH	2048
	CButton m_BtReflash;	//刷新控制按钮
#define LIST_NUM	9
	CButton m_ListBtn[LIST_NUM];	//复选按钮
	COLORREF m_ListColor[LIST_NUM];	//复选按钮背景颜色
	int m_CheckBtnHeigh;			//复选按钮高度
	int m_CheckBtnWidth;			//复选按钮宽度
public:
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnReflashClick();
	afx_msg void OnCheckButton(UINT ID);
};