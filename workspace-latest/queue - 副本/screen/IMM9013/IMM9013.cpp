#include "IMM9013.h"

Implement_DynCreate(IMM9013, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM9013.ini")->GetParamValue(key, valueBuffer)

// 初始化工作
IMM9013::IMM9013(void)
{
	char temp[16];

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("LOGLEVEL", temp);

	IMM9013Slog = new Log("log\\IMM9013.log", temp);

	loadStripScreenInfo();

	loadIntegScreenInfo();
}

IMM9013::~IMM9013(void)
{
	IMM9013Slog->CloseLogFile();
}

int IMM9013::loadStripScreenInfo( )
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

int IMM9013::loadIntegScreenInfo( )
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
int  IMM9013::InitiDev( )
{
	char ComPortNum[8];
	memset(ComPortNum, 0, sizeof(ComPortNum));

	SCParamValueINI("COM", ComPortNum);
	Com.InitPort( atoi(ComPortNum) );
	return 0;
}

// 关闭串口设备
int  IMM9013::CloseDev( )
{
	Com.ClosePort();
	return 0;
}

// 向条屏发送信息
int  IMM9013::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
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
int  IMM9013::SendIntegScreen( char *recvData, int addr, int line, char *other )
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
	sprintf( sendData, "请%s号到%s窗口", custerNum, windowNum );

	for (int i=0; i<m_integSndNums; i++) {
		bytes = PacketMessageZH(sendData, integScreenSndInfo[i].addr, (unsigned char *)sendBuffer);
		Com.WriteToPort(sendBuffer, bytes);	
	}

	return 0;
}

// 条屏初始化显示
int  IMM9013::InitStripScreen( )
{
	IMM9013Slog->WriteLog(LL_DEBUG, "IMM9013::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		//printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, stripScreenDefInfo[i].addr, 3, stripScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM9013::InitStripScreen( )\n");

	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

// 综合屏初始化显示
int  IMM9013::InitIntegScreen( )
{
	IMM9013Slog->WriteLog(LL_DEBUG, "IMM9013::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		//printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM9013::InitIntegScreen( )\n");
	
	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

int  IMM9013::PacketMessage(const char *recvBuffer, int addr, unsigned char *sendBuffer)
{
	int bytes=0;
	int len;
	char aftData[1024];
	static int count = 0x00;
	if( count>255 )
		count = 0;
	else 
		count++;
	memset(aftData , 0, sizeof(aftData) );

	len = strlen(recvBuffer);
	sendBuffer[0]  = addr;
	sendBuffer[1]  = 0xfe; //ch_back[1]=addr;
	sendBuffer[2]  = len+12; //ch_back[2]=0xbb;
	sendBuffer[3]  = count; //ch_back[3]=0x51;
	sendBuffer[4]  = 0x86;
	sendBuffer[5]  = 0x3c;
	sendBuffer[6]  = 0x00;
	sendBuffer[7]  = 0x05; 
	sendBuffer[8]  = 0x00; 
	

	memcpy( sendBuffer+9, recvBuffer, len );

	bytes = 9 + len;
	sendBuffer[bytes++] = 0;
	int k = CracksAlgorithm::CRC16_CCITT(sendBuffer, bytes);
	sendBuffer[bytes++] = k/256;
	sendBuffer[bytes++] = k%256;
	
	sendBuffer[2]  = bytes;
	for(int i = 0;i<bytes; i++)
		printf("%2x ",sendBuffer[i]);
	printf("\n");
	return bytes;
}
int  IMM9013::PacketMessageZH(const char *recvBuffer, int addr, unsigned char *sendBuffer)
{
	int bytes=0;
	int len;
	char aftData[1024];
	static int countZH = 0x00;
	if( countZH>255 )
		countZH = 0;
	else 
		countZH++;
	memset(aftData , 0, sizeof(aftData) );
	
	len = strlen(recvBuffer);
	sendBuffer[0]  = addr;
	sendBuffer[1]  = 0xfe; //ch_back[1]=addr;
	sendBuffer[2]  = len+9; //ch_back[2]=0xbb;
	sendBuffer[3]  = countZH; //ch_back[3]=0x51;
	sendBuffer[4]  = 0x8c;
	sendBuffer[5]  = 0x00;
	
	
	memcpy( sendBuffer+6, recvBuffer, len );
	
	bytes = 6 + len;
	sendBuffer[bytes++] = 0;
	int k = CracksAlgorithm::CRC16_CCITT(sendBuffer, bytes);
	sendBuffer[bytes++] = k/256;
	sendBuffer[bytes++] = k%256;
	
	sendBuffer[2]  = bytes;
	for(int i = 0;i<bytes; i++)
		printf("%2x ",sendBuffer[i]);
	printf("\n");
	return bytes;
}

int  IMM9013::dataTrans(const char *preData, char *aftData)
{	
	int i = 0;
	int k = 0;
	int len = strlen(preData);
	for( i=0; i<len; i++)
	{
		if(preData[i]&0X80)//判断为汉字
		{
			aftData[k] = preData[i];
			aftData[++k] = preData[++i];
			aftData[++k] = 0x00;
		}
		else
		{
			aftData[k] = preData[i];
			aftData[++k] = 0x00;
		}
		k++;
	}


	return k;

}