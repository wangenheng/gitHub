#pragma once
#include "../../include/framework.h"

// 深圳银达通 dlltpzp.dll 依赖 borlndmm.dll 不要与 中意恒信混淆

#define MAXLINE	128

namespace IMM90011NM
{
	// 请根据第三方接口自定义函数指针
	typedef HANDLE ( __stdcall *pFunOpenPort       ) ( int, long, BYTE, BYTE, BYTE );
	typedef int    ( __stdcall *pFunClosePort      ) ( HANDLE );
	typedef int    ( __stdcall *pFunDataSend       ) ( HANDLE, int, char *, int, int, int, int );
	typedef int	   ( __stdcall *pFunSendDataZHLine ) ( int, long, int, char *, int );

	// 函数指针初始化
	HANDLE              g_Dllhandle    = NULL;
	pFunOpenPort        OpenPort       = NULL;
	pFunClosePort		ClosePort	   = NULL;
	pFunDataSend		DataSend	   = NULL;
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

using namespace IMM90011NM;

class IMM90011 : public ScreenAdapter
{
	Declare_DynCreate(IMM90011)
public:
	IMM90011(void);
public:
	virtual ~IMM90011(void);

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
	Log		*IMM90011Slog;
	bool	m_initStrip; // 是否初始化窗口屏
	bool	m_initInteg; // 是否初始化综合屏
	int		m_stripDefNums; // 窗口屏数目
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // 综合屏数目
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // 综合屏数目
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];


private: // 屏幕显示相关参数
	int		m_ComPort;
	int		m_ComBaund;
	int		m_StripAddr;
	int		m_ShowColor;
	int		m_ShowMode;
	int		m_ShowTime;
	int		m_ShowSpeed;
};
