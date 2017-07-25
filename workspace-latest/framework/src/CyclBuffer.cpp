#include <stdio.h>
#include "CyclBuffer.h"
#include "ConfigReader.h"

//CyclBuffer cyclBuffer;

CyclBuffer::CyclBuffer()
{
	char logLevel[16];
	char pzBinding[8];

	// 日志
	memset(logLevel , 0, sizeof(logLevel) );
	memset(pzBinding, 0, sizeof(pzBinding));
	FWParamValueINI("LOGLEVEL", logLevel);
	cyclBufferLog = new Log("log\\CyclBuffer.log", logLevel);

	for (int i=0; i<ROWS; i++)
	{
		memset(ScreenData[i], 0, sizeof(ScreenData[i]));
		memset(VoicesData[i], 0, sizeof(VoicesData[i]));
	}
	wSIndex = rSIndex = 0;
	wVIndex = rVIndex = 0;

	InitializeCriticalSection( &m_ScreenData );
	m_hSemaphoreScreenEmpty = CreateSemaphore( NULL, ROWS, ROWS, NULL );
	m_hSemaphoreScreenFull  = CreateSemaphore( NULL,    0, ROWS, NULL );

	InitializeCriticalSection( &m_VoicesData );
	m_hSemaphoreVoicesEmpty = CreateSemaphore( NULL, ROWS, ROWS, NULL );
	m_hSemaphoreVoicesFull  = CreateSemaphore( NULL,    0, ROWS, NULL );

	// 约会模式信号量初始化
	m_hSemaphoreWaitScreen  = CreateSemaphore( NULL, 1, 1, NULL );
	m_hSemaphoreWaitVoices  = CreateSemaphore( NULL, 0, 1, NULL );

	// 默认非约会模式
	m_isBinding = false;
	FWParamValueINI("BINDING", pzBinding);
	m_isBinding = atoi(pzBinding)==0 ? false : true;
	cyclBufferLog->WriteLog(LL_DEBUG,  "m_isBinding = [%s]\n[%s|%d]",  \
		atoi(pzBinding)==0 ? "false" : "true", __FILE__, __LINE__);
	//printf("m_isBinding = [%s]\n", atoi(pzBinding)==0 ? "false" : "true");
}

CyclBuffer::~CyclBuffer()
{
	DeleteCriticalSection( &m_ScreenData );
	CloseHandle( m_hSemaphoreScreenEmpty );
	CloseHandle( m_hSemaphoreScreenFull  );

	DeleteCriticalSection( &m_VoicesData );
	CloseHandle( m_hSemaphoreVoicesEmpty );
	CloseHandle( m_hSemaphoreVoicesFull  );

	CloseHandle( m_hSemaphoreWaitScreen  );
	CloseHandle( m_hSemaphoreWaitVoices  );

	cyclBufferLog->CloseLogFile();
	delete cyclBufferLog;
}

void CyclBuffer::WriteScreenData(char *writeSData)
{
	// 等空缓冲区
	WaitForSingleObject( m_hSemaphoreScreenEmpty, INFINITE );
	
	// 互斥缓冲区
	EnterCriticalSection( &m_ScreenData );
	strncpy(ScreenData[wSIndex], writeSData, strlen(writeSData));
	ScreenData[wSIndex][strlen(writeSData)] = 0;
	cyclBufferLog->WriteLog(LL_DEBUG,  "-->向缓冲池第%d个缓冲区中投放数据[%s]\n[%s|%d]",  \
		wSIndex, ScreenData[wSIndex], __FILE__, __LINE__);
	//printf("-->向缓冲池第%d个缓冲区中投放数据[%s]\n", wSIndex, ScreenData[wSIndex]);
	wSIndex = (wSIndex + 1) % BUFFER_SIZE;
	LeaveCriticalSection( &m_ScreenData );
	
	// 通知有新数据
	ReleaseSemaphore( m_hSemaphoreScreenFull, 1, NULL );
}

