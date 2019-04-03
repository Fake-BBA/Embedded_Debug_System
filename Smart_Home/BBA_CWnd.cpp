#include "stdafx.h"
#include "BBA_CWnd.h"

BBA_CWnd::BBA_CWnd() {};
BBA_CWnd::~BBA_CWnd() {};

int BBA_CWnd::CreatWindow(CString wndTitle, CRect *rect, CWnd* pParent, DWORD id, DWORD style)
{
	strWndClass = AfxRegisterWndClass(0);	//注册窗口类

	CRect rectTemp;
	if (rect == NULL)
	{
		rect = &rectTemp;
		rectTemp.top = 500;
		rectTemp.left = 500;
		rectTemp.right = 2000;
		rectTemp.bottom = 1000;
	}

	int ret=CreateEx(NULL, strWndClass, TEXT(wndTitle),
		style,*rect, pParent,id);
	ShowWindow(SW_SHOW);
	UpdateWindow();	//重绘窗口
	return ret;
}

BEGIN_MESSAGE_MAP(BBA_CWnd, CWnd)

END_MESSAGE_MAP()
