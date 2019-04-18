#include "stdafx.h"
#include "BBA_FlightStatus.h"
#include "Resource.h"
#include "smartHomeTypedef.h"

BBA_FlightStatus::BBA_FlightStatus()
{

}
BBA_FlightStatus::~BBA_FlightStatus()
{

}

int BBA_FlightStatus::CreatFlightStatusWindow(CString wndTitle, CRect *rect, CWnd* pParent, DWORD id, DWORD style)
{
	unsigned long int myStyle = WS_SYSMENU;
	int ret = CreatWindow(wndTitle, rect, pParent, id, myStyle);
	return ret;
}

BEGIN_MESSAGE_MAP(BBA_FlightStatus, BBA_CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_HSCROLL()	//������Ϣ
END_MESSAGE_MAP()

void BBA_FlightStatus::OnSize(UINT nType, int cx, int cy)
{
	BBA_CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}
void BBA_FlightStatus::OnPaint()
{
	//��ÿɻ�����Ļ�����С
	this->GetClientRect(&m_RectClientWindow);

	m_pPDC = new CPaintDC(this);
	//�����������̬��Ϣ
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			m_pPDC->TextOut(textPoint[i][j].x, textPoint[i][j].y, textStr[i][j]);
		}
	}
	//���ƻ���������
	CPoint point;
	CString temp;
	
	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			temp.LoadStringA(IDS_STRING_STATIC_PITCH+i*sliderListNum+j);
			point = sliderPoint[i][j];
			point.y = point.y + sliderWidth / 2 - 8;	//������м�
			point.x = point.x - 50;	//�������ڻ���ǰ50�����ص�
			m_pPDC->TextOut(point.x, point.y, temp);
		}
	}
}

int BBA_FlightStatus::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BBA_CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetClientRect(&m_RectClientWindow);

	textLineNum=4;	//ÿһ����4��text
	textListNum=3;	//ÿһ����4��text
	textLineInterval = 30;	//ÿһ�м��Ϊ10������
	textListInterval = 150;	//ÿһ�еļ��Ϊ20������

	textStartPoint.x = 20;	//text������������x
	textStartPoint.y = 50;	//text������������y
	for (int i = 0; i < textLineNum; i++)
	{
		for (int j = 0; j < textListNum; j++)
		{
			textPoint[i][j].x = textStartPoint.x+ i * textListInterval;
			textPoint[i][j].y = textStartPoint.y+j * textLineInterval;
			memcpy(textStr[i][j], "Hello", 6);
		}
	}

	CString temp;
	CRect rect;
	sliderLineNum = 1;	//ÿһ����1��slider
	sliderListNum = 12;	//ÿһ����10��slider
	sliderLineInterval = 50;	//ÿһ��slider�ļ��
	sliderListInterval = 300;	//ÿһ��slider�ļ��
	sliderStartPoint.x = m_RectClientWindow.right-300;	//slider��������x
	sliderStartPoint.y = 20;	//slider��������y
	sliderLength = 250;		//������
	sliderWidth = 30;		//������
	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			sliderPoint[i][j].x = sliderStartPoint.x + i * sliderListInterval;
			sliderPoint[i][j].y = sliderStartPoint.y + j * sliderLineInterval;

			rect.left = sliderPoint[i][j].x;
			rect.top = sliderPoint[i][j].y;
			
			rect.bottom = rect.top + sliderWidth;	//�����ĸ�
			rect.right = rect.left + sliderLength;	//�����ĳ�
			slider[i][j].Create(WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, rect, this, IDC_SLIDER_MSG_START + i);
		}
	}
	slider[0][0].SetRange(-180, 180, TRUE);
	slider[0][1].SetRange(-180, 180, TRUE);
	slider[0][2].SetRange(-180, 180, TRUE);
	slider[0][3].SetRange(1000, 2000, TRUE);

	slider[0][0].SetPos(0);
	slider[0][1].SetPos(0);
	slider[0][2].SetPos(0);
	slider[0][3].SetPos(0);


	slider[0][4].SetRange(1000, 2000, TRUE);
	slider[0][5].SetRange(1000, 2000, TRUE);
	slider[0][6].SetRange(1000, 2000, TRUE);
	slider[0][7].SetRange(1000, 2000, TRUE);
	slider[0][8].SetRange(1000, 2000, TRUE);
	slider[0][9].SetRange(1000, 2000, TRUE);
	slider[0][10].SetRange(1000, 2000, TRUE);
	slider[0][11].SetRange(1000, 2000, TRUE);
	
	return 0;
}

