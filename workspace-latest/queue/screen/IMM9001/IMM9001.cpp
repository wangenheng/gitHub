#include "IMM9001.h"

Implement_DynCreate(IMM9001, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM9001.ini")->GetParamValue(key, valueBuffer)

IMM9001::IMM9001(void)
{
	char temp[16];

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("LOGLEVEL", temp);
	// 日志
	IMM9001Slog = new Log("log\\IMM9001.log", temp);
	IMM9001Slog->WriteLog(LL_DEBUG, "IMM9001::IMM9001(void)");

	// 加载动态库 dlltpzp.dll
	int status = LoadLibrarys( "screen\\IMM9001\\dlltpzp.dll" );
	if (status != 0)
		IMM9001Slog->WriteLog(LL_ERROR, "LoadLibrarys dlltpzp.dll Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM9001Slog->WriteLog(LL_INFO, "LoadLibrarys dlltpzp.dll Success!\n[%s|%d]", __FILE__, __LINE__);

	// 加载窗口屏配置
	loadStripScreenInfo();

	// 加载综合屏配置
	loadIntegScreenInfo();

	// add by 葛天成 at 2016-11-23 添加综合屏显示格式 begin
	memset(showFormat, 0, sizeof(showFormat));
	SCParamValueINI("SHOWFORMAT", showFormat);
	IMM9001Slog->WriteLog(LL_DEBUG, "SHOWFORMAT=[%s]\n[%s|%d]", showFormat, __FILE__, __LINE__);
	// add by 葛天成 at 2016-11-23 添加综合屏显示格式 end
	// 分解综合屏显示格式
	/*
	char preStr[16];
	char midStr[16];
	char endStr[16];

	memset( preStr, 0, sizeof(preStr) );
	memset( midStr, 0, sizeof(midStr) );
	memset( endStr, 0, sizeof(endStr) );
	SplitStrings::SplitStr( 0, showFormat, preStr, 'X' );
	SplitStrings::SplitStr( 1, showFormat, midStr, 'X' );
	SplitStrings::SplitStr( 2, showFormat, endStr, 'X' );
	IMM9001Slog->WriteLog(LL_DEBUG, "preStr = [%s]\nmidStr = [%s]\nendStr = [%s]\n[%s|%d]", preStr, midStr, endStr,  __FILE__, __LINE__);
	*/
}

IMM9001::~IMM9001(void)
{
	// 关闭日志
	IMM9001Slog->CloseLogFile();
}

// 加载窗口屏配置
int IMM9001::loadStripScreenInfo()
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
		context[strlen(context)] = 0; // 去掉尾端 "

		stripScreenDefInfo[i].addr = atoi(addr);
		strncpy( stripScreenDefInfo[i].defMessage, &context[1], sizeof(stripScreenDefInfo[i].defMessage)-1 );
		stripScreenDefInfo[i].defMessage[sizeof(stripScreenDefInfo[i].defMessage)-1] = 0;
	}

	return 1;
}

// 加载综合屏配置
int IMM9001::loadIntegScreenInfo()
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
		context[strlen(context)-1] = 0; // 去掉尾端 "

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
int  IMM9001::InitiDev( )
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

	IMM9001Slog->WriteLog(LL_DEBUG, "串口 :COM = [%s], 波特率    = [%s]\n[%s|%d]", comPort, comBaund, __FILE__, __LINE__);
	IMM9001Slog->WriteLog(LL_DEBUG, "showColor = [%s], showMode  = [%s]\n[%s|%d]", showColor, showMode, __FILE__, __LINE__);
	IMM9001Slog->WriteLog(LL_DEBUG, "showTime  = [%s], showSpeed = [%s]\n[%s|%d]", showTime, showSpeed, __FILE__, __LINE__);

	return 0;
}

// 关闭串口设备
int  IMM9001::CloseDev( )
{
	return 0;
}

