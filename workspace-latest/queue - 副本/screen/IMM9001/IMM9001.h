#pragma once
#include "../../include/framework.h"

// �������

#define MAXLINE	128

namespace IMM9001NM
{
	// ����ݵ������ӿ��Զ��庯��ָ��
	typedef int	   ( __stdcall *pFunSendDatafun    ) ( int, long, int, char *, int, int, int, int );
	typedef int	   ( __stdcall *pFunSendDataZHLine ) ( int, long, int, char *, int );
	//commandtype=3
	typedef int    ( __stdcall *pFunSendDataOnly   ) ( int, long, int, int, char *, int );

	// ����ָ���ʼ��
	pFunSendDatafun		SendDatafun    = NULL;
	pFunSendDataZHLine	SendDataZHLine = NULL;
	pFunSendDataOnly	SendDataOnly   = NULL;

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

using namespace IMM9001NM;

class IMM9001 : public ScreenAdapter
{
	Declare_DynCreate(IMM9001)
public:
	IMM9001(void);
public:
	virtual ~IMM9001(void);

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
	int LoadLibrarys( char *pathname );

private:
	Log		*IMM9001Slog;
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
