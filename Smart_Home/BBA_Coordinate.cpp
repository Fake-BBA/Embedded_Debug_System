#include "stdafx.h"
#include "BBA_Coordinate.h"

BBA_CoordinateWaveform::BBA_CoordinateWaveform() 
{
	endPoint.x = 1400;
	endPoint.y = 222;

	startPoint.x = 0;
	startPoint.y = 222;

	for (int i = 0; i < MAX_POINT; i++)
	{
		point[i].x = endPoint.x;
		point[i].y = endPoint.y;
	}
}

BBA_CoordinateWaveform::~BBA_CoordinateWaveform() {};

BBA_CoordinateWaveform::BBA_CoordinateWaveform(BBA_CoordinateWaveform &p)
{
	memcpy(point, p.point, MAX_POINT);
	startPoint=p.startPoint;	//起始点
	endPoint=p.endPoint;		//最新的点
}

BBA_CoordinateWaveform BBA_CoordinateWaveform:: operator=(const BBA_CoordinateWaveform &p)
{
	//if (this != &p)
	{
		memcpy(point, p.point, MAX_POINT);
		startPoint = p.startPoint;	//起始点
		endPoint = p.endPoint;		//最新的点
	}
	return *this;
}

int BBA_CoordinateWaveform::CreatWaveform(int x, int y)
{
	point[MAX_POINT - 1].x = endPoint.x;	//刷新点位置固定
	
	//Y轴的点往左移动响应距离
	for (int i = 0; i < MAX_POINT; i++)
	{
		point[i].y = point[i + 1].y;		
	}
	//先计算每一个相邻点的距离
	int distanceX[MAX_POINT];
	for (int i = MAX_POINT-1; i >= 0; i--)
	{
		distanceX[i] = point[i + 1].x - point[i].x;
	}
	//加入新点
	point[MAX_POINT - 2].x = point[MAX_POINT - 1].x - x;
	//重新计算每个相邻点的距离=前一个新点的距离-前一个新点与该点的距离
	for (int i = MAX_POINT - 3; i >= 0; i--)
	{
		point[i].x = point[i + 1].x - distanceX[i+1];
	}
	point[MAX_POINT - 1].y = endPoint.y + y;

	return 0;
}

int BBA_CoordinateWaveform::DrawWaveform(CDC *pDC, int nWidth)
{
	pen.CreatePen(PS_SOLID, nWidth, colour);
	pDC->SelectObject(pen);
	pDC->Polyline(point, MAX_POINT);	//画出波形
	pen.DeleteObject();
	return 0;
}

BBA_Coordinate::BBA_Coordinate() 
{

	m_CoordinateCenter_X = 0;
	m_CoordinateCenter_Y = 0;
	m_unitOneX = 40;	//40个像素代表X轴的一个小格
	m_unitOneY = 40;	//40个像素代表Y轴的一个小格
	m_uintLen=5;	//刻度的长度

	m_ListColor[0] = RGB(255, 0, 0);
	m_ListColor[1] = RGB(255, 125, 0);
	m_ListColor[2] = RGB(255, 255, 0);
	m_ListColor[3] = RGB(0, 255, 0);
	m_ListColor[4] = RGB(0, 255, 255);
	m_ListColor[5] = RGB(0, 0, 255);
	m_ListColor[6] = RGB(255, 0, 125);
	m_ListColor[7] = RGB(125, 0, 255);
	m_ListColor[8] = RGB(255, 125, 255);

	width = 1;	//坐标轴粗细
	colour = (0, 0, 0);	//坐标轴为黑色

	m_CheckBtnHeigh = 20;	//复选框高度为20
	m_CheckBtnWidth = 65;	//复选框长度为65
}

BBA_Coordinate::~BBA_Coordinate() {};

int BBA_Coordinate::CreatCoordinateWindow(CString wndTitle, CRect *rect, CWnd* pParent, DWORD id, DWORD style)
{
	unsigned long int myStyle = WS_SYSMENU;
	int ret = CreatWindow(wndTitle, rect, pParent, id, myStyle);
	return ret;
}

