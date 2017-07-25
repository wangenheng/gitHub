#pragma once
#include "../../include/framework.h"
#include "../../basefun.h"

#define MAXLINE	128

namespace IMM9005NM
{
	// ����ݵ������ӿ��Զ��庯��ָ��
	typedef HANDLE ( __stdcall *LpOpenPort  ) ( char *, long );
	typedef int    ( __stdcall *LpClosePort ) ( HANDLE );
	typedef int    ( __stdcall *LpDataSend  ) ( HANDLE, char *, int, int, int, int, char *, int );

	// ����ָ���ʼ��
	HANDLE		g_handle  = NULL;
	LpOpenPort	OpenPort  = NULL;
	LpClosePort	ClosePort = NULL;
	LpDataSend	DataSend  = NULL;

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

using namespace IMM9005NM;

class QueueArray;

class IMM9005 : public ScreenAdapter
{
	Declare_DynCreate(IMM9005)
public:
	IMM9005(void);
public:
	virtual ~IMM9005(void);

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
	// �����ۺ�������
	int LoadIntegScreen();

private:
	Log  *IMM9005Slog;
	bool  m_initStrip; // �Ƿ��ʼ��������
	bool  m_initInteg; // �Ƿ��ʼ���ۺ���
	int   m_stripNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int   m_integNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	char  showFormat[64]; // �ۺ�����ʾ��ʽ

private:
	QueueArray *queueArray;
	int         m_sndintegnumbers;
};

/*
class QueueArray
{
public:
	QueueArray( ) {};
	QueueArray( int row, int col );
	~QueueArray( );

public:
	// ��ʼ������
	void   InitQueue( );
	// �������
	int    EnQueue( const char *data );
	// ���ʶ���
	char * VisQueue( int index );

public:
	void   SetRows( int rows );
	int    GetRows( ) { return m_row; }
	void   SetCols( int cols );
	int    GetCols( ) { return m_col; }
	void   ConstrObject( );

public:
	void   SetAddrAsIndex( int index, const char *src, int len );
	char * GetAddrAsIndex( int index );

private:
	int     m_row;
	int     m_col;
	char  **m_array;
	char  **m_addrs;
};
*/