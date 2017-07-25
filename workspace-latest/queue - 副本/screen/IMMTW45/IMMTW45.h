#pragma once
#include "../../include/framework.h"

// TW45

#define MAXLINE	128

namespace IMMTW45NM
{
	// ����ݵ������ӿ��Զ��庯��ָ��
	// IP��ַһ����
	typedef int  ( _stdcall *pFunDrawRectString      )( HDC, char*, char*, int, int, int, int, int, int );
	typedef HDC  ( _stdcall *pFunGDCCreate           )( int, int );
	typedef int  ( _stdcall *pFunGDCFree             )( HDC );
	typedef int  ( _stdcall *pFunNetSendPicOnceNew9G )( int, char*, HDC, int, int, int, int, int, int, int, int, int, int, int, int, int );
	// �ۺ���
	typedef int  (_stdcall  *pFunSendDataZHLine      )( int ,long ,int ,char[] ,int ); 

	// ����ָ���ʼ��
	pFunDrawRectString		DrawRectString      = NULL;
	pFunGDCCreate			GDCCreate           = NULL;
	pFunGDCFree				GDCFree             = NULL;
	pFunNetSendPicOnceNew9G	NetSendPicOnceNew9G = NULL;
	pFunSendDataZHLine		SendDataZHLine      = NULL;

	// �ۺ��� ������ʾ�ṹ��
	typedef struct IntegScreenSndInfo {
		long  addr;
	} IntegScreenSndInfo;

	// ������ Ĭ����ʾ�ṹ��
	typedef struct StripScreenDefInfo {
		long  addr;
		char  defMessage[MAXLINE];
	} StripScreenDefInfo;

	// �ۺ��� Ĭ����ʾ�ṹ��
	typedef struct IntegScreenDefInfo {
		long  addr;
		char  defMessage[MAXLINE];
	} IntegScreenDefInfo;
}

using namespace IMMTW45NM;

class IMMTW45 : public ScreenAdapter
{
	Declare_DynCreate(IMMTW45)
public:
	IMMTW45(void);
public:
	virtual ~IMMTW45(void);

protected:
	// ����ͨ�Žӿڣ������������ۺ���

	// �򿪴����豸
	virtual int InitiDev		( );

	// �رմ����豸
	virtual int CloseDev		( );

	// ������������Ϣ
	virtual int SendStripScreen ( char *, char *, char *, char * );

	// ���ۺ���������Ϣ
	virtual int SendIntegScreen ( char *, int, int, char * );

	// ������ʼ����ʾ
	virtual int InitStripScreen ( );

	// �ۺ�����ʼ����ʾ
	virtual int InitIntegScreen ( );

private: // ��������
	int loadStripScreenInfo( );
	int loadIntegScreenInfo( );
	int LoadLibrarys( );
	void InitSendPic( );

private:
	Log		*IMMTW45Slog;
	bool	m_initStrip; // �Ƿ��ʼ��������
	bool	m_initInteg; // �Ƿ��ʼ���ۺ���
	int		m_stripDefNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // �ۺ�����Ŀ
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];
	char    showFormat[64]; // �ۺ�����ʾ��ʽ


private: // ��Ļ��ʾ��ز���
	int		m_ComPort;
	int		m_ComBaund;
	int		m_StripAddr;
	int		m_ShowColor;
	int		m_ShowMode;
	int		m_ShowTime;
	int		m_ShowSpeed;
};

//#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMMTW45.ini")->GetParamValue(key, valueBuffer)
