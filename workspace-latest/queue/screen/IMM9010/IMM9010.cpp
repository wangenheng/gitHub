#include "IMM9010.h"

Implement_DynCreate(IMM9010, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM9010.ini")->GetParamValue(key, valueBuffer)

#pragma  warning(disable:4786)
IMM9010::IMM9010(void)
{
	IMM9010Slog = new Log("log\\IMM9010.log", LL_DEBUG);
	IMM9010Slog->WriteLog(LL_DEBUG, "IMM9010::IMM9010(void)");
	// ���ض�̬�� LcLedComm.dll
	int status = LoadLibrarys( "screen\\IMM9010\\CallPad.dll" );
	if (status != 0)
		IMM9010Slog->WriteLog(LL_ERROR, "LoadLibrarys CallPad.dll Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM9010Slog->WriteLog(LL_INFO, "LoadLibrarys CallPad.dll Sucess!\n[%s|%d]", __FILE__, __LINE__);

	// ��ʼ��������
	loadStripScreenInfo();

	// ��ʼ���ۺ���
	loadIntegScreenInfo();

	// add by ������ at 2016-12-3 ����ۺ�����ʾ��ʽ begin
	memset(showFormat, 0, sizeof(showFormat));
	SCParamValueINI("SHOWFORMAT", showFormat);
	IMM9010Slog->WriteLog(LL_DEBUG, "SHOWFORMAT=[%s]\n[%s|%d]", showFormat, __FILE__, __LINE__);
	// add by ������ at 2016-12-3 ����ۺ�����ʾ��ʽ end


	// add by ������ at 2017-7-8 ����ۺ������� begin
	
	char temp[128];
	
	memset(temp, 0, sizeof(temp));
	SCParamValueINI("SNDINTEGROWNUMBERS", temp);
	queueArray = new QueueArray(atoi(temp), 512);
	queueArray->InitQueue();
	printf("�ۺ���������%d\n",queueArray->GetRows());
	// add by ������ at 2017-7-8 ����ۺ������� end

	
}

IMM9010::~IMM9010(void)
{
	IMM9010Slog->CloseLogFile();
}

int IMM9010::loadStripScreenInfo()
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
		context[strlen(context)-1] = 0;

		stripScreenDefInfo[i].addr = atoi(addr);
		strncpy( stripScreenDefInfo[i].defMessage, &context[1], sizeof(stripScreenDefInfo[i].defMessage)-1 );
		stripScreenDefInfo[i].defMessage[sizeof(stripScreenDefInfo[i].defMessage)-1] = 0;
	}

	return 1;
}

// �����ۺ�������
int IMM9010::loadIntegScreenInfo()
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
		context[strlen(context)-1] = 0;

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
int  IMM9010::InitiDev( )
{
	bool status;
	char comPort[8];

	memset( comPort , 0, sizeof(comPort)  );

	SCParamValueINI( "COM", comPort );
	status = OpenPort( atoi(comPort) );
	if ( !status ) {
		IMM9010Slog->WriteLog(LL_ERROR, "�򿪴����豸:COM=[%s]\n[%s|%d]", comPort, __FILE__, __LINE__);
		return -1;
	}
	IMM9010Slog->WriteLog(LL_DEBUG, "�򿪴����豸:COM=[%s]\n[%s|%d]", comPort, __FILE__, __LINE__);
	return 0;
}

// �رմ����豸
int  IMM9010::CloseDev( )
{
	ClosePort( );
	return 0;
}

// ������������Ϣ
int  IMM9010::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	char temp[128];
	char sendBuffer[512];

	memset(temp,	   0, sizeof ( temp)      );
	memset(sendBuffer, 0, sizeof ( sendBuffer));


	SCParamValueINI( "STRIPINFO", temp );
	//printf("%s\n",temp);
	sprintf(sendBuffer, "%s%s", temp, stripScreenInfo);
	//sprintf(sendBuffer, "\\3\\8%s",stripScreenInfo);
	//SendContent    (int Addr,        int lineno, char *Content,char algin,char ledlen);
	lpSendStripScreen(atoi(stripAddr), 1,          sendBuffer,   '0',       '8');
	IMM9010Slog->WriteLog(LL_DEBUG, "SendStripScreen:[%s]\n[%s|%d]", sendBuffer, __FILE__, __LINE__);
	return 0;
}