BEGIN_MESSAGE_MAP(BBA_Coordinate, BBA_CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER(IDC_TIMER_WAVEFORM, &BBA_Coordinate::OnTimer(UINT_PTR nIDEvent))
	ON_BN_CLICKED(IDC_BTN_REFLASH, &BBA_Coordinate::OnBtnReflashClick)
	ON_COMMAND_RANGE(IDC_CHECK_BTN_START+1, IDC_CHECK_BTN_START + LIST_NUM, OnCheckButton)

	ON_WM_CREATE()
END_MESSAGE_MAP()

void BBA_Coordinate::OnTimer(UINT_PTR nIDEvent)
{
	int n=waveformList.GetCount();
	for (int i = 0; i < n; i++)
	{
		POSITION pos = waveformList.FindIndex(i);
		waveformList.GetAt(pos).CreatWaveform(5, rand() % 100 - 50);
	}
	Invalidate();	//发出重绘消息OnPaint
}

int BBA_Coordinate::DrawCoordinate(CDC *pDC)
{
	pen.CreatePen(PS_SOLID, width, colour);
	pDC->SelectObject(&pen);
	

	m_StartX.x = 0;
	m_StartX.y= m_RectClientWindow.Height() / 2 + m_CoordinateCenter_X;
	m_EndX.x = m_RectClientWindow.Width();
	m_EndX.y = m_StartX.y;

	m_StartY.x = m_RectClientWindow.Width()/2+ m_CoordinateCenter_Y;
	m_StartY.y = 0;
	m_EndY.x = m_StartY.x;
	m_EndY.y = m_RectClientWindow.Height();
	//画X轴
	pDC->MoveTo(m_StartX);
	pDC->LineTo(m_EndX);

	//画Y轴
	pDC->MoveTo(m_StartY);
	pDC->LineTo(m_EndY);

	//画X轴刻度
	for (int i = 0; i < m_RectClientWindow.Width() / m_unitOneX; i++)
	{
		pDC->MoveTo(m_StartX.x+ i*m_unitOneX,m_StartX.y);
		pDC->LineTo(m_StartX.x + i * m_unitOneX, m_StartX.y- m_uintLen);
	}

	//画Y轴刻度
	for (int i = 0; i < m_RectClientWindow.Height() / m_unitOneY; i++)
	{
		pDC->MoveTo(m_StartY.x , m_StartY.y + i * m_unitOneY);
		pDC->LineTo(m_StartY.x + m_uintLen, m_StartY.y + i * m_unitOneY);
	}
	pen.DeleteObject();
	return 0;
}

void BBA_Coordinate::OnPaint()
{
	//获得可绘制屏幕区域大小
	this->GetClientRect(&m_RectClientWindow);
	
	m_pPDC = new CPaintDC(this);
	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象

	//随后建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(NULL);
	//这时还不能绘图，因为没有地方画 ^_^
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
	MemBitmap.CreateCompatibleBitmap(m_pPDC, m_RectClientWindow.right, m_RectClientWindow.bottom-m_CheckBtnHeigh);
	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	//先用背景色将位图清除干净，这里我用的是白色作为背景
	//你也可以用自己应该用的颜色
	MemDC.FillSolidRect(0, 0, m_RectClientWindow.Width(), m_RectClientWindow.Height(), RGB(255, 255, 255));
	//绘图
	int n = waveformList.GetCount();
	for (int i = 0; i < n; i++)
	{
		POSITION pos = waveformList.FindIndex(i);
		waveformList.GetAt(pos).endPoint.x = m_RectClientWindow.right;
		waveformList.GetAt(pos).endPoint.y = m_RectClientWindow.bottom / 2;
		waveformList.GetAt(pos).DrawWaveform(&MemDC,1);	//画出波形
	}
	DrawCoordinate(&MemDC);		//画出坐标轴
	//将内存中的图拷贝到屏幕上进行显示
	m_pPDC->BitBlt(0, 0, m_RectClientWindow.Width(), m_RectClientWindow.Height(), &MemDC, 0, 0, SRCCOPY);
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();


	//delete m_pPDC;
}


void BBA_Coordinate::OnSize(UINT nType, int cx, int cy)
{
	BBA_CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


int BBA_Coordinate::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BBA_CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//创建各种按钮
	CString temp;
	temp.LoadStringA(IDS_STRING_BTN_REFLASH_START);
	m_BtReflash.Create(temp, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,CRect(0,0,100,100),this,IDC_BTN_REFLASH);
	//获得可绘制屏幕区域大小
	this->GetClientRect(&m_RectClientWindow);
	CRect rect;
	rect.top = m_RectClientWindow.bottom - m_CheckBtnHeigh;
	rect.bottom = m_RectClientWindow.bottom;	//将复选框放置到最底
	
	for (int i = 0; i < LIST_NUM; i++)
	{
		rect.left = i* m_CheckBtnWidth;
		rect.right = rect.left+ m_CheckBtnWidth;//每个复选框隔60像素
		temp.LoadStringA(IDS_STRING_BTN_PITCH+i);
		
		m_ListBtn[i].Create(temp, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, rect, this, IDC_CHECK_BTN_START +i+1);
		//m_ListBtn[i].
	
	}

	return 0;
}

void BBA_Coordinate::OnBtnReflashClick()
{
	CString title;
	CString startTitle,stopTitle;
	startTitle.LoadStringA(IDS_STRING_BTN_REFLASH_START);
	stopTitle.LoadStringA(IDS_STRING_BTN_REFLASH_STOP);

	m_BtReflash.GetWindowTextA(title);
	if (title == startTitle)
	{
		SetTimer(IDC_TIMER_WAVEFORM, 50, NULL);	//设置定时器
		m_BtReflash.SetWindowTextA(stopTitle);
	}
	else
	{
		KillTimer(IDC_TIMER_WAVEFORM);
		m_BtReflash.SetWindowTextA(startTitle);
	}
}

void BBA_Coordinate::OnCheckButton(UINT ID)
{
	CButton* pBtn = (CButton*)GetDlgItem(ID);
	int state = pBtn->GetCheck();
	static POSITION pos[LIST_NUM];

	int waveformPos = ID-IDC_CHECK_BTN_START - 1;

	if (state == 1)
	{
		//创建波形图
		BBA_CoordinateWaveform *waveform = new BBA_CoordinateWaveform();
		//waveform->colour = m_ListColor[waveformPos];
		waveformList.AddTail(*waveform);
		pos[waveformPos] = waveformList.GetTailPosition();
		waveformList.GetAt(pos[waveformPos]).colour= m_ListColor[waveformPos];
	}
	else
	{
		waveformList.RemoveAt(pos[waveformPos]);
	}
}