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

	// д��Ļ�߳�
	char ScreenClass[256] = { 0 };
	FWParamValueINI("SCREENCLASS", ScreenClass);
	printf("��Ļ������ = [%s]\n", ScreenClass);
	m_screenDispatch = new ScreenDispatch( ScreenClass );
	m_screenThread   = new Thread( "m_screenDispatch", m_screenDispatch );

	// �������߳�
	char VoicesClass[256] = { 0 };
	FWParamValueINI("VOICESCLASS", VoicesClass);
	printf("���������� = [%s]\n", VoicesClass);
	m_voicesDispatch = new VoicesDispatch( VoicesClass );
	m_voicesThread   = new Thread( "m_voicesDispatch", m_voicesDispatch );

	// �豸״̬�����߳�
	//m_deviceDispatch = new DeviceDispatch( "STA9001" );
	//m_deviceThread   = new Thread( "m_deviceDispatch", m_deviceDispatch );
}

Queue::~Queue()
{
	// д��Ļ�߳�����
	delete m_screenDispatch;
	delete m_screenThread;

	// ���������߳�����
	delete m_voicesDispatch;
	delete m_voicesThread;
	
	// �豸״̬�����߳�����
	//delete m_deviceDispatch;
	//delete m_deviceThread;

	// SocketServer����
	delete m_socketServer;
}

void Queue::Run()
{
	// ����д��Ļ�߳�
	m_screenThread->Start();
	// �������������߳�
	m_voicesThread->Start();
	// �����豸״̬�����߳�
	//m_deviceThread->Start();

	// ��ʼ��SocketServer
	m_socketServer->InitSocket();
	// SocketServer����......
	m_socketServer->Running();

	// �ƺ���
	m_screenThread->Join();
	m_voicesThread->Join();
//	m_deviceThread->Join();
}
