#include "stdafx.h"

#define BEI_JIN_TIME


#define BBA_LOG_TYPE_ERROR	0
#define BBA_LOG_TYPE_PRINT	1

#define BBA_LOG_PRINTF printf

#define	BBA_LOG_SINGLE_MAX	100	//������־���������
class BBA_Log 
{
public:
	BBA_Log();
	BBA_Log(CListBox *p_CListBox_Ctrl);	//���캯��
	int BBA_LOG_Init(CListBox *p_CListBox_Ctrl);	//m_CDialogΪ��ǰ�Ի���ָ�룬p_cEditΪ�Ի�����ı༭�����ָ�� 
	~BBA_Log();	//��������
	void CoutLog(int type,char *str);
	//void Printf(int type, char *str);

private:
	CListBox *p_CListBox_Ctrl;
	//CString *m_CS_Log;	//��־����ı༭���丸�����ǶԻ�����־����CString��
	char *p_char_Log;		//������־�����ַ�����
	struct tm m_m_logTm;	//��¼��ǰʱ��
	long long m_timeNow;	//��ǰʱ���Ժ���Ϊ��λ������
};
