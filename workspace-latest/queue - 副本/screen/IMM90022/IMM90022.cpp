#include "IMM90022.h"

Implement_DynCreate(IMM90022, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM90022.ini")->GetParamValue(key, valueBuffer)

// 初始化工作
IMM90022::IMM90022(void)
{
	char temp[16];

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("LOGLEVEL", temp);

	IMM90022Slog = new Log("log\\IMM90022.log", temp);

	// 加载动态库 KeyPad.dll
	int status = LoadLibrarys( "screen\\IMM90022\\KeyPad.dll" );
	if (status != 0)
		IMM90022Slog->WriteLog(LL_ERROR, "LoadLibrarys KeyPad.dll Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM90022Slog->WriteLog(LL_INFO, "LoadLibrarys KeyPad.dll Sucess!\n[%s|%d]", __FILE__, __LINE__);

	loadStripScreenInfo();

	loadIntegScreenInfo();

	// 加载窗口屏拼接成综合屏的配置
	status = LoadIntegScreen();
	if (status != 0)
		IMM90022Slog->WriteLog(LL_ERROR, "加载拼接综合屏出错 Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMM90022Slog->WriteLog(LL_INFO, "加载拼接综合屏成功 Sucess!\n[%s|%d]", __FILE__, __LINE__);
}

IMM90022::~IMM90022(void)
{
	IMM90022Slog->CloseLogFile();
}

int IMM90022::loadStripScreenInfo( )
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

int IMM90022::loadIntegScreenInfo( )
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
int  IMM90022::InitiDev( )
{
	char ComPortNum[8];
	memset(ComPortNum, 0, sizeof(ComPortNum));

	SCParamValueINI("COM", ComPortNum);
	Com.InitPort( atoi(ComPortNum) );
	return 0;
}

// 关闭串口设备
int  IMM90022::CloseDev( )
{
	Com.ClosePort();
	return 0;
}

// 向条屏发送信息
int  IMM90022::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	/*
	printf( "SendStripScreen\n" );
	printf( "stripAddr = [%s]\n", stripAddr );
	printf( "stripScreenInfo = [%s]\n", stripScreenInfo );
	printf( "stripShowMode = [%s]\n", stripShowMode );
	printf( "IntegScreenInfo = [%s]\n", IntegScreenInfo );
	*/
	int bytes;
	char sendBuffer[1024];
	char exChangFormat[1024];

	bytes = 0;
	memset(sendBuffer, 0, sizeof(sendBuffer));
	memset(exChangFormat, 0, sizeof(exChangFormat));

	IMM90022Slog->WriteLog(LL_DEBUG, "SendStripScreen before = [%s]\n[%s|%d]", stripScreenInfo, __FILE__, __LINE__);
	if (memcmp(stripScreenInfo, "  欢迎光临", strlen("  欢迎光临")) == 0) {
		sprintf(exChangFormat, "%s热情服务", stripScreenInfo+2);
	} else if (memcmp(stripScreenInfo, "  暂停服务", strlen("  暂停服务")) == 0 ) {
		sprintf(exChangFormat, "  %s", stripScreenInfo);
	} else if (memcmp(stripScreenInfo, "  取消暂停", strlen("  取消暂停")) == 0) {
		//sprintf(exChangFormat, " ", stripScreenInfo);
		strncpy(exChangFormat, "欢迎光临热情服务", strlen("欢迎光临热情服务"));
	} else if (memcmp(stripScreenInfo, "  请", strlen("  请")) == 0) {
		sprintf(exChangFormat, "%s", stripScreenInfo+1);
	} else if (stripScreenInfo[2]>='A' && stripScreenInfo[2]<='Z') {
		sprintf(exChangFormat, "%s", stripScreenInfo);
	} else {
		printf("未知格式\n");
	}
	IMM90022Slog->WriteLog(LL_DEBUG, "SendStripScreen after  = [%s]\n[%s|%d]", exChangFormat, __FILE__, __LINE__);
	//bytes = PacketMessage(stripScreenInfo, atoi(stripAddr), (unsigned char *)sendBuffer);
	bytes = PacketMessage(exChangFormat, atoi(stripAddr), (unsigned char *)sendBuffer);

	Com.WriteToPort(sendBuffer, bytes);
	Sleep(100);

	return 0;
}

// 向综合屏发送信息
int  IMM90022::SendIntegScreen( char *recvData, int addr, int line, char *other )
{
/*
	printf("SendIntegScreen\n");
	printf("recvData = [%s]\n", recvData);
	printf("addr =  [%d]\n", addr);
	printf("line =  [%d]\n", line);
	printf("other = [%s]\n", other);
*/
	int  i;
	int  bytes;
	char custerNum[8];
	char windowNum[8];
	char sendData[512];
	char sendBuffer[1024];
	char preStr[16];
	char midStr[16];
	char endStr[16];

	bytes = 0;
	memset(custerNum , 0, sizeof(custerNum) );
	memset(windowNum , 0, sizeof(windowNum) );
	memset(sendData  , 0, sizeof(sendData)  );
	memset(sendBuffer, 0, sizeof(sendBuffer));
	memset( preStr, 0, sizeof(preStr) );
	memset( midStr, 0, sizeof(midStr) );
	memset( endStr, 0, sizeof(endStr) );

	SplitStrings::SplitStr( 0, recvData, custerNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowNum, ',' );
	//sprintf( sendData, "请%s到%s窗口", custerNum, windowNum );
	SplitStrings::SplitStr( 0, showFormat, preStr, 'X' );
	SplitStrings::SplitStr( 1, showFormat, midStr, 'X' );
	SplitStrings::SplitStr( 2, showFormat, endStr, 'X' );
	sprintf( sendData, "%s%s%s%s%s", preStr, custerNum, midStr, windowNum, endStr );

	IMM90022Slog->WriteLog(LL_DEBUG, "综合屏数据 = [%s]\n[%s|%d]", sendData, __FILE__, __LINE__);
	// 综合屏发送操作
	for (i=0; i<m_integSndNums; i++) {
		memset(sendBuffer, 0, sizeof(sendBuffer));
		IMM90022Slog->WriteLog(LL_DEBUG, "综合屏 addr = [%d], content = [%s]\n[%s|%d]", \
				integScreenSndInfo[i].addr, sendData, __FILE__, __LINE__);
		bytes = PacketMessageToIntegScreen(sendData, integScreenSndInfo[i].addr, (unsigned char *)sendBuffer);
		Com.WriteToPort(sendBuffer, bytes);	
	}

	// 拼接综合屏发送操作
	for (i=0; i<m_integSubNums; i++) {
		queueArray[i].EnQueue( sendData );
		for (int j=0; j<queueArray[i].GetRows(); j++) {
			IMM90022Slog->WriteLog(LL_DEBUG, "拼接综合屏 addr = [%s], content = [%s]\n[%s|%d]", \
				queueArray[i].GetAddrAsIndex(j), queueArray[i].VisQueue(j), __FILE__, __LINE__);
			memset(sendBuffer, 0, sizeof(sendBuffer));
			bytes = PacketMessage( queueArray[i].VisQueue(j), atoi(queueArray[i].GetAddrAsIndex(j)), (unsigned char *)sendBuffer );	
			Com.WriteToPort( sendBuffer, bytes );
			Sleep(100);
		}
	}

	return 0;
}

// 条屏初始化显示
int  IMM90022::InitStripScreen( )
{
	IMM90022Slog->WriteLog(LL_DEBUG, "IMM90022::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		//printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, stripScreenDefInfo[i].addr, 3, stripScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM90022::InitStripScreen( )\n");

	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

// 综合屏初始化显示
int  IMM90022::InitIntegScreen( )
{
	IMM90022Slog->WriteLog(LL_DEBUG, "IMM90022::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		//printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM90022::InitIntegScreen( )\n");
	
	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

int  IMM90022::PacketMessage(const char *recvBuffer, int addr, unsigned char *sendBuffer)
{
	int bytes=0;
 
	sendBuffer[0]  = 0XAA;
	sendBuffer[1]  = addr; //ch_back[1]=addr;
	sendBuffer[2]  = 0XBB; //ch_back[2]=0xbb;
	sendBuffer[3]  = 0X51; //ch_back[3]=0x51;
	sendBuffer[4]  = 0X54; //ch_back[4]=0x54;
	sendBuffer[5]  = 0X00; //校验位
	sendBuffer[6]  = 0X01; //ch_back[6]=0x01;
	sendBuffer[7]  = 0X00; //ch_back[7]=0x00;
	sendBuffer[8]  = 0X99; //ch_back[8]=0x99;
	sendBuffer[9]  = 0X00; //ch_back[9]=0x00;
	sendBuffer[10] = 0X63; //ch_back[10]=0x63;

	memcpy( sendBuffer+11, recvBuffer, strlen(recvBuffer) );

	bytes = 11 + strlen(recvBuffer);
	sendBuffer[bytes++] = 0XFF;

	sendBuffer[5] = CracksAlgorithm::str_crc(sendBuffer, 6, bytes, 256);

	return bytes;
}

//ch_back[6]=0x08;
//ch_back[7]=0x02;
int  IMM90022::PacketMessageToIntegScreen(const char *recvBuffer, int addr, unsigned char *sendBuffer)
{
	int bytes=0;
 
	sendBuffer[0]  = 0XAA;
	sendBuffer[1]  = addr; //ch_back[1]=addr;
	sendBuffer[2]  = 0XBB; //ch_back[2]=0xbb;
	sendBuffer[3]  = 0X51; //ch_back[3]=0x51;
	sendBuffer[4]  = 0X54; //ch_back[4]=0x54;
	sendBuffer[5]  = 0X00; //校验位
	sendBuffer[6]  = 0X08; //ch_back[6]=0x01;
	sendBuffer[7]  = 0X02; //ch_back[7]=0x00;
	sendBuffer[8]  = 0X99; //ch_back[8]=0x99;
	sendBuffer[9]  = 0X00; //ch_back[9]=0x00;
	sendBuffer[10] = 0X63; //ch_back[10]=0x63;

	memcpy( sendBuffer+11, recvBuffer, strlen(recvBuffer) );

	bytes = 11 + strlen(recvBuffer);
	sendBuffer[bytes++] = 0XFF;

	sendBuffer[5] = CracksAlgorithm::str_crc(sendBuffer, 6, bytes, 256);

	return bytes;
}

int  IMM90022::LoadLibrarys( char *pathname )
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

int  IMM90022::LoadIntegScreen()
{
	char temp[128];
	char sndIntegSubs[512];
	char addrSubFormat[128];
	char subTemp[128];
		 
	memset( temp         , 0, sizeof(temp)         );
	memset( subTemp      , 0, sizeof(subTemp)      );
	memset( sndIntegSubs , 0, sizeof(sndIntegSubs) );
	memset( addrSubFormat, 0, sizeof(addrSubFormat));
	SCParamValueINI( "SNDINTGSUBNUM", temp         );
	SCParamValueINI( "SNDINTEGSUB"  , sndIntegSubs );

	// 构造综合屏缓冲区对象数组
	m_integSubNums = atoi(temp);
	queueArray = new QueueArray[ m_integSubNums ];
	//printf("m_integSubNums = [%d]\n", m_integSubNums);

	// 循环构造每个对象
	for (int i=0; i<m_integSubNums; i++)
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
	IMM90022Slog->WriteLog(LL_DEBUG, "SHOWFORMAT=[%s]\n[%s|%d]", showFormat, __FILE__, __LINE__);

	return 0;
}