void CyclBuffer::ReadScreenData(char *readSData)
{
	// 等非空缓冲区
	WaitForSingleObject( m_hSemaphoreScreenFull, INFINITE );
	
	// 互斥缓冲区
	EnterCriticalSection( &m_ScreenData );
	strncpy(readSData, ScreenData[rSIndex], strlen(ScreenData[rSIndex]));
	readSData[strlen(ScreenData[rSIndex])] = 0;
	cyclBufferLog->WriteLog(LL_DEBUG,  "  <--编号为%d的线程从缓冲池中第%d个缓冲区取出数据[%s]\n[%s|%d]",  \
		GetCurrentThreadId(), rSIndex, readSData, __FILE__, __LINE__);
	//printf("  <--编号为%d的线程从缓冲池中第%d个缓冲区取出数据[%s]\n", 
	//	GetCurrentThreadId(), rSIndex, readSData);
	rSIndex = (rSIndex + 1) % BUFFER_SIZE;
	LeaveCriticalSection( &m_ScreenData );
	
	// 释放空缓冲区
	ReleaseSemaphore( m_hSemaphoreScreenEmpty, 1, NULL );

	// 约会模式
	if (m_isBinding == true)
	{
		WaitForSingleObject( m_hSemaphoreWaitScreen, INFINITE );
		cyclBufferLog->WriteLog(LL_DEBUG,  "[%d]ScreenThread Waiting .... VoicesThread!\n[%s|%d]",  \
			GetCurrentThreadId(), __FILE__, __LINE__);
		printf("[%d]ScreenThread Waiting .... VoicesThread!\n", GetCurrentThreadId());
		ReleaseSemaphore( m_hSemaphoreWaitVoices, 1, NULL );
	}
}

void CyclBuffer::WriteVoicesData(char *writeVData)
{
	// 等空缓冲区
	WaitForSingleObject( m_hSemaphoreVoicesEmpty, INFINITE );
	
	// 互斥缓冲区
	EnterCriticalSection( &m_VoicesData );
	strncpy(VoicesData[wVIndex], writeVData, strlen(writeVData));
	VoicesData[wVIndex][strlen(writeVData)] = 0;
	cyclBufferLog->WriteLog(LL_DEBUG,  "-->向缓冲池第%d个缓冲区中投放数据[%s]\n[%s|%d]",  \
		wVIndex, VoicesData[wVIndex], __FILE__, __LINE__);
	//printf("-->向缓冲池第%d个缓冲区中投放数据[%s]\n", wVIndex, VoicesData[wVIndex]);
	wVIndex = (wVIndex + 1) % BUFFER_SIZE;
	LeaveCriticalSection( &m_VoicesData );
	
	// 通知有新数据
	ReleaseSemaphore( m_hSemaphoreVoicesFull, 1, NULL );
}

void CyclBuffer::ReadVoicesData(char *readVData)
{
	// 等非空缓冲区
	WaitForSingleObject( m_hSemaphoreVoicesFull, INFINITE );
	
	// 互斥缓冲区
	EnterCriticalSection( &m_VoicesData );
	strncpy(readVData, VoicesData[rVIndex], strlen(VoicesData[rVIndex]));
	readVData[strlen(VoicesData[rVIndex])] = 0;
	cyclBufferLog->WriteLog(LL_DEBUG,  "  <--编号为%d的线程从缓冲池中第%d个缓冲区取出数据[%s]\n[%s|%d]",  \
		GetCurrentThreadId(), rVIndex, readVData, __FILE__, __LINE__);
	//printf("  <--编号为%d的线程从缓冲池中第%d个缓冲区取出数据[%s]\n", 
	//	GetCurrentThreadId(), rVIndex, readVData);
	rVIndex = (rVIndex + 1) % BUFFER_SIZE;
	LeaveCriticalSection( &m_VoicesData );
	
	// 释放空缓冲区
	ReleaseSemaphore( m_hSemaphoreVoicesEmpty, 1, NULL );

	// 约会模式
	if (m_isBinding == true) 
	{
		WaitForSingleObject( m_hSemaphoreWaitScreen, INFINITE );
		cyclBufferLog->WriteLog(LL_DEBUG,  "[%d]ScreenThread Waiting .... VoicesThread!\n[%s|%d]",  \
			GetCurrentThreadId(), __FILE__, __LINE__);
		printf("[%d]ScreenThread Waiting .... VoicesThread!\n", GetCurrentThreadId());
		ReleaseSemaphore( m_hSemaphoreWaitVoices, 1, NULL );
	}
}
