#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "SocketServer.h"
#include "Thread.h"
#include "ScreenDispatch.h"
#include "VoicesDispatch.h"

class _declspec(dllexport) Queue
{
public:
	Queue();
	virtual ~Queue();
public:
	void Run();
private:
	// SocketServer
	SocketServer   *m_socketServer;

	// 写屏幕线程
	ScreenDispatch *m_screenDispatch;
	Thread         *m_screenThread;	

	// 语音播报线程
	VoicesDispatch *m_voicesDispatch;
	Thread         *m_voicesThread;

	// 设备状态更新线程
	//DeviceDispatch *m_deviceDispatch;
	//Thread         *m_deviceThread;
};

#endif // end __QUEUE_H__
