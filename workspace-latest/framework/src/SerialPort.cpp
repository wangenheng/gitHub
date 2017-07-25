//#include "stdafx.h"
#include "SerialPort.h"

#include <stdio.h>
#include <assert.h>

// 关闭空语句警告
#pragma warning (disable:4390)
 
SerialPort::SerialPort()
{
	// 串口句柄
	m_hComm  = NULL;
	m_isOpen = FALSE;
	
	m_readInterval = 0;
	m_readMutiplier = 0;
	m_writeMutiplier = 0;
	
	//串口通讯特性
	//波特率
	m_BaudRate = 0;
	//奇偶校验
	m_Parity   = 0;
	//数据位
	m_ByteSize = 0;
	//停止位
	m_StopBits = 0;
}

SerialPort::~SerialPort()
{
	ClosePort();
}

BOOL SerialPort::InitPort(int  portnr,		// portnumber (1..9)
						  int  baud,		// baudrate
						  char  parity,		// parity 
						  int  databits,	// databits 
						  int  stopbits)	// stopbits						   
{
	assert(portnr > 0 && portnr < 10);
	
	m_nPortNr = portnr;
	
	BOOL bResult = FALSE;
	char *szPort = new char[64];
	char *szBaud = new char[64];

	// if the port is already opened: close it
	if (m_isOpen == TRUE)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
		m_isOpen = FALSE;
	}

	// prepare port strings
	sprintf(szPort, "COM%d", portnr);
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);

	// get a handle to the port
	m_hComm = CreateFile(szPort,						// communication port string (COMX)
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0,								// comm devices must be opened with exclusive access
					     NULL,							// no security attributes
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     0,							    // Async I/O
					     NULL);							// template must be 0 for comm devices

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;

		return FALSE;
	}

	// set the timeout values
	m_CommTimeouts.ReadIntervalTimeout         = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier  = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant    = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant   = 1000;

	// configure
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{						   
		if (GetCommState(m_hComm, &m_dcb))
		{
			m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
			if (BuildCommDCB(szBaud, &m_dcb))
			{
				if (SetCommState(m_hComm, &m_dcb)) 
					; // normal operation... continue
			}
		}
	}

	delete [] szPort;
	delete [] szBaud;

	// flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	
	m_isOpen = TRUE;

	return TRUE;
}

void SerialPort::ClosePort()
{
	if (m_isOpen == TRUE)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
		m_isOpen = FALSE;
	}
}

int SerialPort::WriteToPort(char* pszComSendBuffer, int ComSendBuffLen)
{
	BOOL bWrite  = TRUE;
	BOOL bResult = TRUE;
	DWORD dwError   = 0;
	DWORD BytesSent = 0;
	
	assert(m_isOpen == TRUE);

	if (bWrite)
	{
		// Clear buffer
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		bResult = WriteFile(m_hComm,						// Handle to COMM Port
							pszComSendBuffer,				// Pointer to message buffer in calling finction
							ComSendBuffLen,					// Length of message to send
							&BytesSent,						// Where to store the number of bytes sent
							NULL);

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
				{
					// continue to GetOverlappedResults()
					BytesSent = 0;
					bWrite = FALSE;
					break;
				}
				default:
				{
					// all other error codes
					//port->ProcessErrorMessage("WriteFile()");
					//记录ERROR日志
				}
			}
		} 
	} // end if(bWrite)

	// Verify that the data size send equals what we tried to send
	if ((int)BytesSent != ComSendBuffLen)
	{
		//TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
		//记录追踪日志
	}
	
	return BytesSent;
}

int SerialPort::ReadToPort(char* pszComRecvBuffer, int ComRecvBuffLen)
{
	BOOL  bRead     = TRUE;
	BOOL  bResult   = TRUE;
	DWORD dwError   = 0;
	DWORD BytesRead = 0;
	COMSTAT comstat;
	
	assert(m_isOpen == TRUE);
	
	bResult = ClearCommError(m_hComm, &dwError, &comstat);
	if (comstat.cbInQue == 0)
	{
		return -1;
	}

	if (bRead)
	{
		bResult = ReadFile(m_hComm,						// Handle to COMM port 
						   pszComRecvBuffer,			// RX Buffer Pointer
						   ComRecvBuffLen,				// Read one byte
						   &BytesRead,					// Stores number of bytes read
						   NULL);						// pointer to the m_ov structure
		// deal with the error code 
		if (!bResult)  
		{ 
			switch (dwError = GetLastError()) 
			{ 
				case ERROR_IO_PENDING: 	
				{ 
					// asynchronous i/o is still in progress
					// Proceed on to GetOverlappedResults();
					bRead = FALSE;
					break;
				}
				default:
				{
					// Another error has occured. Process this error.
					// port->ProcessErrorMessage("ReadFile()");
					// 记录日志
					break;
				} 
			}
		}
		else
		{
			// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
			bRead = TRUE;
		}
	}  // close if (bRead)
	
	// Verify that the data size send equals what we tried to send
	if ((int)BytesRead != ComRecvBuffLen)
	{
		//TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
		//记录追踪日志
	}
	
	return BytesRead;
}
