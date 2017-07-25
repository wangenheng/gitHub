#pragma once
#include "../../include/framework.h"

// ƽ������

#define MAXLINE	128

namespace IMM9002NM
{
	// �ۺ��� ������ʾ�ṹ��
	typedef struct IntegScreenSndInfo {
		long  addr;
	} IntegScreenSndInfo;

	// ������ Ĭ����ʾ�ṹ��
	typedef struct StripScreenDefInfo {
		long  addr;
		char  defMessage[MAXLINE];
	} StripScreenDefInfo;

	// �ۺ��� Ĭ����ʾ�ṹ��
	typedef struct IntegScreenDefInfo {
		long  addr;
		char  defMessage[MAXLINE];
	} IntegScreenDefInfo;
}

using namespace IMM9002NM;

class IMM9002 : public ScreenAdapter
{
	Declare_DynCreate(IMM9002)
public:
	IMM9002(void);
public:
	virtual ~IMM9002(void);

protected:
	// ����ͨ�Žӿڣ������������ۺ���

	// �򿪴����豸
	virtual int InitiDev		( );

	// �رմ����豸
	virtual int CloseDev		( );

	// ������������Ϣ
	virtual int SendStripScreen ( char *, char *, char *, char * );

	// ���ۺ���������Ϣ
	virtual int SendIntegScreen ( char *, int, int, char * );

	// ������ʼ����ʾ
	virtual int InitStripScreen ( );

	// �ۺ�����ʼ����ʾ
	virtual int InitIntegScreen ( );

private: // ��������
	int loadStripScreenInfo( );
	int loadIntegScreenInfo( );
	int inline PacketMessage(const char *recvBuffer, int addr, unsigned char *sendBuffer);

private:
	Log  *IMM9002Slog;
	SerialPort Com;
	bool	m_initStrip; // �Ƿ��ʼ��������
	bool	m_initInteg; // �Ƿ��ʼ���ۺ���
	int		m_stripDefNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // �ۺ�����Ŀ
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];
};
