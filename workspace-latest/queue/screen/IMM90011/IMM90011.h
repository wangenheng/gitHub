#pragma once
#include "../../include/framework.h"

// ��������ͨ dlltpzp.dll ���� borlndmm.dll ��Ҫ�� ������Ż���

#define MAXLINE	128

namespace IMM90011NM
{
	// ����ݵ������ӿ��Զ��庯��ָ��
	typedef HANDLE ( __stdcall *pFunOpenPort       ) ( int, long, BYTE, BYTE, BYTE );
	typedef int    ( __stdcall *pFunClosePort      ) ( HANDLE );
	typedef int    ( __stdcall *pFunDataSend       ) ( HANDLE, int, char *, int, int, int, int );
	typedef int	   ( __stdcall *pFunSendDataZHLine ) ( int, long, int, char *, int );

	// ����ָ���ʼ��
	HANDLE              g_Dllhandle    = NULL;
	pFunOpenPort        OpenPort       = NULL;
	pFunClosePort		ClosePort	   = NULL;
	pFunDataSend		DataSend	   = NULL;
	pFunSendDataZHLine	SendDataZHLine = NULL;

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

using namespace IMM90011NM;

class IMM90011 : public ScreenAdapter
{
	Declare_DynCreate(IMM90011)
public:
	IMM90011(void);
public:
	virtual ~IMM90011(void);

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
	Log		*IMM90011Slog;
	bool	m_initStrip; // �Ƿ��ʼ��������
	bool	m_initInteg; // �Ƿ��ʼ���ۺ���
	int		m_stripDefNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // �ۺ�����Ŀ
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];


private: // ��Ļ��ʾ��ز���
	int		m_ComPort;
	int		m_ComBaund;
	int		m_StripAddr;
	int		m_ShowColor;
	int		m_ShowMode;
	int		m_ShowTime;
	int		m_ShowSpeed;
};
