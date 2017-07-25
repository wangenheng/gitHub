#include <iostream>
#include "ScreenDispatch.h"
#include "CyclBuffer.h"
#include "SplitStrings.h"
#include "ConfigReader.h"

using namespace std;

ScreenDispatch::ScreenDispatch( )
{
}

ScreenDispatch::ScreenDispatch( const char *classname )
{
	char temp[16];

	memset(temp, 0, sizeof(temp));
	FWParamValueINI("LOGLEVEL", temp);
	log_Screen = new Log("log\\ScreenDispatch.log", temp);
	log_Screen->WriteLog(LL_DEBUG, "classname = [%s]\n[%s|%d]\n", classname, __FILE__, __LINE__);

	m_classname = classname;
	m_prc = RuntimeClass::LoadByName( m_classname );
    if (m_prc == NULL) {
		cout << m_classname << endl;
	    return ;
	}   
	m_screenAdapter = (ScreenAdapter *)m_prc->CreateObject();
	log_Screen->WriteLog(LL_INFO, "classname = [%s]\n[%s|%d]\n", classname, __FILE__, __LINE__);
}

ScreenDispatch::~ScreenDispatch()
{
	delete m_screenAdapter;
	log_Screen->CloseLogFile();
	delete log_Screen;
}

void ScreenDispatch::Run()
{
	printf("ScreenDispatch::Run()\n");
	log_Screen->WriteLog(LL_INFO, "ScreenDispatch::Run()\n[%s|%d]\n", __FILE__, __LINE__);
	do_process();
}

int ScreenDispatch::do_process()
{
	char recvBuffer[128];
	char stripAddr[16];				// ������ַ
	char stripScreenInfo[64];		// ������ʾ��Ϣ
	char stripShowMode[8];			// ������ʾģʽ
	char IntegScreenInfo[8];		// �ۺ�����ʾ��Ϣ

	// �򿪴����豸(�������๹�캯���е��ã��˴�ע�͵�)
	m_screenAdapter->InitiDev();
	// ������ʼ����ʾ
	m_screenAdapter->InitStripScreen();
	// �ۺ�����ʼ����ʾ
	m_screenAdapter->InitIntegScreen();

	log_Screen->WriteLog(LL_INFO, "m_screenAdapter->InitiDev()\n[%s|%d]\n", __FILE__, __LINE__);
	log_Screen->WriteLog(LL_INFO, "m_screenAdapter->InitStripScreen()\n[%s|%d]\n", __FILE__, __LINE__);
	log_Screen->WriteLog(LL_INFO, "m_screenAdapter->InitIntegScreen()\n[%s|%d]\n", __FILE__, __LINE__);

	while (true)
	{
		// ��ʼ��
		memset( recvBuffer     , 0, sizeof(recvBuffer)      );
		memset( stripAddr      , 0, sizeof(stripAddr)       );
		memset( stripScreenInfo, 0, sizeof(stripScreenInfo) );
		memset( stripShowMode  , 0, sizeof(stripShowMode)   );
		memset( IntegScreenInfo, 0, sizeof(IntegScreenInfo) );

		// �����ݵ�recvBuffer��
		//printf("ScreenThread is Running ...\n");
		cyclBuffer.ReadScreenData( recvBuffer );
		//printf("ScreenDispatch::do_process = [%s]\n", recvBuffer);
		//m_screenAdapter->printClassName();
		//m_screenAdapter->print();

		// ����ַ���recvBuffer : ��ȡ�ͻ��˷��͵���Ϣ  -> "1|1|��B001��02����|00|B001,02"
		// stripAddr            : ������ַ				->    1	
		// stripScreenInfo      : ������ʾ��Ϣ			->		��B001��02����
		// stripShowMode        : ������ʾģʽ			->					   00
		// IntegScreenInfo      : �ۺ�����ʾ��Ϣ		->						  B001,02
		ScreenSlipString( recvBuffer, stripAddr, stripScreenInfo, stripShowMode, IntegScreenInfo );

		// ������������Ϣ
		//m_screenAdapter->SendStripScreen( stripAddr, stripScreenInfo, 0/*stripShowMode*/, NULL/*IntegScreenInfo*/ );
		m_screenAdapter->SendStripScreen( stripAddr, stripScreenInfo, 0, NULL );

		// ���ۺ���������Ϣ
		if ( IntegScreenInfo[0]!=0 && strlen(IntegScreenInfo)>0 )
		{
			m_screenAdapter->SendIntegScreen( IntegScreenInfo, 0, 0, NULL );
		}
		
		log_Screen->WriteLog( LL_INFO, "recvBuffer      = [%s]", recvBuffer );
		log_Screen->WriteLog( LL_INFO, "stripAddr       = [%s]", stripAddr );
		log_Screen->WriteLog( LL_INFO, "stripScreenInfo = [%s]", stripScreenInfo );
		log_Screen->WriteLog( LL_INFO, "stripShowMode   = [%s]", stripShowMode );
		log_Screen->WriteLog( LL_INFO, "IntegScreenInfo = [%s]\n[%s|%d]", IntegScreenInfo, __FILE__, __LINE__ );
	}
	// �رմ����豸
	m_screenAdapter->CloseDev( );

	return 0;
}

void ScreenDispatch::ScreenSlipString( const char *recvBuffer, char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{	// 0 1       2        3   4
	//"1|1|��B001��02����|00|B001,02"

	// ������ַ stripAddr
	//ScreenSlipChar( 1, recvBuffer, stripAddr      , '|' );
	SplitStrings::SplitStr( 1, recvBuffer, stripAddr      , '|' );

	// ������Ϣ stripScreenInfo
	//ScreenSlipChar( 2, recvBuffer, stripScreenInfo, '|' );
	SplitStrings::SplitStr( 2, recvBuffer, stripScreenInfo, '|' );

	// ������ʾģʽ stripShowMode
	//ScreenSlipChar( 3, recvBuffer, stripShowMode  , '|' );
	SplitStrings::SplitStr( 3, recvBuffer, stripShowMode  , '|' );

	// �ۺ�����Ϣ
	//ScreenSlipChar( 4, recvBuffer, IntegScreenInfo, '|' );
	SplitStrings::SplitStr( 4, recvBuffer, IntegScreenInfo, '|' );

	// stripAddr=1, stripScreenInfo="��B001��02����, stripShowMode=00, IntegScreenInfo=B001,02
}
