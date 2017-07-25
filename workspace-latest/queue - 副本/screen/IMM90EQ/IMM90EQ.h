#pragma once
#include "../../include/framework.h"
#include "EQ2008_DataStruct.h"
// 90EQ

#define MAXLINE	128

namespace IMM90EQNM
{
	// 请根据第三方接口自定义函数指针
	// IP地址一体屏
	typedef bool(__stdcall *lpUser_RealtimeConnect)     ( int );
	typedef bool(__stdcall *lpUser_RealtimeSendText)    ( int, int, int, int, int, char*, User_FontSet* );
	typedef bool(__stdcall *lpUser_RealtimeDisConnect)  ( int ); 
	// 函数指针初始化
	lpUser_RealtimeConnect    realtimeConnect    = NULL;
	lpUser_RealtimeSendText   realtimeSendText   = NULL;
	lpUser_RealtimeDisConnect realtimeDisConnect = NULL;

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

using namespace IMM90EQNM;

class IMM90EQ : public ScreenAdapter
{
	Declare_DynCreate(IMM90EQ)
public:
	IMM90EQ(void);
public:
	virtual ~IMM90EQ(void);

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
	int LoadLibrarys( );
	bool sendInfo( char *recvData, int addr, int type);//type：1窗口序号，2业务类别，3叫号信息

private:
	Log		*IMM90EQSlog;
	bool	m_initStrip; // 是否初始化窗口屏
	bool	m_initInteg; // 是否初始化综合屏
	int		m_stripDefNums; // 窗口屏数目
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // 综合屏数目
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // 综合屏数目
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];
	char    showFormat[64]; // 综合屏显示格式

	int     m_ckNums;     //窗口屏数目



private: // 屏幕显示相关参数
	int		m_ComPort;
	int		m_ComBaund;
	int		m_StripAddr;
	int		m_ShowColor;
	int		m_ShowMode;
	int		m_ShowTime;
	int		m_ShowSpeed;


};

//#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM90EQ.ini")->GetParamValue(key, valueBuffer)
