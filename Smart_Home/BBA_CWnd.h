#pragma once
#include <afxwin.h>
#include <atldef.h>


class BBA_CWnd :public CWnd {
public:
	BBA_CWnd();
	~BBA_CWnd();
protected:
	DECLARE_MESSAGE_MAP();

public:
	CString strWndClass;
public:
	int CreatWindow(CString wndTitle, CRect *rect=NULL,CWnd* pParent=NULL,DWORD id = 0, DWORD style = WS_VISIBLE | WS_CHILD);
};
