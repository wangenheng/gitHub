#include "IMMTW45.h"
#include "DataStruct.h"

Implement_DynCreate(IMMTW45, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMMTW45.ini")->GetParamValue(key, valueBuffer)

// add by getiancheng at 2016-11-27 begin
CRITICAL_SECTION g_csA;
HDC				 g_GDC;
float AlpaCount( const char* srcStr );
// add by getiancheng at 2016-11-27 end

IMMTW45::IMMTW45(void)
{
	char temp[16];

	InitializeCriticalSection( &g_csA );
	memset(temp, 0, sizeof(temp));
	SCParamValueINI("LOGLEVEL", temp);
	// 日志
	IMMTW45Slog = new Log("log\\IMMTW45.log", temp);
	IMMTW45Slog->WriteLog(LL_DEBUG, "IMMTW45::IMMTW45(void)");

	// 加载动态库 dlltpzp.dll EV_SocketUdpDll.dll
	int status = LoadLibrarys( );
	if (status != 0)
		IMMTW45Slog->WriteLog(LL_ERROR, "LoadLibrarys dlltpzp.dll EV_SocketUdpDll.dll Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMMTW45Slog->WriteLog(LL_INFO, "LoadLibrarys dlltpzp.dll EV_SocketUdpDll.dll Success!\n[%s|%d]", __FILE__, __LINE__);

	// 加载窗口屏配置
	loadStripScreenInfo();

	// 加载综合屏配置
	loadIntegScreenInfo();

	// add by 葛天成 at 2016-11-23 添加综合屏显示格式 begin
	memset(showFormat, 0, sizeof(showFormat));
	SCParamValueINI("SHOWFORMAT", showFormat);
	IMMTW45Slog->WriteLog(LL_DEBUG, "SHOWFORMAT=[%s]\n[%s|%d]", showFormat, __FILE__, __LINE__);
	// add by 葛天成 at 2016-11-23 添加综合屏显示格式 end

	InitMapWinAdr( );
	IMMTW45Slog->WriteLog(LL_DEBUG, "InitMapWinAdr\n[%s|%d]",  __FILE__, __LINE__);
	InitMapIpAddr( );
	IMMTW45Slog->WriteLog(LL_DEBUG, "InitMapIpAddr\n[%s|%d]",  __FILE__, __LINE__);

	InitSendPic( );
	IMMTW45Slog->WriteLog(LL_DEBUG, "InitSendPic\n[%s|%d]",    __FILE__, __LINE__);
}

// 初始化插播
void IMMTW45::InitSendPic( )
{
	int  index;
	int  winOffset;
	char FontName[256];
	char DisplayMsg[256];
	int  status;
	
	EnterCriticalSection( &g_csA );
	for ( index=1; index<=4; index++ )
	{
		winOffset     = mapIp[mapWinAddr[index]]->winOffset;

		int X         = mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].X;
		int Y         = mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].Y;
		int Width     = mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].Width;
		int Height    = mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].Height;
		int FontSize  = mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].FontSize;

		memset( FontName, 0, sizeof(FontName) );
		strncpy( FontName, mapIp[mapWinAddr[index]]->hdcStyle[(index - winOffset)*2].FontName, \
			strlen(mapIp[mapWinAddr[index]]->hdcStyle[(index - winOffset)*2].FontName) );
		FontName[strlen(mapIp[mapWinAddr[index]]->hdcStyle[(index - winOffset)*2].FontName)] = 0;

		int FontColor = mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].FontColor;

		memset( DisplayMsg, 0, sizeof(DisplayMsg) );
		strncpy( DisplayMsg, mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].DisplayMsg, \
			strlen(mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].DisplayMsg) );
		DisplayMsg[ strlen(mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].DisplayMsg) ] = 0;

		// 初始化图片
		g_GDC = GDCCreate( Width, Height );

		
		// 生成图片
		int count = AlpaCount( mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].DisplayMsg );
		DrawRectString( g_GDC, mapIp[ mapWinAddr[index] ]->hdcStyle[ (index-winOffset)*2 ].DisplayMsg, \
			FontName, FontSize, FontColor, \
			X + ( Width - (int)(count*FontSize + count*FontSize/3.0) ) / 2, \
			Y,		\
			Width,	\
			Height );

		
		// 插播
		status = NetSendPicOnceNew9G( 11024, (char *)(mapWinAddr[index].c_str()), g_GDC, 1, \
			mapIp[ mapWinAddr[index] ]->region[ (index-winOffset)*2 ].X,                  \
			mapIp[ mapWinAddr[index] ]->region[ (index-winOffset)*2 ].Y,                  \
			mapIp[ mapWinAddr[index] ]->region[ (index-winOffset)*2 ].Width,              \
			mapIp[ mapWinAddr[index] ]->region[ (index-winOffset)*2 ].Height,             \
			mapIp[ mapWinAddr[index] ]->region[ (index-winOffset)*2 ].PicWidth,           \
			mapIp[ mapWinAddr[index] ]->region[ (index-winOffset)*2 ].PicHeight,          \
			2, // PColorSign 显示屏颜色 1. 单色 2. 双色 
			2, // SendMode   发送方式   以太网通讯取 2
			0, // Showmode   显示方式   
			1, // ShowSpeed  显示速度
			0, // ShowTIme   显示停留时间
			60 // Pagetime   插播状态停留时间 
			);
		printf("%d--->%d\n",index,status);
		if (status == 0)
		{
			IMMTW45Slog->WriteLog(LL_DEBUG, "NetSendPicOnceNew9G sucess ! status = [%d]\n[%s|%d]", status, __FILE__, __LINE__);
		}
		else
		{
			IMMTW45Slog->WriteLog(LL_ERROR, "NetSendPicOnceNew9G sucess ! status = [%d]\n[%s|%d]", status, __FILE__, __LINE__);
		}
		
			
		// 释放图片
		GDCFree( g_GDC );
	}

	for ( index=1; index<=30; index++ )
	{
		winOffset     = mapIp[mapWinAddr[index]]->winOffset;

		int X         = mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].X;
		int Y         = mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].Y;
		int Width     = mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].Width;
		int Height    = mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].Height;
		int FontSize  = mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].FontSize;

		memset( FontName, 0, sizeof(FontName) );
		strncpy( FontName, mapIp[mapWinAddr[index]]->hdcStyle[1 + (index - winOffset)*2].FontName, \
			strlen(mapIp[mapWinAddr[index]]->hdcStyle[1 + (index - winOffset)*2].FontName) );
		FontName[strlen(mapIp[mapWinAddr[index]]->hdcStyle[1 + (index - winOffset)*2].FontName)] = 0;

		int FontColor = mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].FontColor;

		memset( DisplayMsg, 0, sizeof(DisplayMsg) );
		strncpy( DisplayMsg, mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].DisplayMsg, \
			strlen(mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].DisplayMsg) );
		DisplayMsg[ strlen(mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].DisplayMsg) ] = 0;

		// 初始化图片
		g_GDC = GDCCreate( Width, Height );

		// 生成图片
		int count = AlpaCount( mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].DisplayMsg );
		DrawRectString( g_GDC, mapIp[ mapWinAddr[index] ]->hdcStyle[ 1+(index-winOffset)*2 ].DisplayMsg, \
			FontName, FontSize, FontColor, \
			X + ( Width - (int)(count*FontSize + count*FontSize/3.0) ) / 2, \
			Y,		\
			Width,	\
			Height );

		
		// 插播
		status = NetSendPicOnceNew9G( 11024, (char *)(mapWinAddr[index].c_str()), g_GDC, 1, \
			mapIp[ mapWinAddr[index] ]->region[ 1+(index-winOffset)*2 ].X,                  \
			mapIp[ mapWinAddr[index] ]->region[ 1+(index-winOffset)*2 ].Y,                  \
			mapIp[ mapWinAddr[index] ]->region[ 1+(index-winOffset)*2 ].Width,              \
			mapIp[ mapWinAddr[index] ]->region[ 1+(index-winOffset)*2 ].Height,             \
			mapIp[ mapWinAddr[index] ]->region[ 1+(index-winOffset)*2 ].PicWidth,           \
			mapIp[ mapWinAddr[index] ]->region[ 1+(index-winOffset)*2 ].PicHeight,          \
			2, // PColorSign 显示屏颜色 1. 单色 2. 双色 
			2, // SendMode   发送方式   以太网通讯取 2
			0, // Showmode   显示方式   
			1, // ShowSpeed  显示速度
			0, // ShowTIme   显示停留时间
			60 // Pagetime   插播状态停留时间 
			);
		if (status == 0)
		{
			IMMTW45Slog->WriteLog(LL_DEBUG, "NetSendPicOnceNew9G sucess ! status = [%d]\n[%s|%d]", status, __FILE__, __LINE__);
		}
		else
		{
			IMMTW45Slog->WriteLog(LL_ERROR, "NetSendPicOnceNew9G sucess ! status = [%d]\n[%s|%d]", status, __FILE__, __LINE__);
		}
		
			
		// 释放图片
		GDCFree( g_GDC );
	}
	
	LeaveCriticalSection( &g_csA );
	
}

