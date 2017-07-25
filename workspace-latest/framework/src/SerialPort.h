#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#include <windows.h>

#define MAXLINELENGTH	1024*2

class _declspec(dllexport) SerialPort
{														 
public:
	// contruction and destruction
	SerialPort();
	virtual		~SerialPort();

	// port initialisation											
	BOOL		InitPort(int portnr = 1, int baud = 9600, char parity = 'N', int databits = 8, int stopsbits = 1);
	void        ClosePort();

	int         ReadToPort(char* str, int strlen);
	int 		WriteToPort(char* str, int strlen);
	
	void        ResetCommBuffer();
	
private:
	// COM�ڶ˿ں�
	HANDLE				m_hComm;
	// handles
	UINT				m_nPortNr;
	BOOL          		m_isOpen;
	
	COMMTIMEOUTS		m_CommTimeouts;
	DCB					m_dcb;
	
	//���˿ڼ��ʱ��(�����ַ�֮��ļ��ʱ��)
	DWORD 				m_readInterval;
	//���˿ڼ��ʱ��(�����ַ�֮��ļ��ʱ��)
	DWORD 				m_readMutiplier;
	//д�˿ڼ��ʱ��(�����ַ�֮��ļ��ʱ��)
	DWORD 				m_writeMutiplier;
	//����ͨѶ����
	//������
	DWORD 				m_BaudRate;
	//��żУ��
	BYTE  				m_Parity;
	//����λ
	BYTE  				m_ByteSize;
	//ֹͣλ
	BYTE  				m_StopBits;
};

#endif __SERIALPORT_H__
