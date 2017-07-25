#ifndef __CYCLBUFFER_H__
#define __CYCLBUFFER_H__

#include <windows.h>
#include <process.h>

#include "Log.h"

#define ROWS			100		// 缓冲区行数
#define COLS			300		// 缓冲区列数
#define BUFFER_SIZE		ROWS	// 缓冲区下标模数

class CyclBuffer
{
public:
	CyclBuffer();
	virtual ~CyclBuffer();

public:
	void WriteScreenData(char *writeSData);	// 写屏幕缓冲区
	void ReadScreenData(char *readSData);	// 读屏幕缓冲区

	void WriteVoicesData(char *writeVData);	// 写语音缓冲区
	void ReadVoicesData(char *readVData);	// 读语音缓冲区

private:
	char ScreenData[ROWS][COLS];			// 屏幕缓冲区
	char VoicesData[ROWS][COLS];			// 语音缓冲区
	int  wSIndex, rSIndex;					// 操作屏幕缓冲区下标变量
	int  wVIndex, rVIndex;					// 操作语音缓冲区下标变量

private:
	// Screen 信号量与关键段
	CRITICAL_SECTION m_ScreenData;			// 屏幕缓冲区互斥段
	HANDLE	m_hSemaphoreScreenEmpty;		// 屏幕缓冲区空资源信号量
	HANDLE	m_hSemaphoreScreenFull;			// 屏幕缓冲区慢资源信号量

	// Voices 信号量与关键段
	CRITICAL_SECTION m_VoicesData;			// 语音缓冲区互斥段
	HANDLE	m_hSemaphoreVoicesEmpty;		// 语音缓冲区空资源信号量
	HANDLE	m_hSemaphoreVoicesFull;			// 语音缓冲区满资源信号量

	// 约会模式信号量
	HANDLE	m_hSemaphoreWaitScreen;
	HANDLE	m_hSemaphoreWaitVoices;
	bool	m_isBinding;

	// 日志
	Log		*cyclBufferLog;
};

extern CyclBuffer cyclBuffer;

#endif // end __CYCLBUFFER_H__