#include "IMM9005.h"

Implement_DynCreate(IMM9005, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM9005.ini")->GetParamValue(key, valueBuffer)

IMM9005::IMM9005(void)
{
	IMM9005Slog = new Log("log\\IMM9005.log", LL_DEBUG);
	IMM9005Slog->WriteLog(LL_DEBUG, "IMM9005::IMM9005(void)");
	// 加载动态库 LcLedComm.dll
	int status = LoadLibrarys( "screen\\IMM9005\\LcLedComm.dll" );
	if (status != 0)
		IMM9005Slog->WriteLog(LL_ERROR, "LoadLibrarys LcLedComm.dll Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM9005Slog->WriteLog(LL_INFO, "LoadLibrarys LcLedComm.dll Sucess!\n[%s|%d]", __FILE__, __LINE__);

	// 加载综合屏配置
	status = LoadIntegScreen();
	if (status != 0)
		IMM9005Slog->WriteLog(LL_ERROR, "加载综合屏出错 Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM9005Slog->WriteLog(LL_INFO, "加载综合屏成功 Sucess!\n[%s|%d]", __FILE__, __LINE__);

	// 初始化窗口屏
	//loadStripScreenInfo();

	// 初始化综合屏
	//loadIntegScreenInfo();

	// 串口操作句柄
	g_handle = NULL;
}

IMM9005::~IMM9005(void)
{
	IMM9005Slog->CloseLogFile();
}

int IMM9005::loadStripScreenInfo()
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
	m_stripNums = atoi(temp);
	for (int i=0; i<m_stripNums; i++) {
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

int IMM9005::loadIntegScreenInfo()
{
	char temp[128];
	char integScreen[128];
	char strFormat[64];
	char addr[16];
	char context[32];

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("INITINTEGSCREEN", temp);
	m_initInteg = atoi(temp)==1 ? true : false;
	if (m_initInteg == false)
		return 0;

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("INTEGNUMBERS", temp);
	m_integNums = atoi(temp);
	for (int i=0; i<m_integNums; i++) {
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
int  IMM9005::InitiDev( )
{
	char comPort[8];
	char comBaund[16];

	memset( comPort , 0, sizeof(comPort)  );
	memset( comBaund, 0, sizeof(comBaund) );

	SCParamValueINI( "COM", comPort );
	SCParamValueINI( "SENDBAUND", comBaund );

	g_handle = OpenPort( comPort, atol(comBaund) );
	if ( !g_handle )
		return -1;
	IMM9005Slog->WriteLog(LL_DEBUG, "打开串口设备:COM=[%s], 波特率 = [%s]\n[%s|%d]", comPort, comBaund, __FILE__, __LINE__);

	return 0;
}

// 关闭串口设备
int  IMM9005::CloseDev( )
{
	assert( g_handle != NULL );
	ClosePort( g_handle );
	return 0;
}

// 向条屏发送信息
int  IMM9005::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	//printf("addr = [%s],stripScreenInfo = [%s]\n", stripAddr, stripScreenInfo);
	DataSend( g_handle, stripAddr, 0, 1, 1, 1, stripScreenInfo, 1 );

	return 0;
}

// 向综合屏发送信息
int  IMM9005::SendIntegScreen( char *recvData, int addr, int line, char *other )
{	
	int  i;
	char sendAddr[8];
	char customeNum[8];
	char windowsNum[8];
	char sendBuffer[64];
	char preStr[16];
	char midStr[16];
	char endStr[16];

	i = 0;
	memset( sendAddr  , 0, sizeof(sendAddr)   );
	memset( customeNum, 0, sizeof(customeNum) );
	memset( windowsNum, 0, sizeof(windowsNum) );
	memset( sendBuffer, 0, sizeof(sendBuffer) );
	memset( preStr, 0, sizeof(preStr) );
	memset( midStr, 0, sizeof(midStr) );
	memset( endStr, 0, sizeof(endStr) );

	
	SplitStrings::SplitStr( 0, recvData, customeNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowsNum, ',' );

	// 分解综合屏显示格式
	SplitStrings::SplitStr( 0, showFormat, preStr, 'X' );
	SplitStrings::SplitStr( 1, showFormat, midStr, 'X' );
	SplitStrings::SplitStr( 2, showFormat, endStr, 'X' );
	sprintf( sendBuffer, "%s%s%s%s%s", preStr, customeNum, midStr, windowsNum, endStr );

	for (i=0; i<m_sndintegnumbers; i++)
	{
		//printf("sendBuffer = [%s]\n", sendBuffer);
		queueArray[i].EnQueue( sendBuffer );
		for (int j=0; j<queueArray[i].GetRows(); j++)
		{
			DataSend( g_handle, queueArray[i].GetAddrAsIndex(j), 0, 1, 1, 1, queueArray[i].VisQueue(j), 1 );
			//printf("addr = [%s], info = [%s]\n", queueArray[i].GetAddrAsIndex(j), queueArray[i].VisQueue(j));
		}
	}

	return 0;
}

// 窗口屏初始化显示
int  IMM9005::InitStripScreen( )
{	
	IMM9005Slog->WriteLog(LL_DEBUG, "IMM9005::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripNums; i++)
		//printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
		;
	
	return 0;
}

// 综合屏初始化显示
int  IMM9005::InitIntegScreen( )
{	
	IMM9005Slog->WriteLog(LL_DEBUG, "IMM9005::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integNums; i++)
		//printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
		;
	
	return 0;
}

int  IMM9005::LoadLibrarys( char *pathname )
{
	HINSTANCE hDll = NULL;
	hDll = LoadLibrary( pathname );
	if (hDll == NULL)
		return -1;

	OpenPort  = (LpOpenPort) GetProcAddress(hDll, "LedOpenComm");
	if (OpenPort  == NULL)
		return -1;

	ClosePort = (LpClosePort)GetProcAddress(hDll, "LedCloseComm");
	if (ClosePort == NULL)
		return -1;

	DataSend  = (LpDataSend)GetProcAddress(hDll, "LedSendData");
	if (DataSend == NULL)
		return -1;

	return 0;
}

int IMM9005::LoadIntegScreen( )
{
	char temp[128];
	char sndIntegSubs[512];
	char addrSubFormat[128];
	char subTemp[128];
		 
	memset( temp         , 0, sizeof(temp)         );
	memset( subTemp      , 0, sizeof(subTemp)      );
	memset( sndIntegSubs , 0, sizeof(sndIntegSubs) );
	memset( addrSubFormat, 0, sizeof(addrSubFormat));
	SCParamValueINI( "SNDINTEGNUMBERS", temp         );
	SCParamValueINI( "SNDINTEGSUB"    , sndIntegSubs );

	// 构造综合屏缓冲区对象数组
	m_sndintegnumbers = atoi(temp);
	//printf("综合屏条数：%d\n",m_sndintegnumbers);
	queueArray = new QueueArray[ m_sndintegnumbers ];
	//printf("m_sndintegnumbers = [%d]\n", m_sndintegnumbers);

	// 循环构造每个对象
	for (int i=0; i<m_sndintegnumbers; i++)
	{
		memset(temp, 0, sizeof(temp));
		memset(subTemp, 0, sizeof(subTemp));
		memset(addrSubFormat, 0, sizeof(addrSubFormat));

		//SNDINTEGSUB1
		SplitStrings::SplitStr( i, sndIntegSubs, temp, '|' );
		sprintf(addrSubFormat, "%sROWS", temp);
		//printf("addrSubFormat = [%s]\n", addrSubFormat);

		SCParamValueINI(addrSubFormat, subTemp);
		//printf("subTemp = [%s][%d]\n", subTemp, atoi(subTemp));
		//queueArray[i].m_row = atoi(subTemp);
		queueArray[i].SetRows( atoi(subTemp) );
		//printf("queueArray[%d].GetRows() = [%d]\n", i, queueArray[i].GetRows());

		memset( addrSubFormat, 0, sizeof(addrSubFormat));
		sprintf(addrSubFormat, "%sCOLS", temp);
		SCParamValueINI(addrSubFormat, subTemp);
		//queueArray[i].m_col = atoi(subTemp);
		queueArray[i].SetCols( atoi(subTemp) );
		//printf("queueArray[%d].GetCols() = [%d]\n", i, queueArray[i].GetCols());

		queueArray[i].ConstrObject( );
		queueArray[i].InitQueue( );

		for (int j=0; j<queueArray[i].GetRows(); j++)
		{
			memset(addrSubFormat, 0, sizeof(addrSubFormat));
			memset(subTemp, 0, sizeof(subTemp));

			sprintf(addrSubFormat, "%s[%d]", temp, j+1);
			SCParamValueINI(addrSubFormat, subTemp);

			queueArray[i].SetAddrAsIndex( j, subTemp, strlen(subTemp) );
			//printf("===[%s]===\n", queueArray[i].GetAddrAsIndex(j));
		}
	}

	memset(showFormat, 0, sizeof(showFormat));
	SCParamValueINI("SHOWFORMAT", showFormat);
	IMM9005Slog->WriteLog(LL_DEBUG, "SHOWFORMAT=[%s]\n[%s|%d]", showFormat, __FILE__, __LINE__);

	return 0;
}

/*
inline QueueArray::QueueArray( int row, int col )
{
	int i;
	m_row = row;
	m_col = col;

	m_array = new char*[m_row];
	for (i = 0; i<m_row; i++)
	{
		m_array[i] = new char[m_col];
		memset( m_array[i], 0, m_col );
	}

	m_addrs = new char*[m_row];
	for (i = 0; i<m_row; i++)
	{
		m_addrs[i] = new char[m_col];
		memset( m_addrs[i], 0, m_col );
	}
}

inline QueueArray::~QueueArray( )
{
	if (m_row != 0 && m_col != 0)
	{
		for (int i=0; i<m_row; i++)
		{
			delete [] m_array[i];
		}

		if (m_array)
		{
			delete [] m_array;
		}
	}

	m_row = m_col = 0;
}

inline void QueueArray::InitQueue( )
{
	for (int i = 0; i < m_row; i++)
	{
		memset( m_array[i], 0, m_col );
	}
}

inline int QueueArray::EnQueue( const char* data )
{
	int i;
	for (i = 0; i<m_row-1; i++)
	{
		memset( m_array[i], 0, m_col );
		memcpy( m_array[i], m_array[i+1], strlen(m_array[i+1]) );
	}

	memset( m_array[i], 0, m_col );
	memcpy( m_array[i], data, strlen(data) );

	return 0;
}

inline char * QueueArray::VisQueue( int index )
{
	return m_array[index];
}

inline void QueueArray::SetRows( int rows )
{
	m_row = rows;
}

inline void QueueArray::SetCols( int cols )
{
	m_col = cols;
}

inline void QueueArray::ConstrObject( )
{
	int i;

	m_array = new char*[m_row];
	for (i = 0; i<m_row; i++)
	{
		m_array[i] = new char[m_col];
		memset( m_array[i], 0, m_col );
	}

	m_addrs = new char*[m_row];
	for (i = 0; i<m_row; i++)
	{
		m_addrs[i] = new char[m_col];
		memset( m_addrs[i], 0, m_col );
	}
}

inline void   QueueArray::SetAddrAsIndex(int index, const char *src, int len)
{
	if (len > m_col)
		return ;
	memset(m_addrs[index], 0, m_col);
	strncpy(m_addrs[index], src, len);
}

inline char * QueueArray::GetAddrAsIndex( int index )
{
	return m_addrs[index];
}
*/