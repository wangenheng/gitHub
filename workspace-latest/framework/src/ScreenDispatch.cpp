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
	char stripAddr[16];				// 条屏地址
	char stripScreenInfo[64];		// 条屏显示信息
	char stripShowMode[8];			// 条屏显示模式
	char IntegScreenInfo[8];		// 综合屏显示信息

	// 打开串口设备(已在子类构造函数中调用，此处注释掉)
	m_screenAdapter->InitiDev();
	// 条屏初始化显示
	m_screenAdapter->InitStripScreen();
	// 综合屏初始化显示
	m_screenAdapter->InitIntegScreen();

	log_Screen->WriteLog(LL_INFO, "m_screenAdapter->InitiDev()\n[%s|%d]\n", __FILE__, __LINE__);
	log_Screen->WriteLog(LL_INFO, "m_screenAdapter->InitStripScreen()\n[%s|%d]\n", __FILE__, __LINE__);
	log_Screen->WriteLog(LL_INFO, "m_screenAdapter->InitIntegScreen()\n[%s|%d]\n", __FILE__, __LINE__);

	while (true)
	{
		// 初始化
		memset( recvBuffer     , 0, sizeof(recvBuffer)      );
		memset( stripAddr      , 0, sizeof(stripAddr)       );
		memset( stripScreenInfo, 0, sizeof(stripScreenInfo) );
		memset( stripShowMode  , 0, sizeof(stripShowMode)   );
		memset( IntegScreenInfo, 0, sizeof(IntegScreenInfo) );

		// 读数据到recvBuffer中
		//printf("ScreenThread is Running ...\n");
		cyclBuffer.ReadScreenData( recvBuffer );
		//printf("ScreenDispatch::do_process = [%s]\n", recvBuffer);
		//m_screenAdapter->printClassName();
		//m_screenAdapter->print();

		// 拆解字符串recvBuffer : 读取客户端发送的信息  -> "1|1|请B001到02窗口|00|B001,02"
		// stripAddr            : 条屏地址				->    1	
		// stripScreenInfo      : 条屏显示信息			->		请B001到02窗口
		// stripShowMode        : 条屏显示模式			->					   00
		// IntegScreenInfo      : 综合屏显示信息		->						  B001,02
		ScreenSlipString( recvBuffer, stripAddr, stripScreenInfo, stripShowMode, IntegScreenInfo );

		// 向条屏发送信息
		//m_screenAdapter->SendStripScreen( stripAddr, stripScreenInfo, 0/*stripShowMode*/, NULL/*IntegScreenInfo*/ );
		m_screenAdapter->SendStripScreen( stripAddr, stripScreenInfo, 0, NULL );

		// 向综合屏发送信息
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
	// 关闭串口设备
	m_screenAdapter->CloseDev( );

	return 0;
}

void ScreenDispatch::ScreenSlipString( const char *recvBuffer, char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{	// 0 1       2        3   4
	//"1|1|请B001到02窗口|00|B001,02"

	// 条屏地址 stripAddr
	//ScreenSlipChar( 1, recvBuffer, stripAddr      , '|' );
	SplitStrings::SplitStr( 1, recvBuffer, stripAddr      , '|' );

	// 条屏信息 stripScreenInfo
	//ScreenSlipChar( 2, recvBuffer, stripScreenInfo, '|' );
	SplitStrings::SplitStr( 2, recvBuffer, stripScreenInfo, '|' );

	// 条屏显示模式 stripShowMode
	//ScreenSlipChar( 3, recvBuffer, stripShowMode  , '|' );
	SplitStrings::SplitStr( 3, recvBuffer, stripShowMode  , '|' );

	// 综合屏信息
	//ScreenSlipChar( 4, recvBuffer, IntegScreenInfo, '|' );
	SplitStrings::SplitStr( 4, recvBuffer, IntegScreenInfo, '|' );

	// stripAddr=1, stripScreenInfo="请B001到02窗口, stripShowMode=00, IntegScreenInfo=B001,02
}