float AlpaCount( const char* srcStr )
{
	int len;
	float count;

	count = 0;
	len = strlen( srcStr );
	for(int i=0; i<len; i++)
	{
		if (srcStr[i] & 0X80)
		{
			i++;
			count++;
		}
		else if (!(srcStr[i]&0X80) && !(srcStr[i+1]&0X80))
		{
			i++;
			count++;
		}
		else if (!(srcStr[i]&0X80) && srcStr[i+1]&0X80)
		{
			count += 0.5;
		}
	}

	return count;
}

IMMTW45::~IMMTW45(void)
{
	// 关闭日志
	IMMTW45Slog->CloseLogFile();
}

// 加载窗口屏配置
int IMMTW45::loadStripScreenInfo()
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
int IMMTW45::loadIntegScreenInfo()
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
int  IMMTW45::InitiDev( )
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

	IMMTW45Slog->WriteLog(LL_DEBUG, "串口 :COM = [%s], 波特率    = [%s]\n[%s|%d]", comPort, comBaund, __FILE__, __LINE__);
	IMMTW45Slog->WriteLog(LL_DEBUG, "showColor = [%s], showMode  = [%s]\n[%s|%d]", showColor, showMode, __FILE__, __LINE__);
	IMMTW45Slog->WriteLog(LL_DEBUG, "showTime  = [%s], showSpeed = [%s]\n[%s|%d]", showTime, showSpeed, __FILE__, __LINE__);

	return 0;
}

