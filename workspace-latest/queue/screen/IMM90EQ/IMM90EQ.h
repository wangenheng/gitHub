#pragma once
#include "../../include/framework.h"
#include "EQ2008_DataStruct.h"
// 90EQ

#define MAXLINE	128

namespace IMM90EQNM
{
	// ����ݵ������ӿ��Զ��庯��ָ��
	// IP��ַһ����
	typedef bool(__stdcall *lpUser_RealtimeConnect)     ( int );
	typedef bool(__stdcall *lpUser_RealtimeSendText)    ( int, int, int, int, int, char*, User_FontSet* );
	typedef bool(__stdcall *lpUser_RealtimeDisConnect)  ( int ); 
	// ����ָ���ʼ��
	lpUser_RealtimeConnect    realtimeConnect    = NULL;
	lpUser_RealtimeSendText   realtimeSendText   = NULL;
	lpUser_RealtimeDisConnect realtimeDisConnect = NULL;

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

using namespace IMM90EQNM;

class IMM90EQ : public ScreenAdapter
{
	Declare_DynCreate(IMM90EQ)
public:
	IMM90EQ(void);
public:
	virtual ~IMM90EQ(void);

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
	int LoadLibrarys( );
	bool sendInfo( char *recvData, int addr, int type);//type��1������ţ�2ҵ�����3�к���Ϣ

private:
	Log		*IMM90EQSlog;
	bool	m_initStrip; // �Ƿ��ʼ��������
	bool	m_initInteg; // �Ƿ��ʼ���ۺ���
	int		m_stripDefNums; // ��������Ŀ
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // �ۺ�����Ŀ
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // �ۺ�����Ŀ
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];
	char    showFormat[64]; // �ۺ�����ʾ��ʽ

	int     m_ckNums;     //��������Ŀ



private: // ��Ļ��ʾ��ز���
	int		m_ComPort;
	int		m_ComBaund;
	int		m_StripAddr;
	int		m_ShowColor;
	int		m_ShowMode;
	int		m_ShowTime;
	int		m_ShowSpeed;


};

//#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM90EQ.ini")->GetParamValue(key, valueBuffer)
