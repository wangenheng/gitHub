
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
	//�򿪶˿�
	int OpenPort(char *strPort);
	/****���˿ں���****/
	int ReadPort(unsigned char  *InBuff,int BuffLen);
	/****д�˿ں���****/
	int WritePort(unsigned char *OutBuff,int BuffLen);
	void ClosePort(void);
	void ResetM();
	//���˿ڼ��ʱ�䣨�����ַ�֮��ļ��ʱ�䣩
	DWORD ReadInterval;
	//���˿ڼ��ʱ�䣨�����ַ�֮��ļ��ʱ�䣩
	DWORD ReadMutiplier;
	//д�˿ڼ��ʱ�䣨�����ַ�֮��ļ��ʱ�䣩
	DWORD WriteMutiplier;
	//����ͨѶ����
	//������
	DWORD BaudRate;
	//��żУ��
	BYTE  Parity;
	//����λ
	BYTE  ByteSize;
	//ֹͣλ
	BYTE  StopBits;
};

#endif