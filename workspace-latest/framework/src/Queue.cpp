#include <stdio.h>
#include "Queue.h"
#include "CyclBuffer.h"
#include "ConfigReader.h"

CyclBuffer cyclBuffer;

Queue::Queue()
{
	char pszPort[16] = { 0 };
	FWParamValueINI("PORT", pszPort);
	m_socketServer   = new SocketServer(atoi(pszPort));

	// 写屏幕线程
	char ScreenClass[256] = { 0 };
	FWParamValueINI("SCREENCLASS", ScreenClass);
	printf("屏幕兼容类 = [%s]\n", ScreenClass);
	m_screenDispatch = new ScreenDispatch( ScreenClass );
	m_screenThread   = new Thread( "m_screenDispatch", m_screenDispatch );

	// 播语音线程
	char VoicesClass[256] = { 0 };
	FWParamValueINI("VOICESCLASS", VoicesClass);
	printf("语音兼容类 = [%s]\n", VoicesClass);
	m_voicesDispatch = new VoicesDispatch( VoicesClass );
	m_voicesThread   = new Thread( "m_voicesDispatch", m_voicesDispatch );

	// 设备状态更新线程
	//m_deviceDispatch = new DeviceDispatch( "STA9001" );
	//m_deviceThread   = new Thread( "m_deviceDispatch", m_deviceDispatch );
}

Queue::~Queue()
{
	// 写屏幕线程销毁
	delete m_screenDispatch;
	delete m_screenThread;

	// 语音播报线程销毁
	delete m_voicesDispatch;
	delete m_voicesThread;
	
	// 设备状态更新线程销毁
	//delete m_deviceDispatch;
	//delete m_deviceThread;

	// SocketServer销毁
	delete m_socketServer;
}

void Queue::Run()
{
	// 启动写屏幕线程
	m_screenThread->Start();
	// 启动语音播报线程
	m_voicesThread->Start();
	// 启动设备状态更新线程
	//m_deviceThread->Start();

	// 初始化SocketServer
	m_socketServer->InitSocket();
	// SocketServer启动......
	m_socketServer->Running();

	// 善后处理
	m_screenThread->Join();
	m_voicesThread->Join();
//	m_deviceThread->Join();
}