// ���ۺ���������Ϣ
int  IMM9010::SendIntegScreen( char *recvData, int addr, int line, char *other )
{
	int  status;
	char sendData[512];
	char realSendData[4096];
	string sendDatas;
	char custerNum[8];
	char windowNum[8];
	char preStr[16];
	char midStr[16];
	char endStr[16];
	string m_sendData;

	memset( sendData ,   0, sizeof(sendData)    );
	memset( realSendData,0, sizeof(realSendData));
	memset( custerNum,   0, sizeof(custerNum)   );
	memset( windowNum,   0, sizeof(windowNum)   ); 
	memset( preStr, 0, sizeof(preStr) );
	memset( midStr, 0, sizeof(midStr) );
	memset( endStr, 0, sizeof(endStr) );

	sendDatas = "";

	SplitStrings::SplitStr( 0, recvData, custerNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowNum, ',' );

	// �ֽ��ۺ�����ʾ��ʽ
	SplitStrings::SplitStr( 0, showFormat, preStr, 'X' );
	SplitStrings::SplitStr( 1, showFormat, midStr, 'X' );
	SplitStrings::SplitStr( 2, showFormat, endStr, 'X' );
	sprintf( sendData, "^speed:0^^col:3^%s^col:1^%s^col:3^%s^col:1^%s^col:3^%s", preStr, custerNum, midStr, windowNum, endStr );
	m_sendData = sendData;

	queueArray->EnQueue( m_sendData );
	for (int i=0; i<m_integSndNums; i++) {
		//status = lpSendIntegScreen(integScreenSndInfo[i].addr, 255, sendData);
		for( int j=0; j<queueArray->GetNowRow(); j++ )
		{
			sendDatas+=queueArray->VisQueueStr(j);
		}

		sendDatas.copy(realSendData, sendDatas.length(), 0);
		*(realSendData+sendDatas.length())='\0';
	
		status = lpSendIntegScreen(integScreenSndInfo[i].addr,0,0,128,64,realSendData);
		if (status != 0) {
			IMM9010Slog->WriteLog(LL_ERROR, "SendIntegScreen = [%s]\n[%s|%d]", sendData, __FILE__, __LINE__);
			return status;
		}
		IMM9010Slog->WriteLog(LL_DEBUG, "SendIntegScreen = [%s]\n[%s|%d]", sendData, __FILE__, __LINE__);
	}
	
	
	return 0;
}

// ��������ʼ����ʾ
int  IMM9010::InitStripScreen( )
{	
	IMM9010Slog->WriteLog(LL_DEBUG, "IMM9010::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		//printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
	}
	
	return 0;
}

// �ۺ�����ʼ����ʾ
int  IMM9010::InitIntegScreen( )
{	
	IMM9010Slog->WriteLog(LL_DEBUG, "IMM9010::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		//printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
	}
	
	return 0;
}

int  IMM9010::LoadLibrarys( char *pathname )
{
	HINSTANCE hDll = NULL;
	hDll = LoadLibrary( pathname );
	if (hDll == NULL)
		return -1;

	OpenPort  = (LpInitPort) GetProcAddress(hDll, "Connect");
	if (OpenPort  == NULL)
		return -1;

//	ClosePort = (LpClosePort)GetProcAddress(hDll, " Disconnect");
//	if (ClosePort == NULL)
//		return -1;

	lpSendStripScreen  = (LpSendStripScreen)GetProcAddress(hDll, "SendContent");
	if (SendStripScreen == NULL)
		return -1;

	lpSendIntegScreen  = (LPSendIntegScreen)GetProcAddress(hDll, "COMP_Led_SendContent");
	if (SendIntegScreen == NULL)
		return -1;

	return 0;
}
