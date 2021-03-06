
// Smart_HomeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Smart_Home.h"
#include "Smart_HomeDlg.h"
#include "afxdialogex.h"
#include "smartHomeTypedef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartHomeDlg 对话框


CSmartHomeDlg::CSmartHomeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMART_HOME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmartHomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_CListBox_Ctrl);
	DDX_Control(pDX, IDC_SLIDER_POWER_CTRL, sliderPowerCtrl);
	DDX_Control(pDX, IDC_SLIDER2, sliderPCtrl);
	DDX_Control(pDX, IDC_SLIDER3, sliderICtrl);
	DDX_Control(pDX, IDC_SLIDER4, sliderDCtrl);
}

BEGIN_MESSAGE_MAP(CSmartHomeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_BBA_UDP_RECV, &CSmartHomeDlg::OnAP_ServerMsg)
	ON_BN_CLICKED(IDC_BUTTON_SET_WIFI, &CSmartHomeDlg::OnBnClickedButtonSetWifi)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_LOG, &CSmartHomeDlg::OnBnClickedButtonCleanLog)
	ON_BN_CLICKED(IDC_BUTTON_GET_WIFI_IP, &CSmartHomeDlg::OnBnClickedButtonGetWifiIp)
	ON_BN_CLICKED(IDC_BUTTON_SEND_STATION, &CSmartHomeDlg::OnBnClickedButtonSendStation)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_COORDINATE, &CSmartHomeDlg::OnBnClickedButtonOpenCoordinate)
	ON_BN_CLICKED(IDC_BUTTON_FLIGHT_STATUS, &CSmartHomeDlg::OnBnClickedButtonFlightStatus)
	ON_WM_HSCROLL()
	ON_STN_CLICKED(IDC_STATIC_EXPECT_POWER, &CSmartHomeDlg::OnStnClickedStaticExpectPower)
	ON_BN_CLICKED(IDC_BUTTON_SET_80, &CSmartHomeDlg::OnBnClickedButtonSet80)
END_MESSAGE_MAP()




BOOL CSmartHomeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	bbaLog.BBA_LOG_Init(&m_CListBox_Ctrl);	//初始化日志类
	m_p_BBA_Coordinate = NULL;	//初始化波形图窗口指针
	m_p_BBA_FlightStatus = NULL;	//初始化飞行器数据窗口指针
	
	bba_AP_udp.Init("192.168.4.2", 1025, "192.168.4.1",1026);	//初始化连接到ESP8266 AP的IP地址和端口
	
	sliderPowerCtrl.SetRange(0,300);
	//SOCKET 异步消息
	WSAAsyncSelect(bba_AP_udp.m_socket, m_hWnd, WM_BBA_UDP_RECV, FD_READ);	//监听套接字UDP接收到的消息
	//m_pCWnd->ShowWindow(2);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSmartHomeDlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSmartHomeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSmartHomeDlg::OnBnClickedButtonCleanLog()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CListBox_Ctrl.ResetContent();
}

LRESULT CSmartHomeDlg::OnAP_ServerMsg(WPARAM wParam, LPARAM lParam)
{
	SOCKET m_socket = (SOCKET)wParam;
	uint8 p_buffer[200];	//UDP接收buff
	char logStr[200];	//日志输出buff
	char staticOutPut[50];	//输出到static
	uint32 power=0;
	uint32 zeroTimes=0;
	uint8 checkSum = 0;
	uint8 i;
	uint8 length;
	memset(p_buffer, 0x00, 200);
	int len;
	
	switch (WSAGETSELECTEVENT(lParam))
	{

	case FD_READ:
		len = bba_AP_udp.Recvfrom((char *)p_buffer);
		if (len != 10) return -1;
		p_buffer[len] = 0;
		//bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, p_buffer);
		length = p_buffer[2];
		checkSum = 0;
		for (i = 0; i < length + 3; i++)
		{
			checkSum += p_buffer[i];
		}

		if (checkSum != p_buffer[length + 3]) return -1;	//错误数据包
		//bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, "Message Recv：");

		zeroTimes = (uint16)(p_buffer[3] << 8);
		zeroTimes |= (uint16)p_buffer[4];

		power |=((uint32)p_buffer[5])<<24;
		power |= ((uint32)p_buffer[6]) << 16;
		power |= ((uint32)p_buffer[7]) << 8;
		power |=(uint32)p_buffer[8];

		sprintf(staticOutPut, "实际功率:%d W", power);
		GetDlgItem(IDC_STATIC_POWER)->SetWindowText(staticOutPut);

		sprintf(staticOutPut, "过零频率:%d", zeroTimes);
		GetDlgItem(IDC_STATIC_ZERO_TIMES)->SetWindowText(staticOutPut);

		break;
	case FD_CLOSE:
		break;
	default:
		break;
	}
	return 1;
}


