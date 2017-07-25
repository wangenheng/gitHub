#pragma once
#include "../../include/framework.h"

// 瑞泽

#define MAXLINE	128

namespace IMM9003NM
{
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

using namespace IMM9003NM;

class IMM9003 : public ScreenAdapter
{
	Declare_DynCreate(IMM9003)
public:
	IMM9003(void);
public:
	virtual ~IMM9003(void);

protected:
	// 串口通信接口，操作窗口屏，综合屏

	// 打开串口设备
	virtual int InitiDev		( );

	// 关闭串口设备
	virtual int CloseDev		( );

	// 向条屏发送信息
	virtual int SendStripScreen ( char *, char *, char *, char * );

	// 向综合屏发送信息
	virtual int SendIntegScreen ( char *, int, int, char * );

	// 窗口屏初始化显示
	virtual int InitStripScreen ( );

	// 综合屏初始化显示
	virtual int InitIntegScreen ( );

private: // 辅助函数
	int loadStripScreenInfo( );
	int loadIntegScreenInfo( );
	int inline PacketMessage(const char *recvBuffer, int addr, unsigned char *sendBuffer);

private:
	Log    *IMM9003Slog;
	SerialPort Com;
	int		m_integSndNums; // 综合屏数目
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];

	bool	m_initStrip;    // 是否初始化窗口屏
	int		m_stripDefNums; // 窗口屏数目
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	bool	m_initInteg;    // 是否初始化综合屏
	int		m_integDefNums; // 综合屏数目
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];

	/*
	 * 综合屏组合方式 1 同步 2 独立 3 拼接
	 * 1: 多块综合屏同步显示
	 * 2: 多块综合屏独立显示
	 * 3: 多块综合屏拼接显示
	 */
	int     m_IntegShowMode;	
};
