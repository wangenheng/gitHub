#ifndef __SCREENADAPTER_H__
#define __SCREENADAPTER_H__

#include "Adapter.h"

class _declspec(dllexport) ScreenAdapter : public Adapter
{
	Declare_DynCreate(ScreenAdapter)
public:
	ScreenAdapter(void);
public:
	virtual ~ScreenAdapter(void);

public:
	// 串口通信接口，操作条屏，综合屏

	// 打开串口设备
	virtual int  InitiDev		 ( ) { return 0; }

	// 关闭串口设备
	virtual int  CloseDev		 ( ) { return 0; }

	// 向条屏发送信息
	virtual int  SendStripScreen ( char *, char *, char *, char * ) { return 0; }

	// 向综合屏发送信息
	virtual int  SendIntegScreen ( char *, int, int, char * ) { return 0; }

	// 条屏初始化显示
	virtual int  InitStripScreen ( ) { return 0; }

	// 综合屏初始化显示
	virtual int  InitIntegScreen ( ) { return 0; }

};

#endif // end __SCREENADAPTER_H__
