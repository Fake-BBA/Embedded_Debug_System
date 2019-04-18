#include "stdafx.h"
#include "bba_log.h"
#include <stdio.h>
#include <time.h>



BBA_Log::BBA_Log()
{

}

BBA_Log::BBA_Log(CListBox *p_CListBox_Ctrl)
{
	this->p_CListBox_Ctrl = p_CListBox_Ctrl;
	//this->m_CDialog = m_CDialog;
	p_char_Log = new char(BBA_LOG_SINGLE_MAX);
	//AllocConsole();
	//freopen("CONOUT$", "w+t", stdout);
}

BBA_Log::~BBA_Log()
{
	delete p_char_Log;
}

int BBA_Log::BBA_LOG_Init(CListBox *p_CListBox_Ctrl)
{
	this->p_CListBox_Ctrl = p_CListBox_Ctrl;
	//this->m_CDialog = m_CDialog;
	p_char_Log = new char(BBA_LOG_SINGLE_MAX);
	p_char_Log[0]=0;
	//AllocConsole();	//¿ØÖÆÌ¨Êä³ö
	//freopen("CONOUT$", "w+t", stdout);
	return 1;
}

void BBA_Log::CoutLog(int type, char *str)
{	
	time(&m_timeNow);
	m_m_logTm = *gmtime(&m_timeNow);
	//char strTemp[BBA_LOG_SINGLE_MAX];
	//memset(strTemp, 0x00, BBA_LOG_SINGLE_MAX);
#ifdef BEI_JIN_TIME
	sprintf(p_char_Log, "[%02d:%02d:%02d] :%s\r\n", m_m_logTm.tm_hour + 8, m_m_logTm.tm_min, m_m_logTm.tm_sec, str);
#else
	sprintf(char_Log, "[%02d:%02d:%02d] :%s\r\n", m_logTm.tm_hour + 8, m_logTm.tm_min, m_logTm.tm_sec, str);
#endif

	//strcat(p_char_Log, strTemp);
	//m_CS_Log.Insert(0, (CString)p_char_Log);
	p_CListBox_Ctrl->AddString(p_char_Log);
	p_CListBox_Ctrl->SetTopIndex(p_CListBox_Ctrl->GetCount() - 1);
	//m_CDialog->UpdateData(FALSE);
	//SetWindowText(m_hwnd, m_CS_Log);
}

