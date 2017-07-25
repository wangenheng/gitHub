#include "StdAfx.h"
#include "PortFun.h"
#include "Drvapi.h"

/////////////////////////////////////////////////////////////////
CPortFun::CPortFun()
{
	//初始化各变量
	m_hPort = NULL;
	// 读端口允许字符串传送间隔
	ReadInterval=30;
	//读端口间隔时间（两个字符之间的间隔时间）
	ReadMutiplier=30;
	//写端口间隔时间（两个字符之间的间隔时间）
	WriteMutiplier=30;
	//串口通讯特性
	BaudRate=9600;			//波特率9600
	Parity=NOPARITY;		//无校验
	ByteSize=8;				//8位
	StopBits=ONESTOPBIT;	//1位停止位
}

CPortFun::~CPortFun()
{
	if(m_hPort != NULL)
	{
		CloseHandle(m_hPort);
		m_hPort = NULL;
	}
}
/*
功能
	打开端口
参数
	strPort:端口号，如"COM1","LPT1"等	
返回值：
	-1:打开端口
	0:	成功
*/
int CPortFun::OpenPort(char *strPort)
{
	if(m_hPort)
	{
		CloseHandle(m_hPort);
		m_hPort=NULL;
	}
	//打开端口
    m_hPort=CreateFile(strPort,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if(m_hPort==INVALID_HANDLE_VALUE)
		return -1;

	if(strnicmp(strPort,"COM",3) == 0)
    {
		DCB dcb;	
	    GetCommState(m_hPort,&dcb);
		//波特率
	    dcb.BaudRate=BaudRate;
		//奇偶校验
	    dcb.Parity=Parity;
		//数据长度
	    dcb.ByteSize=ByteSize;
		//停止位
	    dcb.StopBits=StopBits;
	    SetCommState(m_hPort,&dcb);
    }
	return 0;
}
/****读端口****/
/********
功能
	读端口
	读完后清空输入输出缓冲区
参数
	InBuff:读到的字符串,BuffLen:待读取的数据的长度
返回值：
	-2:端口句柄为空
	-1:读端口失败
	nBytesRead:读到的数据的长度
最长读端口时间为: BuffLen*ReadInterval毫秒
*********/
int CPortFun::ReadPort(unsigned char *InBuff,int BuffLen)
{
	if(m_hPort==NULL)
		return -2;
	COMMTIMEOUTS timeouts;
	GetCommTimeouts(m_hPort,&timeouts);
	//设置读取字符时允许字符串传送时间间隔
	timeouts.ReadIntervalTimeout=MAXDWORD;//ReadInterval;
	//设置读取字符时的间隔时间
	timeouts.ReadTotalTimeoutMultiplier=0;//ReadMutiplier;
    timeouts.ReadTotalTimeoutConstant=100;
	SetCommTimeouts(m_hPort,&timeouts);
     
	DWORD nBytesRead=0;
	//读端口
	if(!ReadFile(m_hPort,InBuff,BuffLen,&nBytesRead,NULL))
		return -1;
	InBuff[nBytesRead]='\0';
	//读完后清空输入输出缓冲区
	PurgeComm(m_hPort,PURGE_TXCLEAR|PURGE_RXCLEAR);

	//返回实际读取的数据长度
	return (int)nBytesRead;
}
///////////////////////////////////////////////////////////////////
/****************************
功能：
	往端口中写取数据
	写前清空输入输出缓冲区
参数：
	OutBuff,输出的数据；
	BuffLen,输出的数据长度
返回值：
	-2:端口句柄为空
	-1：写端口失败；
	nBytesWrite：写入的字节数
最长写端口时间为: BuffLen*WriteInterval毫秒
*****************************/
int CPortFun::WritePort(unsigned char *OutBuff, int BuffLen)
{
	if(m_hPort==NULL)
		return -2;
	COMMTIMEOUTS timeouts;
	DWORD nBytesWrite;

	GetCommTimeouts(m_hPort,&timeouts);
	timeouts.WriteTotalTimeoutMultiplier = WriteMutiplier;
	SetCommTimeouts(m_hPort,&timeouts);

	//写前清空输入输出缓冲区
	PurgeComm(m_hPort,PURGE_TXCLEAR|PURGE_RXCLEAR);

	if(!WriteFile(m_hPort,OutBuff,BuffLen,&nBytesWrite,NULL))
		return -1;
	return (int)nBytesWrite;
}

//关闭端口
//无输入数据和返回数据
void CPortFun::ClosePort(void)
{
	if(m_hPort != NULL)
	{
		CloseHandle(m_hPort);
		m_hPort = NULL;
	}
}