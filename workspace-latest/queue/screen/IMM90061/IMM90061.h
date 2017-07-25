#pragma once
#include "../../include/framework.h"

// 永泰新欣

#define MAXLINE	128

namespace IMM90061NM
{
	// 请根据第三方接口自定义函数指针
	typedef int	   ( __stdcall *pFunSendDataZHLine ) ( int, long, int, char *, int );

	// 函数指针初始化
	pFunSendDataZHLine	SendDataZHLine = NULL;

	// 综合屏 上送显示结构体
	typedef struct IntegScreenSndInfo {
		long  addr;
	} IntegScreenSndInfo;

	// 窗口屏 默认显示结构体
	typedef struct StripScreenDefInfo {
		long  addr;
		char  defMessage[MAXLINE];
	} StripScreenDefInfo;

	// 综合屏 默认显示结构体
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
	bool	m_initStrip;	// 是否初始化窗口屏
	bool	m_initInteg;	// 是否初始化综合屏
	int		m_stripDefNums; // 窗口屏数目
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // 综合屏数目
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // 综合屏数目
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];

	int	    m_Len;
	
};
