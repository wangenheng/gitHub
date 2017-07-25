#pragma once
#include "../../include/framework.h"
#include "../../basefun.h"

#define MAXLINE	128

namespace IMM9010NM
{
	// ����ݵ������ӿ��Զ��庯��ָ��
	typedef bool   ( __stdcall *LpInitPort  )      ( byte );
	typedef bool   ( __stdcall *LpClosePort )      ( );
	typedef int    ( __stdcall *LPSendIntegScreen) (int Addr, int x,int y,int w,int h, char *Content);
	typedef int    ( __stdcall *LpSendStripScreen) (int Addr, int lineno, char *Content,char algin,char ledlen);

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
	typedef struct StripScreenDefInfo
	{
		long  addr;
		char  defMessage[MAXLINE];
	} StripScreenDefInfo;

	// �ۺ��� Ĭ����ʾ�ṹ��
	typedef struct IntegScreenDefInfo
	{
		long  addr;
		char  defMessage[MAXLINE];
	} IntegScreenDefInfo;
}

using namespace IMM9010NM;

class IMM9010 : public ScreenAdapter
{
	Declare_DynCreate(IMM9010)
public:
	IMM9010(void);
public:
	virtual ~IMM9010(void);

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
	Log    *IMM9010Slog;
	bool    m_initStrip;	// �Ƿ��ʼ��������
	bool    m_initInteg;	// �Ƿ��ʼ���ۺ���
	int		m_stripDefNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // �ۺ�����Ŀ
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];

	char    showFormat[64]; // �ۺ�����ʾ��ʽ

	QueueArray *queueArray; //�ۺ�������
};