void BBA_FlightStatus::SendData()
{
	
	uint8 sendBuff[50];
	uint8 pos = 0;

	sendBuff[pos++] = 0XAA;
	sendBuff[pos++] = 0XAF;
	sendBuff[pos++] = LOOP_TIME;	//������
	sendBuff[pos++] = 0x00;		//���ݳ���

	sendBuff[pos++] = sendData.Throttle >> 8;
	sendBuff[pos++] = sendData.Throttle;

	sendBuff[pos++] = sendData.Yaw >> 8;
	sendBuff[pos++] = sendData.Yaw;

	sendBuff[pos++] = sendData.Roll >> 8;
	sendBuff[pos++] = sendData.Roll;

	sendBuff[pos++] = sendData.Pitch >> 8;
	sendBuff[pos++] = sendData.Pitch;

	sendBuff[pos++] = sendData.AUX1 >> 8;
	sendBuff[pos++] = sendData.AUX1;
	sendBuff[pos++] = sendData.AUX2 >> 8;
	sendBuff[pos++] = sendData.AUX2;
	sendBuff[pos++] = sendData.AUX3 >> 8;
	sendBuff[pos++] = sendData.AUX3;
	sendBuff[pos++] = sendData.AUX4 >> 8;
	sendBuff[pos++] = sendData.AUX4;
	sendBuff[pos++] = sendData.AUX5 >> 8;
	sendBuff[pos++] = sendData.AUX5;
	sendBuff[pos++] = sendData.AUX6 >> 8;
	sendBuff[pos++] = sendData.AUX6;
	sendBuff[3] = pos - 4;

	uint8 sum = 0;
	for (uint8 i = 0; i < pos; i++)
	{
		sum += sendBuff[i];
	}

	sendBuff[pos++] = sum;

	bba_Station_udp->Send((char*)sendBuff, pos);

}
void BBA_FlightStatus::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	BBA_CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	pScrollBar->GetDlgCtrlID();

	char outPutPos[10];	//�洢���ִ�

	

	for (int i = 0; i < sliderLineNum; i++)
	{
		for (int j = 0; j < sliderListNum; j++)
		{
			if(&slider[i][j]==(CSliderCtrl*)pScrollBar)
			{ 
				int pos = slider[i][j].GetPos();
				CPoint point = sliderPoint[i][j];
				point.y = point.y + sliderWidth / 2-8;	//������м�
				point.x = point.x + sliderLength;	//�ֳ�
				itoa(pos, outPutPos, 10);	//ʮ����
				memset(outPutPos + strlen(outPutPos), ' ', 10 - strlen(outPutPos));	//��0�������ո�
				outPutPos[9] = 0;	//�����ӽ�����
				m_pPDC->TextOut(point.x,point.y, TEXT(outPutPos));
			}
		}
	}

	sendData.Throttle = slider[0][3].GetPos();
	sendData.Yaw = slider[0][2].GetPos();
	sendData.Roll = slider[0][1].GetPos();
	sendData.Pitch = slider[0][0].GetPos();

	sendData.AUX1 = slider[0][4].GetPos();
	sendData.AUX2 = slider[0][5].GetPos();
	sendData.AUX3 = slider[0][6].GetPos();
	sendData.AUX4 = slider[0][7].GetPos();
	sendData.AUX5 = slider[0][8].GetPos();
	sendData.AUX6 = slider[0][9].GetPos();

	SendData();
}