// 关闭串口设备
int  IMMTW45::CloseDev( )
{
	return 0;
}

// 向条屏发送信息
int  IMMTW45::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	int status;

	//status = 0;
	//status = SendDatafun( m_ComPort, m_ComBaund, atoi(stripAddr), stripScreenInfo, m_ShowColor, m_ShowMode, m_ShowTime, m_ShowSpeed );
	int winAddr;
	int winOffset;
	char FontName[256];

	memset( FontName, 0, sizeof(FontName) );

	EnterCriticalSection( &g_csA );
	winAddr   = atoi(stripAddr);
	winOffset = mapIp[mapWinAddr[winAddr]]->winOffset;

	int X         = mapIp[ mapWinAddr[winAddr] ]->hdcStyle[ 1+(winAddr-winOffset)*2 ].X;
	int Y         = mapIp[ mapWinAddr[winAddr] ]->hdcStyle[ 1+(winAddr-winOffset)*2 ].Y;
	int Width     = mapIp[ mapWinAddr[winAddr] ]->hdcStyle[ 1+(winAddr-winOffset)*2 ].Width;
	int Height    = mapIp[ mapWinAddr[winAddr] ]->hdcStyle[ 1+(winAddr-winOffset)*2 ].Height;
	int FontSize  = mapIp[ mapWinAddr[winAddr] ]->hdcStyle[ 1+(winAddr-winOffset)*2 ].FontSize;
	strncpy( FontName, mapIp[mapWinAddr[winAddr]]->hdcStyle[1 + (winAddr - winOffset)*2].FontName, \
		strlen(mapIp[mapWinAddr[winAddr]]->hdcStyle[1 + (winAddr - winOffset)*2].FontName) );
	FontName[strlen(mapIp[mapWinAddr[winAddr]]->hdcStyle[1 + (winAddr - winOffset)*2].FontName)] = 0;
	int FontColor = mapIp[ mapWinAddr[winAddr] ]->hdcStyle[ 1+(winAddr-winOffset)*2 ].FontColor;
	
	// 初始化图片
	g_GDC = GDCCreate( Width, Height );


	//stripScreenInfo
	int count = AlpaCount( stripScreenInfo );
	DrawRectString( g_GDC, stripScreenInfo, FontName, FontSize, FontColor, \
		X + (Width - (int)(count*FontSize + count*FontSize/3.0)) / 2, \
		Y, \
		Width, \
		Height );

	// 插播
	//mapIp[ mapWinAddr[winAddr] ]->region[ 1+(winAddr-winOffset)*2 ].TimeOut = TIMEOUT;  // 重置刷新时间
	mapIp[ mapWinAddr[winAddr] ]->region[ 1+(winAddr-winOffset)*2 ].Flag    = 1;		// 有请求到来
	status = NetSendPicOnceNew9G( 11024, (char *)(mapWinAddr[winAddr].c_str()), g_GDC, 1, \
		mapIp[ mapWinAddr[winAddr] ]->region[ 1+(winAddr-winOffset)*2 ].X,                \
		mapIp[ mapWinAddr[winAddr] ]->region[ 1+(winAddr-winOffset)*2 ].Y,                \
		mapIp[ mapWinAddr[winAddr] ]->region[ 1+(winAddr-winOffset)*2 ].Width,            \
		mapIp[ mapWinAddr[winAddr] ]->region[ 1+(winAddr-winOffset)*2 ].Height,           \
		mapIp[ mapWinAddr[winAddr] ]->region[ 1+(winAddr-winOffset)*2 ].PicWidth,         \
		mapIp[ mapWinAddr[winAddr] ]->region[ 1+(winAddr-winOffset)*2 ].PicHeight,        \
		2, // PColorSign 显示屏颜色 1. 单色 2. 双色 
		2, // SendMode   发送方式   以太网通讯取 2
		0, // Showmode   显示方式   
		1, // ShowSpeed  显示速度
		0, // ShowTIme   显示停留时间
		60 // Pagetime   插播状态停留时间 
		);
	if (status == 0) {
		IMMTW45Slog->WriteLog(LL_INFO, "SendStripScreen = [%s] status = [%d]\n[%s|%d]", stripScreenInfo, status,  __FILE__, __LINE__);
	} else {
		IMMTW45Slog->WriteLog(LL_ERROR, "SendStripScreen = [%s] status = [%d]\n[%s|%d]", stripScreenInfo, status,  __FILE__, __LINE__);
	}

	// 释放图片
	GDCFree( g_GDC );

	LeaveCriticalSection( &g_csA );

	return 0;
}