// 向条屏发送信息
int  IMM9001::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	int status;

	status = SendDatafun( m_ComPort, m_ComBaund, atoi(stripAddr), stripScreenInfo, m_ShowColor, m_ShowMode, m_ShowTime, m_ShowSpeed );
	if (status == 0) {
		IMM9001Slog->WriteLog(LL_INFO, "SendStripScreen = [%s] status = [%d]\n[%s|%d]", stripScreenInfo, status,  __FILE__, __LINE__);
	} else {
		IMM9001Slog->WriteLog(LL_ERROR, "SendStripScreen = [%s] status = [%d]\n[%s|%d]", stripScreenInfo, status,  __FILE__, __LINE__);
	}

	return 0;
}

// 向综合屏发送信息
int  IMM9001::SendIntegScreen( char *recvData, int addr, int line, char *other )
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

	// 分解客户号，窗口号
	SplitStrings::SplitStr( 0, recvData, custerNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowNum, ',' );
	//sprintf( sendData, "    请%s号到%s窗口办理业务", custerNum, windowNum );
	//sprintf( sendData, "请%s到%s窗口", custerNum, windowNum );
	// version 0.1
	/*
	sprintf(strWindowNum, "%02d", windowNum);
	//custerNum
	//showFormat
	memcpy(strstr(showFormat, "XXXX"), custerNum, strlen("XXXX"));
	memcpy(strstr(showFormat, "XX"), strWindowNum, strlen("XX"));
	IMM9001Slog->WriteLog(LL_DEBUG, "showFormat= [%s]\n[%s|%d]", showFormat,  __FILE__, __LINE__);
	*/
	// 分解综合屏显示格式
	SplitStrings::SplitStr( 0, showFormat, preStr, 'X' );
	SplitStrings::SplitStr( 1, showFormat, midStr, 'X' );
	SplitStrings::SplitStr( 2, showFormat, endStr, 'X' );
	sprintf( sendData, "%s%s%s%s%s", preStr, custerNum, midStr, windowNum, endStr );



	
	for (int i=0; i<m_integSndNums; i++) {
		IMM9001Slog->WriteLog(LL_INFO, "integScreenSndInfo[%d].addr = [%d]\n[%s|%d]", i, integScreenSndInfo[i].addr,  __FILE__, __LINE__);
		status = SendDataZHLine(m_ComPort, m_ComBaund, integScreenSndInfo[i].addr, sendData, 19);
		if (status == 0) {
			IMM9001Slog->WriteLog(LL_INFO, "sendData = [%s] status = [%d]\n[%s|%d]", sendData,  status, __FILE__, __LINE__);
		} else {
			IMM9001Slog->WriteLog(LL_ERROR, "sendData = [%s] status = [%d]\n[%s|%d]", sendData,  status, __FILE__, __LINE__);
		}
		//if (status != 0)
		//	return status;
	}
	
	
	return 0;
}

// 窗口屏初始化显示
int  IMM9001::InitStripScreen( )
{	
	IMM9001Slog->WriteLog(LL_DEBUG, "IMM9001::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
		SendDataOnly( m_ComPort, m_ComBaund, stripScreenDefInfo[i].addr, 3, stripScreenDefInfo[i].defMessage, 0x11 );
	}
	
	return 0;
}

// 综合屏初始化显示
int  IMM9001::InitIntegScreen( )
{	
	IMM9001Slog->WriteLog(LL_DEBUG, "IMM9001::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
		SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
	}
	
	return 0;
}

int  IMM9001::LoadLibrarys( char *pathname )
{
	HINSTANCE hDll = NULL;
	hDll = LoadLibrary( pathname );
	if (hDll == NULL)
		return -1;

	SendDatafun    = (pFunSendDatafun) GetProcAddress( hDll, "SendDatafun" );
	if (SendDatafun  == NULL)
		return -1;

	SendDataZHLine = (pFunSendDataZHLine) GetProcAddress( hDll, "SendDataZHLine" );
	if (SendDataZHLine == NULL)
		return -1;

	SendDataOnly   = (pFunSendDataOnly) GetProcAddress(hDll, "SendDataOnly");
	if (SendDataOnly == NULL)
		return -1;

	return 0;
}
