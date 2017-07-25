#include "IMM90061.h"

Implement_DynCreate(IMM90061, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM90061.ini")->GetParamValue(key, valueBuffer)

// 初始化工作
IMM90061::IMM90061(void)
{
	char temp[16];

	m_ComPort1 = 0;
	m_ComPort2 = 0;
	memset(temp, 0, sizeof(temp));
	SCParamValueINI("LOGLEVEL", temp);

	IMM90061Slog = new Log("log\\IMM90061.log", temp);

	// 加载动态库 dlltpzp.dll
	int status = LoadLibrarys( "screen\\IMM90061\\dlltpzp.dll" );
	if (status != 0)
		IMM90061Slog->WriteLog(LL_ERROR, "LoadLibrarys dlltpzp.dll Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM90061Slog->WriteLog(LL_INFO, "LoadLibrarys dlltpzp.dll Success!\n[%s|%d]", __FILE__, __LINE__);

	loadStripScreenInfo();

	loadIntegScreenInfo();

	m_Len = 0;
}

IMM90061::~IMM90061(void)
{
	IMM90061Slog->CloseLogFile();
}

int IMM90061::loadStripScreenInfo( )
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

int IMM90061::loadIntegScreenInfo( )
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
		printf("integScreenSndInfo[%d].addr = [%s]\n", i+1, addr);
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
 *	必须实现的外部API接口，供框架回调  *
 ***************************************/

// 打开串口设备
int  IMM90061::InitiDev( )
{
	char ComPortNum[8];
	memset(ComPortNum, 0, sizeof(ComPortNum));

	SCParamValueINI("COM1", ComPortNum);
	m_ComPort1 = atoi(ComPortNum);
	Com1.InitPort( m_ComPort1 );

	memset(ComPortNum, 0, sizeof(ComPortNum));
	SCParamValueINI("COM2", ComPortNum);
	m_ComPort2 = atoi(ComPortNum);
	Com2.InitPort( m_ComPort2 );

	return 0;
}

// 关闭串口设备
int  IMM90061::CloseDev( )
{
	Com1.ClosePort();
	Com2.ClosePort();
	return 0;
}

// 窗口屏 - 永泰新欣
int  IMM90061::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	int bytes1;
	int bytes2;
	char sendBuffer[1024];

	bytes1 = 0;
	bytes2 = 0;
	memset(sendBuffer, 0, sizeof(sendBuffer));

	char InitMessage[512];
	memset(InitMessage, 0, sizeof(InitMessage));
	
	bytes2 = PackStripScreenMsg(stripScreenInfo, atoi(stripAddr), (unsigned char *)sendBuffer);
	bytes1 = PackInitMessage( atoi(stripAddr), (unsigned char *)InitMessage );

	Com1.WriteToPort(InitMessage, bytes1);
	Com1.WriteToPort(sendBuffer, bytes2);

	return 0;
}

// 综合屏 - 中意恒信
int  IMM90061::SendIntegScreen( char *recvData, int addr, int line, char *other )
{
	int  status;
	char sendData[512];
	char custerNum[8];
	char windowNum[8];

	memset( sendData , 0, sizeof(sendData)  );
	memset( custerNum, 0, sizeof(custerNum) );
	memset( windowNum, 0, sizeof(windowNum) );

	SplitStrings::SplitStr( 0, recvData, custerNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowNum, ',' );
	sprintf( sendData, "请%s到%s窗口", custerNum, windowNum );
	
	for (int i=0; i<m_integSndNums; i++) {
		IMM90061Slog->WriteLog(LL_INFO, "integScreenSndInfo[%d].addr = [%d]\n[%s|%d]", i, integScreenSndInfo[i].addr,  __FILE__, __LINE__);
		status = SendDataZHLine(m_ComPort2, 9600, integScreenSndInfo[i].addr, sendData, 19);
		if (status == 0) {
			IMM90061Slog->WriteLog(LL_INFO, "sendData = [%s] status = [%d]\n[%s|%d]", sendData,  status, __FILE__, __LINE__);
		} else {
			IMM90061Slog->WriteLog(LL_ERROR, "sendData = [%s] status = [%d]\n[%s|%d]", sendData,  status, __FILE__, __LINE__);
		}
	}
	
	return 0;
}

// 条屏初始化显示
int  IMM90061::InitStripScreen( )
{
	IMM90061Slog->WriteLog(LL_DEBUG, "IMM90061::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		//printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, stripScreenDefInfo[i].addr, 3, stripScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM90061::InitStripScreen( )\n");

	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

// 综合屏初始化显示
int  IMM90061::InitIntegScreen( )
{
	IMM90061Slog->WriteLog(LL_DEBUG, "IMM90061::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		//printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM90061::InitIntegScreen( )\n");
	
	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

int  IMM90061::PackInitMessage( int addr, unsigned char *InitMessage )
{
	int bytes = 0;
	int i     = 0;
	
	InitMessage[i++] = 0XAA;
	InitMessage[i++] = 0X55;
	InitMessage[i++] = 0X04;
	InitMessage[i++] = addr;

	InitMessage[i++] = 0XF8;
	InitMessage[i++] = 0X51;
	InitMessage[i++] = m_Len;
	InitMessage[i++] = 0X00;

	return 29;
}

int  IMM90061::PackStripScreenMsg(const char *recvBuffer, int addr, unsigned char *sendBuffer)
{
	int bytes = 0;
	int len   = 0;
 
	sendBuffer[0]  = 0XAA;
	sendBuffer[1]  = 0X55;
	sendBuffer[2]  = 0X00;
	sendBuffer[3]  = addr; // 窗口地址
	sendBuffer[4]  = 0XF8; 
	sendBuffer[5]  = 0X21;
	sendBuffer[6]  = 0X00; 

	memcpy( sendBuffer+7, recvBuffer, strlen(recvBuffer) );

	bytes = 7 + strlen(recvBuffer);
	sendBuffer[bytes++] = 0X00;

	len = bytes - 3;
	sendBuffer[2]  = len ; // 报文长度
	m_Len = strlen(recvBuffer);

	return bytes;
}

// 综合屏中意恒信
int  IMM90061::PackInterScreenMsg(const char *recvBuffer, int addr, unsigned char *sendBuffer)
{	
	return 0;
}

int IMM90061::LoadLibrarys( char *pathname )
{
	HINSTANCE hDll = NULL;
	hDll = LoadLibrary( pathname );
	if (hDll == NULL)
		return -1;

	SendDataZHLine = (pFunSendDataZHLine) GetProcAddress( hDll, "SendDataZHLine" );
	if (SendDataZHLine == NULL)
		return -1;

	return 0;
}