#ifndef __CYCLBUFFER_H__
#define __CYCLBUFFER_H__

#include <windows.h>
#include <process.h>

#include "Log.h"

#define ROWS			100		// ����������
#define COLS			300		// ����������
#define BUFFER_SIZE		ROWS	// �������±�ģ��

class CyclBuffer
{
public:
	CyclBuffer();
	virtual ~CyclBuffer();

public:
	void WriteScreenData(char *writeSData);	// д��Ļ������
	void ReadScreenData(char *readSData);	// ����Ļ������

	void WriteVoicesData(char *writeVData);	// д����������
	void ReadVoicesData(char *readVData);	// ������������

private:
	char ScreenData[ROWS][COLS];			// ��Ļ������
	char VoicesData[ROWS][COLS];			// ����������
	int  wSIndex, rSIndex;					// ������Ļ�������±����
	int  wVIndex, rVIndex;					// ���������������±����

private:
	// Screen �ź�����ؼ���
	CRITICAL_SECTION m_ScreenData;			// ��Ļ�����������
	HANDLE	m_hSemaphoreScreenEmpty;		// ��Ļ����������Դ�ź���
	HANDLE	m_hSemaphoreScreenFull;			// ��Ļ����������Դ�ź���

	// Voices �ź�����ؼ���
	CRITICAL_SECTION m_VoicesData;			// ���������������
	HANDLE	m_hSemaphoreVoicesEmpty;		// ��������������Դ�ź���
	HANDLE	m_hSemaphoreVoicesFull;			// ��������������Դ�ź���

	// Լ��ģʽ�ź���
	HANDLE	m_hSemaphoreWaitScreen;
	HANDLE	m_hSemaphoreWaitVoices;
	bool	m_isBinding;

	// ��־
	Log		*cyclBufferLog;
};

extern CyclBuffer cyclBuffer;

#endif // end __CYCLBUFFER_H__