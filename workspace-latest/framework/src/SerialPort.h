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
	// COM口端口号
	HANDLE				m_hComm;
	// handles
	UINT				m_nPortNr;
	BOOL          		m_isOpen;
	
	COMMTIMEOUTS		m_CommTimeouts;
	DCB					m_dcb;
	
	//读端口间隔时间(两个字符之间的间隔时间)
	DWORD 				m_readInterval;
	//读端口间隔时间(两个字符之间的间隔时间)
	DWORD 				m_readMutiplier;
	//写端口间隔时间(两个字符之间的间隔时间)
	DWORD 				m_writeMutiplier;
	//串口通讯特性
	//波特率
	DWORD 				m_BaudRate;
	//奇偶校验
	BYTE  				m_Parity;
	//数据位
	BYTE  				m_ByteSize;
	//停止位
	BYTE  				m_StopBits;
};

#endif __SERIALPORT_H__
