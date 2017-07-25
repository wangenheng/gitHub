#include "IMM90EQ.h"
//#include "DataStruct.h"

Implement_DynCreate(IMM90EQ, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM90EQ.ini")->GetParamValue(key, valueBuffer)

// add by getiancheng at 2016-11-27 begin
//CRITICAL_SECTION g_csA;
//HDC				 g_GDC;
//float AlpaCount( const char* srcStr );
// add by getiancheng at 2016-11-27 end

IMM90EQ::IMM90EQ(void)
{
	char temp[16];

	//InitializeCriticalSection( &g_csA );
	memset(temp, 0, sizeof(temp));
	SCParamValueINI("LOGLEVEL", temp);
	// ��־
	IMM90EQSlog = new Log("log\\IMM90EQ.log", temp);
	IMM90EQSlog->WriteLog(LL_DEBUG, "IMM90EQ::IMM90EQ(void)");

	// ���ض�̬�� dlltpzp.dll EV_SocketUdpDll.dll
	int status = LoadLibrarys( );
	if (status != 0)
		IMM90EQSlog->WriteLog(LL_ERROR, "LoadLibrarys  Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM90EQSlog->WriteLog(LL_INFO, "LoadLibrarys  Success!\n[%s|%d]", __FILE__, __LINE__);

	// ���ش���������
	loadStripScreenInfo();

	// �����ۺ�������
	loadIntegScreenInfo();

	// add by ����� at 2016-11-23 ����ۺ�����ʾ��ʽ begin
	memset(showFormat, 0, sizeof(showFormat));
	SCParamValueINI("SHOWFORMAT", showFormat);
	IMM90EQSlog->WriteLog(LL_DEBUG, "SHOWFORMAT=[%s]\n[%s|%d]", showFormat, __FILE__, __LINE__);
	// add by ����� at 2016-11-23 ����ۺ�����ʾ��ʽ end

	InitMapWinAddr( );
	IMM90EQSlog->WriteLog(LL_DEBUG, "InitMapWinAddr\n[%s|%d]",  __FILE__, __LINE__);
	InitMapWinRegion();
	IMM90EQSlog->WriteLog(LL_DEBUG, "InitMapWinRegion\n[%s|%d]",  __FILE__, __LINE__);
	InitMapFont();
	IMM90EQSlog->WriteLog(LL_DEBUG, "InitMapFont\n[%s|%d]",    __FILE__, __LINE__);

	for(int i = 1;i<4;i++)
	{
		
	//	cout<<mapFont[i]->strFontName<<"  "<< mapFont[i]->iFontSize<<"  "<< mapFont[i]->bFontBold<<"  "<<\
	//		mapFont[i]->bFontItaic<<"  "<< mapFont[i]->bFontUnderline<<"  "<< mapFont[i]->colorFont<<"  "<<\
	//		mapFont[i]->iAlignStyle<<"  "<<mapFont[i]->iVAlignerStyle<<"  "<<mapFont[i]->iRowSpace<<endl;

		cout<<mapFont[i]->strFontName<<endl;
	}
}

IMM90EQ::~IMM90EQ(void)
{
	// �ر���־
	IMM90EQSlog->CloseLogFile();
}

// ���ش���������
int IMM90EQ::loadStripScreenInfo()
{
	char temp[128];
	char stripScreen[128];
	char strFormat[64];
	char addr[16];
	char context[32];

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("INITSTRIPSCREEN", temp);
	m_initStrip = atoi(temp)==1 ? true : false;
	if (m_initStrip == false)
		return 0;

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("STRIPNUMBERS", temp);
	m_stripDefNums = atoi(temp);
	for (int i=0; i<m_stripDefNums; i++) {
		memset( addr       , 0, sizeof(addr)        );
		memset( context    , 0, sizeof(context)     );
		memset( stripScreen, 0, sizeof(stripScreen) );
		memset( strFormat  , 0, sizeof(strFormat)   );

		sprintf(strFormat, "STRIPSCREEN%d", i+1);
		SCParamValueINI(strFormat, stripScreen);

		SplitStrings::SplitStr( 0, stripScreen, addr   , '|' );
		SplitStrings::SplitStr( 1, stripScreen, context, '|' );
		context[strlen(context)] = 0; // ȥ��β�� "

		stripScreenDefInfo[i].addr = atoi(addr);
		strncpy( stripScreenDefInfo[i].defMessage, &context[1], sizeof(stripScreenDefInfo[i].defMessage)-1 );
		stripScreenDefInfo[i].defMessage[sizeof(stripScreenDefInfo[i].defMessage)-1] = 0;
	}

	return 1;
}

// �����ۺ�������
int IMM90EQ::loadIntegScreenInfo()
{
	char temp[128];
	char integScreen[128];
	char strFormat[64];
	char addr[16];
	char context[32];
	int  i;

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("SNDINTEGNUMBERS", temp);
	m_integSndNums = atoi(temp);
	for (i=0; i<m_integSndNums; i++) {
		memset( addr     , 0, sizeof(addr)      );
		memset( strFormat, 0, sizeof(strFormat) );

		sprintf(strFormat, "SNDINTEGSCREEN%d", i+1);
		SCParamValueINI(strFormat, addr);

		integScreenSndInfo[i].addr = atoi(addr);
		//printf("integScreenSndInfo[%d].addr = [%s]\n", i+1, addr);
	}
	memset(temp, 0, sizeof(temp));
	SCParamValueINI("INITINTEGSCREEN", temp);
	m_initInteg = atoi(temp)==1 ? true : false;
	if (m_initInteg == false)
		return 0;

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("INTEGNUMBERS", temp);
	m_integDefNums = atoi(temp);
	for (i=0; i<m_integDefNums; i++) {
		memset( addr       , 0, sizeof(addr)        );
		memset( context    , 0, sizeof(context)     );
		memset( integScreen, 0, sizeof(integScreen) );
		memset( strFormat  , 0, sizeof(strFormat)   );

		sprintf(strFormat, "INTEGSCREEN%d", i+1);
		SCParamValueINI(strFormat, integScreen);

		SplitStrings::SplitStr( 0, integScreen, addr   , '|' );
		SplitStrings::SplitStr( 1, integScreen, context, '|' );
		context[strlen(context)-1] = 0; // ȥ��β�� "

		integScreenDefInfo[i].addr = atoi(addr);
		strncpy( integScreenDefInfo[i].defMessage, &context[1], sizeof(integScreenDefInfo[i].defMessage)-1 );
		integScreenDefInfo[i].defMessage[sizeof(integScreenDefInfo[i].defMessage)-1] = 0;
	}

	return 1;
}

/***************************************
 *	����ʵ�ֵ��ⲿAPI�ӿڣ�����ܻص�  *
 ***************************************/

// �򿪴����豸
int  IMM90EQ::InitiDev( )
{
	char comPort[8];
	char comBaund[16];
	char showColor[8];
	char showMode[8];
	char showTime[8];
	char showSpeed[8];

	memset( comPort  , 0, sizeof(comPort)   );
	memset( comBaund , 0, sizeof(comBaund)  );
	memset( showColor, 0, sizeof(showColor) );
	memset( showMode , 0, sizeof(showMode)  );
	memset( showTime , 0, sizeof(showTime)  );
	memset( showSpeed, 0, sizeof(showSpeed) );

	SCParamValueINI( "COM"      , comPort   );
	SCParamValueINI( "SENDBAUND", comBaund  );
	SCParamValueINI( "SHOWCOLOR", showColor );
	SCParamValueINI( "SHOWMODE" , showMode  );
	SCParamValueINI( "SHOWTIME" , showTime  );
	SCParamValueINI( "SHOWSPEED", showSpeed );

	m_ComPort   = atoi( comPort   );
	m_ComBaund  = atoi( comBaund  );
	m_ShowColor = atoi( showColor );
	m_ShowMode  = atoi( showMode  );
	m_ShowTime  = atoi( showTime  );
	m_ShowSpeed = atoi( showSpeed );

	IMM90EQSlog->WriteLog(LL_DEBUG, "���� :COM = [%s], ������    = [%s]\n[%s|%d]", comPort, comBaund, __FILE__, __LINE__);
	IMM90EQSlog->WriteLog(LL_DEBUG, "showColor = [%s], showMode  = [%s]\n[%s|%d]", showColor, showMode, __FILE__, __LINE__);
	IMM90EQSlog->WriteLog(LL_DEBUG, "showTime  = [%s], showSpeed = [%s]\n[%s|%d]", showTime, showSpeed, __FILE__, __LINE__);

	return 0;
}

// �رմ����豸
int  IMM90EQ::CloseDev( )
{
	return 0;
}

// ������������Ϣ
int  IMM90EQ::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	int status = 0;

	int winAddr;
	char FontName[256];

	memset( FontName, 0, sizeof(FontName) );

	//EnterCriticalSection( &g_csA );
	winAddr   = atoi(stripAddr);
	if(!sendInfo(stripScreenInfo,winAddr,3))
	{
		IMM90EQSlog->WriteLog(LL_ERROR, "sendData = [%s] status = [%d]\n[%s|%d]", stripScreenInfo,  status, __FILE__, __LINE__);
	}
	else
		IMM90EQSlog->WriteLog(LL_INFO, "sendData = [%s] status = [%d]\n[%s|%d]", stripScreenInfo,  status, __FILE__, __LINE__);

	return 0;
}

// ���ۺ���������Ϣ
int  IMM90EQ::SendIntegScreen( char *recvData, int addr, int line, char *other )
{
	
	return 0;
}

// ��������ʼ����ʾ
int  IMM90EQ::InitStripScreen( )
{	
	IMM90EQSlog->WriteLog(LL_DEBUG, "IMM90EQ::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	char info[256];
	memset(info,0,sizeof(info));
	for (int i=1; i<=mapWinRegion.size(); i++) {

//		if(i==14) continue;
		for(int j = 1; j<4; j++)
		{
			if(j == 1) sprintf(info,"%d",i);
			if(j == 2) sprintf(info,"%s",mapWinRegion[i]->dispMsg);
			if(j == 3) sprintf(info,"%s","��ӭ����");
			
			sendInfo(info,i,j);
			
		}	
	}
	
	return 0;
}

// �ۺ�����ʼ����ʾ
int  IMM90EQ::InitIntegScreen( )
{	
//	IMM90EQSlog->WriteLog(LL_DEBUG, "IMM90EQ::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
//	for (int i=0; i<m_integDefNums; i++) {
//		printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
//		SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
//	}
	
	return 0;
}

int  IMM90EQ::LoadLibrarys( )
{
	HINSTANCE hDll;
	hDll = LoadLibrary("screen\\IMM90EQ\\EQ2008_Dll.dll");
	if(hDll != NULL)
	{
		realtimeConnect = (lpUser_RealtimeConnect)GetProcAddress(hDll,"User_RealtimeConnect");
		if(realtimeConnect == NULL)
		{
			IMM90EQSlog->WriteLog(LL_ERROR, "Load Function User_RealtimeConnect Error!\n[%s|%d]", __FILE__, __LINE__);
			return 1;
		}
		realtimeSendText = (lpUser_RealtimeSendText)GetProcAddress(hDll,"User_RealtimeSendText");
		if(realtimeSendText == NULL)
		{
			IMM90EQSlog->WriteLog(LL_ERROR, "Load Function User_RealtimeSendText Error!\n[%s|%d]", __FILE__, __LINE__);
			return 1;
		}
		realtimeDisConnect = (lpUser_RealtimeDisConnect)GetProcAddress(hDll,"User_RealtimeDisConnect");
		if(realtimeDisConnect == NULL)
		{
			IMM90EQSlog->WriteLog(LL_ERROR, "Load Function User_RealtimeDisConnect Error!\n[%s|%d]", __FILE__, __LINE__);
			return 1;
		}
	}
	else
		return 1;

	return 0;
}
bool IMM90EQ::sendInfo( char *recvData, int addr,int type)
{
	printf("%s\n",recvData);

	mapFont[type]->strFontName = "����";



	if(!realtimeConnect(mapWinAdr[addr]))
	{
		IMM90EQSlog->WriteLog(LL_ERROR, "Connect Error!\n[%s|%d]", __FILE__, __LINE__);
		return 0;
	}
	IMM90EQSlog->WriteLog(LL_INFO, "Connect Success!\n[%s|%d]", __FILE__, __LINE__);
	//printf("Error Info: %s %s %d  \n",recvData,mapFont[type]->strFontName,mapFont[type]->colorFont);




	if( type == 1)
	{
		if(!realtimeSendText(mapWinAdr[addr],mapWinRegion[addr]->numX,mapWinRegion[addr]->numY,mapWinRegion[addr]->numWidth,mapWinRegion[addr]->numHeight,\
						recvData,mapFont[type]))
		{
			IMM90EQSlog->WriteLog(LL_ERROR, "realtimesend(%s) Error!\n[%s|%d]", recvData,  __FILE__, __LINE__);
			if(!realtimeDisConnect(mapWinAdr[addr]))
			{
				IMM90EQSlog->WriteLog(LL_ERROR, "Disconnect Error!\n[%s|%d]", __FILE__, __LINE__);
				return 0;
			}
			IMM90EQSlog->WriteLog(LL_INFO, "realtimesend(%s) Success!\n[%s|%d]",  recvData, __FILE__, __LINE__);
			return 0;
		}		
	}



	if( type == 2)
	{
		if(!realtimeSendText(mapWinAdr[addr],mapWinRegion[addr]->busnsX,mapWinRegion[addr]->busnsY,mapWinRegion[addr]->busnsWidth,mapWinRegion[addr]->busnsHeight,\
						recvData,mapFont[type]))
		{
			IMM90EQSlog->WriteLog(LL_ERROR, "realtimesend(%s) Error!\n[%s|%d]", recvData,  __FILE__, __LINE__);
			if(!realtimeDisConnect(mapWinAdr[addr]))
			{
				IMM90EQSlog->WriteLog(LL_ERROR, "Disconnect Error!\n[%s|%d]", __FILE__, __LINE__);
				return 0;
			}
			IMM90EQSlog->WriteLog(LL_INFO, "realtimesend(%s) Success!\n[%s|%d]",  recvData, __FILE__, __LINE__);
			return 0;
		}
	}



	if( type == 3)
	{
		printf("Error Info: %s %s %d  \n",recvData,mapFont[type]->strFontName,mapFont[type]->colorFont);
		if(!realtimeSendText(mapWinAdr[addr],mapWinRegion[addr]->callX,mapWinRegion[addr]->callY,mapWinRegion[addr]->callWidth,mapWinRegion[addr]->callHeight,\
			recvData,mapFont[type]))
		{
			IMM90EQSlog->WriteLog(LL_ERROR, "realtimesend(%s) Error!\n[%s|%d]", recvData,  __FILE__, __LINE__);
			if(!realtimeDisConnect(mapWinAdr[addr]))
			{
				IMM90EQSlog->WriteLog(LL_ERROR, "Disconnect Error!\n[%s|%d]", __FILE__, __LINE__);
				return 0;
			}
			IMM90EQSlog->WriteLog(LL_INFO, "realtimesend(%s) Success!\n[%s|%d]",  recvData, __FILE__, __LINE__);
			return 0;
		}
	}
	

	if(!realtimeDisConnect(mapWinAdr[addr]))
	{
		IMM90EQSlog->WriteLog(LL_ERROR, "Disconnect Error!\n[%s|%d]", __FILE__, __LINE__);
		return 0;
	}
	IMM90EQSlog->WriteLog(LL_INFO, "Disconnect Success!\n[%s|%d]", __FILE__, __LINE__);
	return 1;
}