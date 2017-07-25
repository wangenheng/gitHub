#include "IMM9004.h"

Implement_DynCreate(IMM9004, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM9004.ini")->GetParamValue(key, valueBuffer)

IMM9004::IMM9004(void)
{
	IMM9004Slog = new Log("log\\IMM9004.log", LL_DEBUG);
	IMM9004Slog->WriteLog(LL_DEBUG, "IMM9004::IMM9004(void)");
	// 加载动态库 LcLedComm.dll
	int status = LoadLibrarys( "screen\\IMM9004\\KeyPad.dll" );
	if (status != 0)
		IMM9004Slog->WriteLog(LL_ERROR, "LoadLibrarys LcLedComm.dll Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM9004Slog->WriteLog(LL_INFO, "LoadLibrarys LcLedComm.dll Sucess!\n[%s|%d]", __FILE__, __LINE__);

	// 初始化窗口屏
	loadStripScreenInfo();

	// 初始化综合屏
	loadIntegScreenInfo();

	// add by 王恩享 at 2016-12-3 添加综合屏显示格式 begin
	memset(showFormat, 0, sizeof(showFormat));
	SCParamValueINI("SHOWFORMAT", showFormat);
	IMM9004Slog->WriteLog(LL_DEBUG, "SHOWFORMAT=[%s]\n[%s|%d]", showFormat, __FILE__, __LINE__);
	// add by 王恩享 at 2016-12-3 添加综合屏显示格式 end
}

IMM9004::~IMM9004(void)
{
	IMM9004Slog->CloseLogFile();
}

int IMM9004::loadStripScreenInfo()
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

// 加载综合屏配置
int IMM9004::loadIntegScreenInfo()
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
 *	必须实现的外部API接口，供框架回调  *
 ***************************************/

// 打开串口设备
int  IMM9004::InitiDev( )
{
	bool status;
	char comPort[8];

	memset( comPort , 0, sizeof(comPort)  );

	SCParamValueINI( "COM", comPort );
	status = OpenPort( atoi(comPort) );
	if ( !status ) {
		IMM9004Slog->WriteLog(LL_ERROR, "打开串口设备:COM=[%s]\n[%s|%d]", comPort, __FILE__, __LINE__);
		return -1;
	}
	IMM9004Slog->WriteLog(LL_DEBUG, "打开串口设备:COM=[%s]\n[%s|%d]", comPort, __FILE__, __LINE__);
	return 0;
}

// 关闭串口设备
int  IMM9004::CloseDev( )
{
	ClosePort( );
	return 0;
}

// 向条屏发送信息
int  IMM9004::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	char sendBuffer[512];

	memset(sendBuffer, 0, sizeof(sendBuffer));
	sprintf(sendBuffer, "\\1\\9\\3%s", stripScreenInfo);
	lpSendStripScreen(atoi(stripAddr), sendBuffer);

	return 0;
}

// 向综合屏发送信息
int  IMM9004::SendIntegScreen( char *recvData, int addr, int line, char *other )
{
	int  status;
	char sendData[512];
	char custerNum[8];
	char windowNum[8];
	char preStr[16];
	char midStr[16];
	char endStr[16];

	memset( sendData , 0, sizeof(sendData)  );
	memset( custerNum, 0, sizeof(custerNum) );
	memset( windowNum, 0, sizeof(windowNum) );
	memset( preStr, 0, sizeof(preStr) );
	memset( midStr, 0, sizeof(midStr) );
	memset( endStr, 0, sizeof(endStr) );

	SplitStrings::SplitStr( 0, recvData, custerNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowNum, ',' );

	// 分解综合屏显示格式
	SplitStrings::SplitStr( 0, showFormat, preStr, 'X' );
	SplitStrings::SplitStr( 1, showFormat, midStr, 'X' );
	SplitStrings::SplitStr( 2, showFormat, endStr, 'X' );
	sprintf( sendData, "%s%s%s%s%s", preStr, custerNum, midStr, windowNum, endStr );

	//printf("%s\n",sendData);
	//sprintf( sendData, "请%s到%s窗口", custerNum, windowNum );
	for (int i=0; i<m_integSndNums; i++) {
		status = lpSendIntegScreen(integScreenSndInfo[i].addr, 255, sendData);
		if (status != 0) {
			IMM9004Slog->WriteLog(LL_ERROR, "SendIntegScreen = [%s]\n[%s|%d]", sendData, __FILE__, __LINE__);
			return status;
		}
		IMM9004Slog->WriteLog(LL_DEBUG, "SendIntegScreen = [%s]\n[%s|%d]", sendData, __FILE__, __LINE__);
	}
	
	
	return 0;
}

// 窗口屏初始化显示
int  IMM9004::InitStripScreen( )
{	
	IMM9004Slog->WriteLog(LL_DEBUG, "IMM9004::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		//printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
	}
	
	return 0;
}

// 综合屏初始化显示
int  IMM9004::InitIntegScreen( )
{	
	IMM9004Slog->WriteLog(LL_DEBUG, "IMM9004::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		//printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
	}
	
	return 0;
}

int  IMM9004::LoadLibrarys( char *pathname )
{
	HINSTANCE hDll = NULL;
	hDll = LoadLibrary( pathname );
	if (hDll == NULL)
		return -1;

	OpenPort  = (LpInitPort) GetProcAddress(hDll, "initPort");
	if (OpenPort  == NULL)
		return -1;

	ClosePort = (LpClosePort)GetProcAddress(hDll, "closePort");
	if (ClosePort == NULL)
		return -1;

	lpSendStripScreen  = (LpSendStripScreen)GetProcAddress(hDll, "SendWireLessLed");
	if (SendStripScreen == NULL)
		return -1;

	lpSendIntegScreen  = (LPSendIntegScreen)GetProcAddress(hDll, "SendColorTp");
	if (SendIntegScreen == NULL)
		return -1;

	return 0;
}
