#pragma once
#include "../../include/framework.h"
#include "../../basefun.h"

// ƽ������ -- �����ػ��汾

#define MAXLINE	128

namespace IMM90022NM
{
	// ����ݵ������ӿ��Զ��庯��ָ��
	typedef bool   ( __stdcall *LpInitPort  )      ( byte );
	typedef int    ( __stdcall *LpClosePort )      ( );
	typedef int    ( __stdcall *LPSendIntegScreen) ( byte addr, byte delay, char *content ); 
	typedef bool   ( __stdcall *LpSendStripScreen) ( byte addr, char *content );

	// ����ָ���ʼ��
	LpInitPort	OpenPort  = NULL;
	LpClosePort	ClosePort = NULL;
	LpSendStripScreen lpSendStripScreen = NULL;
	LPSendIntegScreen lpSendIntegScreen = NULL;

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

using namespace IMM90022NM;

class IMM90022 : public ScreenAdapter
{
	Declare_DynCreate(IMM90022)
public:
	IMM90022(void);
public:
	virtual ~IMM90022(void);

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
	int inline PacketMessage(const char *recvBuffer, int addr, unsigned char *sendBuffer);
	int PacketMessageToIntegScreen(const char *recvBuffer, int addr, unsigned char *sendBuffer);
	int LoadIntegScreen( );
	int LoadLibrarys( char *pathname );

private:
	Log  *IMM90022Slog;
	SerialPort Com;
	bool	m_initStrip; // �Ƿ��ʼ��������
	bool	m_initInteg; // �Ƿ��ʼ���ۺ���
	int		m_stripDefNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // �ۺ�����Ŀ
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];

	char    showFormat[64]; // �ۺ�����ʾ��ʽ

private:
	QueueArray *queueArray;
	int     m_integSubNums; // ������ƴ�ӳ��ۺ����ĸ���
};
