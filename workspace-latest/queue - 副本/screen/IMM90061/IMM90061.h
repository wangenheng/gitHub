#pragma once
#include "../../include/framework.h"

// ��̩����

#define MAXLINE	128

namespace IMM90061NM
{
	// ����ݵ������ӿ��Զ��庯��ָ��
	typedef int	   ( __stdcall *pFunSendDataZHLine ) ( int, long, int, char *, int );

	// ����ָ���ʼ��
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

using namespace IMM90061NM;

class IMM90061 : public ScreenAdapter
{
	Declare_DynCreate(IMM90061)
public:
	IMM90061(void);
public:
	virtual ~IMM90061(void);

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
	int inline PackInitMessage( int addr, unsigned char *InitMessage );
	int inline PackStripScreenMsg( const char *recvBuffer, int addr, unsigned char *sendBuffer );
	int inline PackInterScreenMsg( const char *recvBuffer, int addr, unsigned char *sendBuffer );
	int LoadLibrarys( char *pathname );

private:
	Log  *IMM90061Slog;
	SerialPort    Com1;
	SerialPort    Com2;
	int     m_ComPort1;
	int     m_ComPort2;
	bool	m_initStrip;	// �Ƿ��ʼ��������
	bool	m_initInteg;	// �Ƿ��ʼ���ۺ���
	int		m_stripDefNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // �ۺ�����Ŀ
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];

	int	    m_Len;
	
};
