#include "IMM9002.h"

Implement_DynCreate(IMM9002, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM9002.ini")->GetParamValue(key, valueBuffer)

// 初始化工作
IMM9002::IMM9002(void)
{
	char temp[16];

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("LOGLEVEL", temp);

	IMM9002Slog = new Log("log\\IMM9002.log", temp);

	loadStripScreenInfo();

	loadIntegScreenInfo();
}

IMM9002::~IMM9002(void)
{
	IMM9002Slog->CloseLogFile();
}

int IMM9002::loadStripScreenInfo( )
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

int IMM9002::loadIntegScreenInfo( )
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
int  IMM9002::InitiDev( )
{
	char ComPortNum[8];
	memset(ComPortNum, 0, sizeof(ComPortNum));

	SCParamValueINI("COM", ComPortNum);
	Com.InitPort( atoi(ComPortNum) );
	return 0;
}

// 关闭串口设备
int  IMM9002::CloseDev( )
{
	Com.ClosePort();
	return 0;
}

// 向条屏发送信息
int  IMM9002::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
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

	bytes = 0;
	memset(sendBuffer, 0, sizeof(sendBuffer));

	bytes = PacketMessage(stripScreenInfo, atoi(stripAddr), (unsigned char *)sendBuffer);

	Com.WriteToPort(sendBuffer, bytes);

	return 0;
}

// 向综合屏发送信息
int  IMM9002::SendIntegScreen( char *recvData, int addr, int line, char *other )
{
/*
	printf("SendIntegScreen\n");
	printf("recvData = [%s]\n", recvData);
	printf("addr =  [%d]\n", addr);
	printf("line =  [%d]\n", line);
	printf("other = [%s]\n", other);
*/
	int bytes;
	char custerNum[8];
	char windowNum[8];
	char sendData[512];
	char sendBuffer[1024];

	bytes = 0;
	memset(custerNum , 0, sizeof(custerNum) );
	memset(windowNum , 0, sizeof(windowNum) );
	memset(sendData  , 0, sizeof(sendData)  );
	memset(sendBuffer, 0, sizeof(sendBuffer));

	SplitStrings::SplitStr( 0, recvData, custerNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowNum, ',' );
	sprintf( sendData, "请%s到%s窗口", custerNum, windowNum );

	for (int i=0; i<m_integSndNums; i++) {
		bytes = PacketMessage(sendData, integScreenSndInfo[i].addr, (unsigned char *)sendBuffer);
		Com.WriteToPort(sendBuffer, bytes);	
	}

	return 0;
}

// 条屏初始化显示
int  IMM9002::InitStripScreen( )
{
	IMM9002Slog->WriteLog(LL_DEBUG, "IMM9002::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		//printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, stripScreenDefInfo[i].addr, 3, stripScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM9002::InitStripScreen( )\n");

	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

// 综合屏初始化显示
int  IMM9002::InitIntegScreen( )
{
	IMM9002Slog->WriteLog(LL_DEBUG, "IMM9002::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		//printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM9002::InitIntegScreen( )\n");
	
	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

int  IMM9002::PacketMessage(const char *recvBuffer, int addr, unsigned char *sendBuffer)
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
