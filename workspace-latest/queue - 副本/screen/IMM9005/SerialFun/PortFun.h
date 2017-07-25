
#include <Windows.h>
#ifndef    H_PORTFUN
#define    H_PORTFUN

class MyPort
{
public:
	MyPort();
	~MyPort();
public:
	HANDLE m_hPort;
	bool portflag;
public:
	//打开端口
	int OpenPort(char *strPort);
	/****读端口函数****/
	int ReadPort(unsigned char  *InBuff,int BuffLen);
	/****写端口函数****/
	int WritePort(unsigned char *OutBuff,int BuffLen);
	void ClosePort(void);
	void ResetM();
	//读端口间隔时间（两个字符之间的间隔时间）
	DWORD ReadInterval;
	//读端口间隔时间（两个字符之间的间隔时间）
	DWORD ReadMutiplier;
	//写端口间隔时间（两个字符之间的间隔时间）
	DWORD WriteMutiplier;
	//串口通讯特性
	//波特率
	DWORD BaudRate;
	//奇偶校验
	BYTE  Parity;
	//数据位
	BYTE  ByteSize;
	//停止位
	BYTE  StopBits;
};

#endif