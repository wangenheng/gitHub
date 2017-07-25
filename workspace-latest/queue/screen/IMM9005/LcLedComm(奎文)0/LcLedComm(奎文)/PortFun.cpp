#include "StdAfx.h"
#include "PortFun.h"
#include "Drvapi.h"

/////////////////////////////////////////////////////////////////
CPortFun::CPortFun()
{
	//��ʼ��������
	m_hPort = NULL;
	// ���˿������ַ������ͼ��
	ReadInterval=30;
	//���˿ڼ��ʱ�䣨�����ַ�֮��ļ��ʱ�䣩
	ReadMutiplier=30;
	//д�˿ڼ��ʱ�䣨�����ַ�֮��ļ��ʱ�䣩
	WriteMutiplier=30;
	//����ͨѶ����
	BaudRate=9600;			//������9600
	Parity=NOPARITY;		//��У��
	ByteSize=8;				//8λ
	StopBits=ONESTOPBIT;	//1λֹͣλ
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
����
	�򿪶˿�
����
	strPort:�˿ںţ���"COM1","LPT1"��	
����ֵ��
	-1:�򿪶˿�
	0:	�ɹ�
*/
int CPortFun::OpenPort(char *strPort)
{
	if(m_hPort)
	{
		CloseHandle(m_hPort);
		m_hPort=NULL;
	}
	//�򿪶˿�
    m_hPort=CreateFile(strPort,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if(m_hPort==INVALID_HANDLE_VALUE)
		return -1;

	if(strnicmp(strPort,"COM",3) == 0)
    {
		DCB dcb;	
	    GetCommState(m_hPort,&dcb);
		//������
	    dcb.BaudRate=BaudRate;
		//��żУ��
	    dcb.Parity=Parity;
		//���ݳ���
	    dcb.ByteSize=ByteSize;
		//ֹͣλ
	    dcb.StopBits=StopBits;
	    SetCommState(m_hPort,&dcb);
    }
	return 0;
}
/****���˿�****/
/********
����
	���˿�
	���������������������
����
	InBuff:�������ַ���,BuffLen:����ȡ�����ݵĳ���
����ֵ��
	-2:�˿ھ��Ϊ��
	-1:���˿�ʧ��
	nBytesRead:���������ݵĳ���
����˿�ʱ��Ϊ: BuffLen*ReadInterval����
*********/
int CPortFun::ReadPort(unsigned char *InBuff,int BuffLen)
{
	if(m_hPort==NULL)
		return -2;
	COMMTIMEOUTS timeouts;
	GetCommTimeouts(m_hPort,&timeouts);
	//���ö�ȡ�ַ�ʱ�����ַ�������ʱ����
	timeouts.ReadIntervalTimeout=MAXDWORD;//ReadInterval;
	//���ö�ȡ�ַ�ʱ�ļ��ʱ��
	timeouts.ReadTotalTimeoutMultiplier=0;//ReadMutiplier;
    timeouts.ReadTotalTimeoutConstant=100;
	SetCommTimeouts(m_hPort,&timeouts);
     
	DWORD nBytesRead=0;
	//���˿�
	if(!ReadFile(m_hPort,InBuff,BuffLen,&nBytesRead,NULL))
		return -1;
	InBuff[nBytesRead]='\0';
	//���������������������
	PurgeComm(m_hPort,PURGE_TXCLEAR|PURGE_RXCLEAR);

	//����ʵ�ʶ�ȡ�����ݳ���
	return (int)nBytesRead;
}
///////////////////////////////////////////////////////////////////
/****************************
���ܣ�
	���˿���дȡ����
	дǰ����������������
������
	OutBuff,��������ݣ�
	BuffLen,��������ݳ���
����ֵ��
	-2:�˿ھ��Ϊ��
	-1��д�˿�ʧ�ܣ�
	nBytesWrite��д����ֽ���
�д�˿�ʱ��Ϊ: BuffLen*WriteInterval����
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

	//дǰ����������������
	PurgeComm(m_hPort,PURGE_TXCLEAR|PURGE_RXCLEAR);

	if(!WriteFile(m_hPort,OutBuff,BuffLen,&nBytesWrite,NULL))
		return -1;
	return (int)nBytesWrite;
}

//�رն˿�
//���������ݺͷ�������
void CPortFun::ClosePort(void)
{
	if(m_hPort != NULL)
	{
		CloseHandle(m_hPort);
		m_hPort = NULL;
	}
}