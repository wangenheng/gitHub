#pragma once
#include "../../include/framework.h"

// ����

#define MAXLINE	128

namespace IMM9003NM
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

using namespace IMM9003NM;

class IMM9003 : public ScreenAdapter
{
	Declare_DynCreate(IMM9003)
public:
	IMM9003(void);
public:
	virtual ~IMM9003(void);

protected:
	// ����ͨ�Žӿڣ��������������ۺ���

	// �򿪴����豸
	virtual int InitiDev		( );

	// �رմ����豸
	virtual int CloseDev		( );

	// ������������Ϣ
	virtual int SendStripScreen ( char *, char *, char *, char * );

	// ���ۺ���������Ϣ
	virtual int SendIntegScreen ( char *, int, int, char * );

	// ��������ʼ����ʾ
	virtual int InitStripScreen ( );

	// �ۺ�����ʼ����ʾ
	virtual int InitIntegScreen ( );

private: // ��������
	int loadStripScreenInfo( );
	int loadIntegScreenInfo( );
	int inline PacketMessage(const char *recvBuffer, int addr, unsigned char *sendBuffer);

private:
	Log    *IMM9003Slog;
	SerialPort Com;
	int		m_integSndNums; // �ۺ�����Ŀ
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];

	bool	m_initStrip;    // �Ƿ��ʼ��������
	int		m_stripDefNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	bool	m_initInteg;    // �Ƿ��ʼ���ۺ���
	int		m_integDefNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];

	/*
	 * �ۺ�����Ϸ�ʽ 1 ͬ�� 2 ���� 3 ƴ��
	 * 1: ����ۺ���ͬ����ʾ
	 * 2: ����ۺ���������ʾ
	 * 3: ����ۺ���ƴ����ʾ
	 */
	int     m_IntegShowMode;	
};
