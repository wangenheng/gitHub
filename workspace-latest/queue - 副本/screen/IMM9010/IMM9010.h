#pragma once
#include "../../include/framework.h"
#include "../../basefun.h"

#define MAXLINE	128

namespace IMM9010NM
{
	// 请根据第三方接口自定义函数指针
	typedef bool   ( __stdcall *LpInitPort  )      ( byte );
	typedef bool   ( __stdcall *LpClosePort )      ( );
	typedef int    ( __stdcall *LPSendIntegScreen) (int Addr, int x,int y,int w,int h, char *Content);
	typedef int    ( __stdcall *LpSendStripScreen) (int Addr, int lineno, char *Content,char algin,char ledlen);

	// 函数指针初始化
	LpInitPort	OpenPort  = NULL;
	LpClosePort	ClosePort = NULL;
	LpSendStripScreen lpSendStripScreen = NULL;
	LPSendIntegScreen lpSendIntegScreen = NULL;

	// 综合屏 上送显示结构体
	typedef struct IntegScreenSndInfo {
		long  addr;
	} IntegScreenSndInfo;

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

using namespace IMM9010NM;

class IMM9010 : public ScreenAdapter
{
	Declare_DynCreate(IMM9010)
public:
	IMM9010(void);
public:
	virtual ~IMM9010(void);

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

private:
	Log    *IMM9010Slog;
	bool    m_initStrip;	// 是否初始化窗口屏
	bool    m_initInteg;	// 是否初始化综合屏
	int		m_stripDefNums; // 窗口屏数目
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // 综合屏数目
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // 综合屏数目
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];

	char    showFormat[64]; // 综合屏显示格式

	QueueArray *queueArray; //综合屏队列
};