int BBA_FlightStatus::UpdataFlightStatusWindows(char *buff, int len)
{
	enum FlightFuntionWord funtionWord;
	float pitch, roll, yaw;
	int acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;
	uint32 imuLoopTime,mpuLoopTime;

	uint8 framePos=0;	//��¼֡ͷ��buff��λ��
	uint8 frameLength;	//һ֡�ĳ���
	//uint8 buff[50];

	//while (1)
	{
		//frameLength = rcBuff[framePos + 3];
		//memcpy(buff, rcBuff+framePos, frameLength + 4);
		//framePos += frameLength + 4;	//��һ֡��λ��
		uint8 checkSum = 0;
		uint8 rang = buff[3] + 4;
		for (uint8 i = 0; i < rang; i++)
			checkSum += buff[i];

		if (checkSum != buff[len-1])
			return -1;

		switch (uint8(buff[2]))
		{

		case STATUS:
			pitch = float(int16(buff[4] << 8) + uint8(buff[5])) / 100;
			roll = float(int16(buff[6] << 8) + uint8(buff[7])) / 100;
			yaw = float(int16(buff[8] << 8) + uint8(buff[9])) / 100;

			sprintf(textStr[0][0], "Pitch:%f        ", pitch);
			sprintf(textStr[0][1], "Roll:%f        ", roll);
			sprintf(textStr[0][2], "Yaw:%f        ", yaw);

			m_pPDC->TextOut(textPoint[0][0].x, textPoint[0][0].y, textStr[0][0]);
			m_pPDC->TextOut(textPoint[0][1].x, textPoint[0][1].y, textStr[0][1]);
			m_pPDC->TextOut(textPoint[0][2].x, textPoint[0][2].y, textStr[0][2]);
			break;
		case SENSER:
			acc_x = (int16(buff[4] << 8) + uint8(buff[5]));
			acc_y = (int16(buff[4] << 8) + uint8(buff[5]));
			acc_z = (int16(buff[4] << 8) + uint8(buff[5]));

			gyro_x = (int16(buff[4] << 8) + uint8(buff[5]));
			gyro_y = (int16(buff[4] << 8) + uint8(buff[5]));
			gyro_z = (int16(buff[4] << 8) + uint8(buff[5]));

			sprintf(textStr[1][0], "ACC_X:%d        ", acc_x);
			sprintf(textStr[1][1], "ACC_Y:%d        ", acc_y);
			sprintf(textStr[1][2], "ACC_Z:%d        ", acc_z);

			sprintf(textStr[2][0], "GYRO_X:%d        ", gyro_x);
			sprintf(textStr[2][1], "GYRO_Y:%d        ", gyro_y);
			sprintf(textStr[2][2], "GYRO_Z:%d        ", gyro_z);

			m_pPDC->TextOut(textPoint[1][0].x, textPoint[1][0].y, textStr[1][0]);
			m_pPDC->TextOut(textPoint[1][1].x, textPoint[1][1].y, textStr[1][1]);
			m_pPDC->TextOut(textPoint[1][2].x, textPoint[1][2].y, textStr[1][2]);

			m_pPDC->TextOut(textPoint[2][0].x, textPoint[2][0].y, textStr[2][0]);
			m_pPDC->TextOut(textPoint[2][1].x, textPoint[2][1].y, textStr[2][1]);
			m_pPDC->TextOut(textPoint[2][2].x, textPoint[2][2].y, textStr[2][2]);
			break;
		case LOOP_TIME:
			imuLoopTime = int32(buff[4] << 24) + uint32(buff[5] << 16) + uint32(buff[6] << 8) + uint32(buff[7]);	//ns
			sprintf(textStr[3][0], "IMU_LoopTime:%d ms       ", imuLoopTime);
			m_pPDC->TextOut(textPoint[3][0].x, textPoint[3][0].y, textStr[3][0]);

			mpuLoopTime = int32(buff[8] << 24) + uint32(buff[9] << 16) + uint32(buff[10] << 8) + uint32(buff[11]);	//ns
			sprintf(textStr[3][1], "MPU_LoopTime:%d ms       ", mpuLoopTime);
			m_pPDC->TextOut(textPoint[3][1].x, textPoint[3][1].y, textStr[3][1]);
			break;
		}
		//if (framePos >= len) break;
	}

	return 0;
}
