#pragma once
#include "../../include/framework.h"
#include "../../basefun.h"

#define MAXLINE	128

namespace IMM9005NM
{
	// 请根据第三方接口自定义函数指针
	typedef HANDLE ( __stdcall *LpOpenPort  ) ( char *, long );
	typedef int    ( __stdcall *LpClosePort ) ( HANDLE );
	typedef int    ( __stdcall *LpDataSend  ) ( HANDLE, char *, int, int, int, int, char *, int );

	// 函数指针初始化
	HANDLE		g_handle  = NULL;
	LpOpenPort	OpenPort  = NULL;
	LpClosePort	ClosePort = NULL;
	LpDataSend	DataSend  = NULL;

	// 窗口屏 默认显示结构体
	typedef struct StripScreenDefInfo
	{
		long  addr;
		char  defMessage[MAXLINE];
	} StripScreenDefInfo;

	// 综合屏 默认显示结构体
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
	// 串口通信接口，操作条屏，综合屏

	// 打开串口设备
	virtual int InitiDev		( );

	// 关闭串口设备
	virtual int CloseDev		( );

	// 向条屏发送信息
	virtual int SendStripScreen ( char *, char *, char *, char * );

	// 向综合屏发送信息
	virtual int SendIntegScreen ( char *, int, int, char * );

	// 条屏初始化显示
	virtual int InitStripScreen ( );

	// 综合屏初始化显示
	virtual int InitIntegScreen ( );

private: // 辅助函数
	int loadStripScreenInfo( );
	int loadIntegScreenInfo( );
	int LoadLibrarys( char *pathname );
	// 加载综合屏配置
	int LoadIntegScreen();

private:
	Log  *IMM9005Slog;
	bool  m_initStrip; // 是否初始化窗口屏
	bool  m_initInteg; // 是否初始化综合屏
	int   m_stripNums; // 窗口屏数目
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int   m_integNums; // 综合屏数目
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	char  showFormat[64]; // 综合屏显示格式

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
	// 初始化队列
	void   InitQueue( );
	// 插入队列
	int    EnQueue( const char *data );
	// 访问队列
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