void CSmartHomeDlg::OnBnClickedButtonSetWifi()
{
	char sendBuff[200];
	char wifiSSid[50];
	char wifiPass[50];
	char device[10];
	char baudRate[15];

	int sendBuffPos = 0;
	uint8 ssidLen, passLen,deviceNum;
	CString cstringWifiSSid;
	CString cstringWifiPass;
	CString cstringDevice;

	//获取设备号
	GetDlgItem(IDC_EDIT_DEVICE_NUM)->GetWindowText(cstringDevice);
	strcpy(device, cstringDevice);

	//获取WiFi
	GetDlgItem(IDC_EDIT_WIFI_SSID)->GetWindowText(cstringWifiSSid);
	GetDlgItem(IDC_EDIT_WIFI_PASSWORD)->GetWindowText(cstringWifiPass);

	strcpy(wifiSSid, cstringWifiSSid);
	strcpy(wifiPass, cstringWifiPass);
	
	ssidLen = strlen(wifiSSid);
	passLen = strlen(wifiPass);


	//bba_AP_udp.Send((char *)messPack.p_buff, );
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT,"设置新设备Wifi");
}


void CSmartHomeDlg::OnBnClickedButtonGetWifiIp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstringDevice;
	char device[10];
	char logStr[100];

	GetDlgItem(IDC_EDIT_DEVICE_NUM)->GetWindowText(cstringDevice);
	strcpy(device, cstringDevice);

	MessagePacketUnion messPack;
	messPack.messagePacket.sender = 1;
	messPack.messagePacket.receiver = atoi(device);
	messPack.messagePacket.function_word = GET_WIFI_IP;
	bba_AP_udp.Send((char *)messPack.p_buff, 5);
	sprintf(logStr, "请求设备：%d IP\r\n", messPack.messagePacket.receiver);
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
}


void CSmartHomeDlg::OnBnClickedButtonSendStation()
{
	// TODO: 在此添加控件通知处理程序代码
	bba_Station_udp.Send((char *)"Hello World!", 12);
}


void CSmartHomeDlg::OnBnClickedButtonOpenCoordinate()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_p_BBA_Coordinate == NULL)
	{
		m_p_BBA_Coordinate = new BBA_Coordinate();
		m_p_BBA_Coordinate->CreatCoordinateWindow("Coordinate", NULL, this, NULL, NULL);
	}
}


void CSmartHomeDlg::OnBnClickedButtonFlightStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_p_BBA_FlightStatus == NULL)
	{
		CRect rect(300, 300, 1500, 1000);
		m_p_BBA_FlightStatus = new BBA_FlightStatus();
		m_p_BBA_FlightStatus->CreatFlightStatusWindow("Flight Status", &rect, this, NULL, NULL);
		m_p_BBA_FlightStatus->bba_Station_udp = &bba_Station_udp;
	}
}


void CSmartHomeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	int expectPower,pValue,iValue,dValue;
	char outPutPos[30];

	expectPower =sliderPowerCtrl.GetPos();
	//itoa(pos, outPutPos, 10);	//十进制
	sprintf(outPutPos,"%d %c", expectPower,'W');
	GetDlgItem(IDC_STATIC_EXPECT_POWER)->SetWindowText(outPutPos);

	pValue = sliderPCtrl.GetPos();
	sprintf(outPutPos, "%d", pValue);
	GetDlgItem(IDC_STATIC_P_VALUE)->SetWindowText(outPutPos);

	iValue = sliderICtrl.GetPos();
	sprintf(outPutPos, "%d", iValue);
	GetDlgItem(IDC_STATIC_I_VALUE)->SetWindowText(outPutPos);

	dValue = sliderDCtrl.GetPos();
	sprintf(outPutPos, "%d", dValue);
	GetDlgItem(IDC_STATIC_D_VALUE)->SetWindowText(outPutPos);

	uint8 sendBuff[50];
	uint8 pos = 0;	//
	uint8 checkSum = 0;
	uint8 i;

	sendBuff[pos++] = 0xAA;	//帧头
	sendBuff[pos++] = 0xAB;	//帧头
	sendBuff[pos++] = 0x00;	//数据长度

	sendBuff[pos++] = expectPower >> 8;	//
	sendBuff[pos++] = expectPower;

	//sendBuff[pos++] = pValue >> 8;	//
	//sendBuff[pos++] = pValue;

	//sendBuff[pos++] = iValue >> 8;	//
	//sendBuff[pos++] = iValue;

	//sendBuff[pos++] = dValue >> 8;	//
	//sendBuff[pos++] = dValue;

	sendBuff[2] = pos - 3;
	for (i = 0; i < pos; i++)
	{
		checkSum += sendBuff[i];
	}
	sendBuff[pos++] = checkSum;

	bba_AP_udp.Send((char*)sendBuff, pos);
}


void CSmartHomeDlg::OnStnClickedStaticExpectPower()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSmartHomeDlg::OnBnClickedButtonSet80()
{
	// TODO: 在此添加控件通知处理程序代码
	sliderPowerCtrl.SetPos(80);
	OnHScroll(NULL,NULL,NULL);
	//Invalidate();	//发出重绘消息OnPaint
}