// 向综合屏发送信息
int  IMMTW45::SendIntegScreen( char *recvData, int addr, int line, char *other )
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
	IMMTW45Slog->WriteLog(LL_DEBUG, "showFormat= [%s]\n[%s|%d]", showFormat,  __FILE__, __LINE__);
	*/
	// 分解综合屏显示格式
	SplitStrings::SplitStr( 0, showFormat, preStr, 'X' );
	SplitStrings::SplitStr( 1, showFormat, midStr, 'X' );
	SplitStrings::SplitStr( 2, showFormat, endStr, 'X' );
	sprintf( sendData, "%s%s%s%s%s", preStr, custerNum, midStr, windowNum, endStr );

	for (int i=0; i<m_integSndNums; i++) {
		IMMTW45Slog->WriteLog(LL_INFO, "integScreenSndInfo[%d].addr = [%d]\n[%s|%d]", i, integScreenSndInfo[i].addr,  __FILE__, __LINE__);
		status = SendDataZHLine(m_ComPort, m_ComBaund, integScreenSndInfo[i].addr, sendData, m_ShowColor);
		if (status == 0) {
			IMMTW45Slog->WriteLog(LL_INFO, "sendData = [%s] status = [%d]\n[%s|%d]", sendData,  status, __FILE__, __LINE__);
		} else {
			IMMTW45Slog->WriteLog(LL_ERROR, "sendData = [%s] status = [%d]\n[%s|%d]", sendData,  status, __FILE__, __LINE__);
		}
	}
	
	
	return 0;
}

// 窗口屏初始化显示
int  IMMTW45::InitStripScreen( )
{	
	IMMTW45Slog->WriteLog(LL_DEBUG, "IMMTW45::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
//		SendDataOnly( m_ComPort, m_ComBaund, stripScreenDefInfo[i].addr, 3, stripScreenDefInfo[i].defMessage, 0x11 );
	}
	
	return 0;
}

// 综合屏初始化显示
int  IMMTW45::InitIntegScreen( )
{	
	IMMTW45Slog->WriteLog(LL_DEBUG, "IMMTW45::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
//		SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
	}
	
	return 0;
}

int  IMMTW45::LoadLibrarys( )
{
	HINSTANCE hDll = NULL;
	hDll = LoadLibrary( "screen\\IMMTW45\\dlltpzp.dll" );
	if (hDll == NULL)
		return -1;

	SendDataZHLine = (pFunSendDataZHLine) GetProcAddress( hDll, "SendDataZHLine" );
	if (SendDataZHLine == NULL)
		return -1;

	hDll = LoadLibrary( "screen\\IMMTW45\\EV_SocketUdpDll.dll" );
	if (hDll == NULL)
		return -1;

	DrawRectString      = (pFunDrawRectString) GetProcAddress( hDll, "DrawRectString" );
	GDCCreate           = (pFunGDCCreate) GetProcAddress( hDll, "GDCCreate" );
	GDCFree             = (pFunGDCFree) GetProcAddress( hDll, "GDCFree" );
	NetSendPicOnceNew9G = (pFunNetSendPicOnceNew9G) GetProcAddress( hDll, "NetSendPicOnceNew9G" );
	if (DrawRectString == NULL || GDCCreate == NULL || GDCFree == NULL || NetSendPicOnceNew9G == NULL)
		return -1;

	return 0;
}
