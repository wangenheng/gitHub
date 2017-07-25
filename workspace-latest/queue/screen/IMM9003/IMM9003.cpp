#include "IMM9003.h"

Implement_DynCreate(IMM9003, ScreenAdapter)

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM9003.ini")->GetParamValue(key, valueBuffer)

// 初始化工作
IMM9003::IMM9003(void)
{
	char loglevel[16];
	char IntegShowMode[16];

	memset(loglevel     , 0, sizeof(loglevel)     );
	memset(IntegShowMode, 0, sizeof(IntegShowMode));
	SCParamValueINI("LOGLEVEL", loglevel);
	SCParamValueINI("MULTINTEGSHOWMODE", IntegShowMode);
	m_IntegShowMode = atoi(IntegShowMode);

	IMM9003Slog = new Log("log\\IMM9003.log", loglevel);

	loadStripScreenInfo();

	loadIntegScreenInfo();
}

IMM9003::~IMM9003(void)
{
	IMM9003Slog->CloseLogFile();
}

int IMM9003::loadStripScreenInfo( )
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

int IMM9003::loadIntegScreenInfo( )
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
		memset( temp     , 0, sizeof(temp)      );
		memset( addr     , 0, sizeof(addr)      );
		memset( strFormat, 0, sizeof(strFormat) );

		sprintf(strFormat, "SNDINTEGSCREEN%d", i+1);
		SCParamValueINI(strFormat, addr);

		SplitStrings::SplitStr( 0, addr, temp, '|' );
		integScreenSndInfo[i].addr = atoi(temp);
		//printf("integScreenSndInfo[%d].addr = [%d]\n", i+1, integScreenSndInfo[i].addr);
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
int  IMM9003::InitiDev( )
{
	char SeriPortNum[8];

	memset(SeriPortNum, 0, sizeof(SeriPortNum));
	SCParamValueINI("COM", SeriPortNum);

	Com.InitPort( atoi(SeriPortNum) );

	return 0;
}

// 关闭串口设备
int  IMM9003::CloseDev( )
{
	Com.ClosePort();
	return 0;
}

// 向条屏发送信息
int  IMM9003::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	int bytes;
	char sendBuffer[1024];

	bytes = 0;
	memset(sendBuffer, 0, sizeof(sendBuffer));

	bytes = PacketMessage(stripScreenInfo, atoi(stripAddr), (unsigned char *)sendBuffer);

	Com.WriteToPort(sendBuffer, bytes);

	return 0;
}

// 向综合屏发送信息
int  IMM9003::SendIntegScreen( char *recvData, int addr, int line, char *other )
{
	int  bytes;
	char custerNum[8];
	char windowNum[8];
	char sendData[512];
	char sendBuffer[1024];
	int  i;
	char multiAddr[512];
	char strFormat[512];
	char IntegScreenAddr[16];
	char subWindowNum[8];

	bytes = 0;
	memset( custerNum , 0, sizeof(custerNum)  );
	memset( windowNum , 0, sizeof(windowNum)  );
	memset( sendData  , 0, sizeof(sendData)   );
	memset( sendBuffer, 0, sizeof(sendBuffer) );
	memset( IntegScreenAddr, 0, sizeof(IntegScreenAddr));

	SplitStrings::SplitStr( 0, recvData, custerNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowNum, ',' );
	sprintf( sendData, "请%s号到%02d窗口", custerNum, atoi(windowNum) );

	if (m_IntegShowMode == 1) { // 同步
		for (i=0; i<m_integSndNums; i++) {
			bytes = PacketMessage(sendData, integScreenSndInfo[i].addr, (unsigned char *)sendBuffer);
			Com.WriteToPort(sendBuffer, bytes);
		}
	} else if (m_IntegShowMode == 2) { // 独立
		for (i=0; i<m_integSndNums; i++) {
			
			memset( multiAddr      , 0, sizeof(multiAddr)       );
			memset( strFormat      , 0, sizeof(strFormat)       );
			memset( IntegScreenAddr, 0, sizeof(IntegScreenAddr) );

			sprintf(strFormat, "SNDINTEGSCREEN%d", i+1);
			SCParamValueINI(strFormat, multiAddr);

			SplitStrings::SplitStr( 0, multiAddr, IntegScreenAddr, '|' );
			integScreenSndInfo[i].addr = atoi(IntegScreenAddr);

			int count = SplitStrings::SplitCharCount(multiAddr, '|') + 1;
			for (int index=1; index<count; index++) {
				memset( subWindowNum, 0, sizeof(subWindowNum));
				SplitStrings::SplitStr( index, multiAddr, subWindowNum, '|' );
				if ( atoi(subWindowNum) == atoi(windowNum) ) {
					printf("%d|%s\n", integScreenSndInfo[i].addr, sendData);
					bytes = PacketMessage(sendData, integScreenSndInfo[i].addr, (unsigned char *)sendBuffer);
					Com.WriteToPort(sendBuffer, bytes);
				}
			}
		}
	} else if (m_IntegShowMode == 3) { // 3 拼接
		
	} else {
		IMM9003Slog->WriteLog(LL_ERROR, "综合屏未知组合模式\n[%s|%d]", __FILE__, __LINE__);
	}

	return 0;
}

// 条屏初始化显示
int  IMM9003::InitStripScreen( )
{
	IMM9003Slog->WriteLog(LL_DEBUG, "IMM9003::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		//printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, stripScreenDefInfo[i].addr, 3, stripScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM9003::InitStripScreen( )\n");

	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

// 综合屏初始化显示
int  IMM9003::InitIntegScreen( )
{
	IMM9003Slog->WriteLog(LL_DEBUG, "IMM9003::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		//printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
	}
	/*
	Com.InitPort(1);

	printf("IMM9003::InitIntegScreen( )\n");
	
	char sendBuffer[1024];

	memset(sendBuffer, 0, sizeof(sendBuffer));

	Com.WriteToPort(sendBuffer, sizeof(sendBuffer));

	Com.ClosePort();
	*/

	return 0;
}

int  IMM9003::PacketMessage(const char *recvBuffer, int addr, unsigned char *sendBuffer)
{
	int index;

	index = 3;
	if (addr < 64) {                    // 0 ~ 63
		sendBuffer[0] = addr + 0X80;
		sendBuffer[1] = 0XC0;
	} else if (addr>=64 && addr<128) {  // 64 ~ 127
		sendBuffer[0] = addr + 0X40;    // addr + 0X80 - 0X40;
		sendBuffer[1] = 0XC0 + 0X01;
	} else if (addr>=128 && addr<192) { // 128 ~ 191
		sendBuffer[0] = addr;           // addr + 0X80 - 0X80;
		sendBuffer[1] = 0XC0 + 0X02;
	} else if (addr>=192 && addr<256) { // 192 ~ 255
		sendBuffer[0] = addr - 0X40;    // addr + 0X80 - 0X80 - 0X40;
		sendBuffer[1] = 0XC0 + 0X03;    // 0XC0 + 0X02 + 0X01;	
	}
	sendBuffer[2] = 0X44;
	
	for (int i=0; i<strlen(recvBuffer); i++)
	{
		if (recvBuffer[i] & 0X80) // 是汉字
		{
			sendBuffer[index++] = (recvBuffer[i]&0XF0)>>4;
			sendBuffer[index++] =  recvBuffer[i]&0X0F;
		}
		else // 非汉字
		{
			sendBuffer[index++] = recvBuffer[i];
		}
	}

	sendBuffer[index++] = 0X1A;

	return index;
}
