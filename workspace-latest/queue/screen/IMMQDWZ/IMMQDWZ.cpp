#include "IMMQDWZ.h"

Implement_DynCreate(IMMQDWZ, ScreenAdapter)

//#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMMQDWZ.ini")->GetParamValue(key, valueBuffer)

IMMQDWZ::IMMQDWZ(void)
{
	char temp[16];

	memset(temp, 0, sizeof(temp));
	SCParamValueINI("LOGLEVEL", temp);
	// ��־
	IMMQDWZSlog = new Log("log\\IMMQDWZ.log", temp);
	IMMQDWZSlog->WriteLog(LL_DEBUG, "IMMQDWZ::IMMQDWZ(void)");

	// ���ض�̬�� LEDSender2012.dll
	//int status = LoadLibrarys( "screen\\IMMQDWZ\\LEDSender2010.dll" );
	int status = LoadLibrarys( "LEDSender2010.dll" );
	if (status != 0)
		IMMQDWZSlog->WriteLog(LL_ERROR, "LoadLibrarys LEDSender2012.dll Error!\n[%s|%d]", __FILE__, __LINE__);
	else
		IMMQDWZSlog->WriteLog(LL_INFO, "LoadLibrarys LEDSender2012.dll Success!\n[%s|%d]", __FILE__, __LINE__);

	// ���ش���������
	loadStripScreenInfo();

	// �����ۺ�������
	loadIntegScreenInfo();

	// add by ����� at 2016-11-23 ����ۺ�����ʾ��ʽ begin
	memset(showFormat, 0, sizeof(showFormat));
	SCParamValueINI("SHOWFORMAT", showFormat);
	IMMQDWZSlog->WriteLog(LL_DEBUG, "SHOWFORMAT=[%s]\n[%s|%d]", showFormat, __FILE__, __LINE__);
	// add by ����� at 2016-11-23 ����ۺ�����ʾ��ʽ end

	// �����߶���
	InitMapIpAddr( );
	InitMapWinAdr( );
}

IMMQDWZ::~IMMQDWZ(void)
{
	// �ر���־
	IMMQDWZSlog->CloseLogFile();
}

// ���ش���������
int IMMQDWZ::loadStripScreenInfo()
{
	return 1;
}

// �����ۺ�������
int IMMQDWZ::loadIntegScreenInfo()
{
	return 1;
}

/***************************************
 *	����ʵ�ֵ��ⲿAPI�ӿڣ�����ܻص�  *
 ***************************************/

// �򿪴����豸
int  IMMQDWZ::InitiDev( )
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

	IMMQDWZSlog->WriteLog(LL_DEBUG, "���� :COM = [%s], ������    = [%s]\n[%s|%d]", comPort, comBaund, __FILE__, __LINE__);
	IMMQDWZSlog->WriteLog(LL_DEBUG, "showColor = [%s], showMode  = [%s]\n[%s|%d]", showColor, showMode, __FILE__, __LINE__);
	IMMQDWZSlog->WriteLog(LL_DEBUG, "showTime  = [%s], showSpeed = [%s]\n[%s|%d]", showTime, showSpeed, __FILE__, __LINE__);

	return 0;
}

// �رմ����豸
int  IMMQDWZ::CloseDev( )
{
	return 0;
}

// ������������Ϣ
int  IMMQDWZ::SendStripScreen( char *stripAddr, char *stripScreenInfo, char *stripShowMode, char *IntegScreenInfo )
{
	int status;

	//status = SendDatafun( m_ComPort, m_ComBaund, atoi(stripAddr), stripScreenInfo, m_ShowColor, m_ShowMode, m_ShowTime, m_ShowSpeed );
	// ��������
	SetMessageToMap( atoi(stripAddr), stripScreenInfo );
	// ˢ����Ļ
	SendMessageToScream( atoi(stripAddr), ConvertToIPBlock[atoi(stripAddr)]-'0' );

	status = 0;
	if (status == 0) {
		IMMQDWZSlog->WriteLog(LL_INFO, "SendStripScreen = [%s] status = [%d]\n[%s|%d]", stripScreenInfo, status,  __FILE__, __LINE__);
	} else {
		IMMQDWZSlog->WriteLog(LL_ERROR, "SendStripScreen = [%s] status = [%d]\n[%s|%d]", stripScreenInfo, status,  __FILE__, __LINE__);
	}

	return 0;
}

// ���ۺ���������Ϣ
int  IMMQDWZ::SendIntegScreen( char *recvData, int addr, int line, char *other )
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

	// �ֽ�ͻ��ţ����ں�
	SplitStrings::SplitStr( 0, recvData, custerNum, ',' );
	SplitStrings::SplitStr( 1, recvData, windowNum, ',' );
	//sprintf( sendData, "    ��%s�ŵ�%s���ڰ���ҵ��", custerNum, windowNum );
	//sprintf( sendData, "��%s��%s����", custerNum, windowNum );
	// version 0.1
	/*
	sprintf(strWindowNum, "%02d", windowNum);
	//custerNum
	//showFormat
	memcpy(strstr(showFormat, "XXXX"), custerNum, strlen("XXXX"));
	memcpy(strstr(showFormat, "XX"), strWindowNum, strlen("XX"));
	IMMQDWZSlog->WriteLog(LL_DEBUG, "showFormat= [%s]\n[%s|%d]", showFormat,  __FILE__, __LINE__);
	*/
	// �ֽ��ۺ�����ʾ��ʽ
	SplitStrings::SplitStr( 0, showFormat, preStr, 'X' );
	SplitStrings::SplitStr( 1, showFormat, midStr, 'X' );
	SplitStrings::SplitStr( 2, showFormat, endStr, 'X' );
	sprintf( sendData, "%s%s%s%s%s", preStr, custerNum, midStr, windowNum, endStr );

	for (int i=0; i<m_integSndNums; i++) {
		IMMQDWZSlog->WriteLog(LL_INFO, "integScreenSndInfo[%d].addr = [%d]\n[%s|%d]", i, integScreenSndInfo[i].addr,  __FILE__, __LINE__);
		//status = SendDataZHLine(m_ComPort, m_ComBaund, integScreenSndInfo[i].addr, sendData, 19);
		status = 0;
		if (status == 0) {
			IMMQDWZSlog->WriteLog(LL_INFO, "sendData = [%s] status = [%d]\n[%s|%d]", sendData,  status, __FILE__, __LINE__);
		} else {
			IMMQDWZSlog->WriteLog(LL_ERROR, "sendData = [%s] status = [%d]\n[%s|%d]", sendData,  status, __FILE__, __LINE__);
		}
		//if (status != 0)
		//	return status;
	}
	
	
	return 0;
}

// ��������ʼ����ʾ
int  IMMQDWZ::InitStripScreen( )
{	
	char temp[256];

	memset( temp, 0, sizeof(temp) );
	SCParamValueINI("WINAPPMAPNUM", temp);
	

	TSenderParam param;
	int K;

	printf("[%d]\n", atoi(temp));
	for (int index=0; index <atoi(temp); index++)
	{

		memset( &param, 0, sizeof(TSenderParam) );

		param.devParam.devType = 1;
		//printf("%d|%s\n", index+1, searchMapIp[index+1].c_str());
		param.devParam.dstAddr = mapWinAddr[ searchMapIp[index+1] ]->IpOffset;
		param.devParam.locPort = 8889;
		param.devParam.rmtPort = 6666;
		strcpy( param.devParam.rmtHost, (searchMapIp[index+1]).c_str());
		param.devParam.rmtHost[15] = 0;
		param.devParam.pkpLength=512;

		printf("param.devParam.dstAddr = [%d]\n", param.devParam.dstAddr);
		printf("param.devParam.rmtHost = [%s]\n", param.devParam.rmtHost);

		param.notifyMode = 2;
		
		K = MakeRoot(0x21, 2, -1);
		AddChapter(K, 30000, 1);
		AddRegion(K, 0, 0, 2368, 64, 0);
		AddLeaf(K, 10000, 1);
		
		for ( int indexWin=0; indexWin < mapWinAddr[ searchMapIp[index+1] ]->winNum; indexWin++ ) 
		{		
			AddText(K, \
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).x, \
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).y, \
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).width,  \
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).height, \
				1, \
				0, \
				(char *)(((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).showMsg),  \
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).fontName, \
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).fontSize, \
				0XFF, \
				0, 1, 0, \
				1, 0, 1, 0, 0, 0, 1000);
			
			AddText(K, \
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).x, \
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).y, \
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).width, \
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).height, \
				1, \
				0, \
				(char *)(((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).showMsg), \
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).fontName, \
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).fontSize, \
				0X00FF00, \
				0, 1, 1, \
				1, 0, 1, 0, 0, 0, 1000);

			AddText(K, \
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).x, \
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).y, \
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).width, \
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).height, \
				1, \
				0, \
				(char *)(((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).showMsg), \
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).fontName, \
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).fontSize, \
				0XFF, \
				0, 1, 1, \
				1, 0, 1, 0, 0, 0, 1000);
				
				//alignment
				//0:�����
				//1:����
				//2:�Ҷ���
				
		}
		
		LED_SendToScreen(&param, K);
	}

/*
	IMMQDWZSlog->WriteLog(LL_DEBUG, "IMMQDWZ::InitStripScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_stripDefNums; i++) {
		printf("%d\t%s\n", stripScreenDefInfo[i].addr, stripScreenDefInfo[i].defMessage);
		//SendDataOnly( m_ComPort, m_ComBaund, stripScreenDefInfo[i].addr, 3, stripScreenDefInfo[i].defMessage, 0x11 );
	}
	*/
	
	return 0;
}

// �ۺ�����ʼ����ʾ
int  IMMQDWZ::InitIntegScreen( )
{	
	IMMQDWZSlog->WriteLog(LL_DEBUG, "IMMQDWZ::InitIntegScreen( )\n[%s|%d]", __FILE__, __LINE__);
	for (int i=0; i<m_integDefNums; i++) {
		printf("%d\t%s\n", integScreenDefInfo[i].addr, integScreenDefInfo[i].defMessage);
//		SendDataOnly( m_ComPort, m_ComBaund, integScreenDefInfo[i].addr, 3, integScreenDefInfo[i].defMessage, 0x11 );
	}
	
	return 0;
}

int  IMMQDWZ::LoadLibrarys( char *pathname )
{
	HINSTANCE hDll = NULL;
	hDll = LoadLibrary( pathname );
	if (hDll == NULL) {
		IMMQDWZSlog->WriteLog(LL_ERROR, "���ؿ�:[%s]ʧ�ܣ�\n[%s|%d]", pathname, __FILE__, __LINE__);
		return -1;
	} else {
		IMMQDWZSlog->WriteLog(LL_DEBUG, "���ؿ�:[%s]�ɹ���\n[%s|%d]", pathname, __FILE__, __LINE__);
	}

	LED_Startup = (pFunLED_Startup) GetProcAddress( hDll, "LED_Startup" );
	if (LED_Startup != NULL) {
		IMMQDWZSlog->WriteLog(LL_DEBUG, "�򿪺���:LED_Startup �ɹ���\n[%s|%d]", __FILE__, __LINE__);
		
	} else {
		IMMQDWZSlog->WriteLog(LL_ERROR, "�򿪺���:LED_Startup ʧ�ܣ�\n[%s|%d]", __FILE__, __LINE__);
		return -1;
	}

	MakeRoot    = (pFunMakeRoot) GetProcAddress( hDll, "MakeRoot" );
	if (MakeRoot != NULL) {
		IMMQDWZSlog->WriteLog(LL_DEBUG, "�򿪺���:MakeRoot �ɹ���\n[%s|%d]", __FILE__, __LINE__);
		
	} else {
		IMMQDWZSlog->WriteLog(LL_ERROR, "�򿪺���:MakeRoot ʧ�ܣ�\n[%s|%d]", __FILE__, __LINE__);
		return -1;
	}

	AddChapter  = (pFunAddChapter) GetProcAddress( hDll, "AddChapter" );
	if (AddChapter != NULL) {
		IMMQDWZSlog->WriteLog(LL_DEBUG, "�򿪺���:AddChapter �ɹ���\n[%s|%d]", __FILE__, __LINE__);
		
	} else {
		IMMQDWZSlog->WriteLog(LL_ERROR, "�򿪺���:AddChapter ʧ�ܣ�\n[%s|%d]", __FILE__, __LINE__);
		return -1;
	}

	AddRegion   = (pFunAddRegion) GetProcAddress( hDll, "AddRegion" );
	if (AddRegion != NULL) {
		IMMQDWZSlog->WriteLog(LL_DEBUG, "�򿪺���:AddRegion �ɹ���\n[%s|%d]", __FILE__, __LINE__);
		
	} else {
		IMMQDWZSlog->WriteLog(LL_ERROR, "�򿪺���:AddRegion ʧ�ܣ�\n[%s|%d]", __FILE__, __LINE__);
		return -1;
	}

	AddLeaf	= (pFunAddLeaf) GetProcAddress( hDll, "AddLeaf" );
	if (AddLeaf != NULL) {
		IMMQDWZSlog->WriteLog(LL_DEBUG, "�򿪺���:AddLeaf �ɹ���\n[%s|%d]", __FILE__, __LINE__);
	} else {
		IMMQDWZSlog->WriteLog(LL_ERROR, "�򿪺���:AddLeaf ʧ�ܣ�\n[%s|%d]", __FILE__, __LINE__);
		return -1;
	}

	AddText	= (pFunAddText) GetProcAddress( hDll, "AddText" );
	if (AddText != NULL) {
		IMMQDWZSlog->WriteLog(LL_DEBUG, "�򿪺���:AddText �ɹ���\n[%s|%d]", __FILE__, __LINE__);
	} else {
		IMMQDWZSlog->WriteLog(LL_ERROR, "�򿪺���:AddText ʧ�ܣ�\n[%s|%d]", __FILE__, __LINE__);
		return -1;
	}

	LED_SendToScreen = (pFunLED_SendToScreen) GetProcAddress( hDll, "LED_SendToScreen" );
	if (LED_SendToScreen != NULL) {
		IMMQDWZSlog->WriteLog(LL_DEBUG, "�򿪺���:LED_SendToScreen �ɹ���\n[%s|%d]", __FILE__, __LINE__);
	} else {
		IMMQDWZSlog->WriteLog(LL_ERROR, "�򿪺���:LED_SendToScreen ʧ�ܣ�\n[%s|%d]", __FILE__, __LINE__);
		return -1;
	}

	if (LED_Startup != NULL)
		LED_Startup();
	
	return 0;
}

// ˢ��IPһ����
void IMMQDWZ::SendMessageToScream( int winAddr, int index)
{
	TSenderParam param;
	int K;
	memset( &param, 0, sizeof(TSenderParam) );

	param.devParam.devType = 1;
	param.devParam.dstAddr = mapWinAddr[ mapIp[winAddr] ]->IpOffset;
	param.devParam.locPort = 8889;
	param.devParam.rmtPort = 6666;
	strcpy( param.devParam.rmtHost, mapIp[winAddr].c_str());
	param.devParam.rmtHost[15] = 0;
    param.devParam.pkpLength=512;

	param.notifyMode = 2;
	
	K = MakeRoot(0x21, 2, -1);
	AddChapter(K, 30000, 1);
	AddRegion(K, 0, 0, 2368, 64, 0);
	AddLeaf(K, 10000, 1);
	
	/*
	AddText(K, 0, 0, 320, 64, 1, 0, "84", "����", 30, 0XFF, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1000);
	AddText(K, 100, 0, 220, 32, 1, 0, "�ۺϷ���", "����", 14, 0X00FF00, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1000);
	AddText(K, 100, 32, 220, 32, 1, 0, "��ӭ����", "����", 14, 0XFF, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1000);

	AddText(K, 320, 0, 320, 64, 1, 0, "85", "����", 30, 0XFF, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1000);
	AddText(K, 420, 0, 220, 32, 1, 0, "�ۺϷ���", "����", 14, 0X00FF00, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1000);
	AddText(K, 420, 32, 220, 32, 1, 0, "��ӭ����", "����", 14, 0XFF, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1000);
	*/
	
	for ( int indexWin=0; indexWin < mapWinAddr[ searchMapIp[index+1] ]->winNum; indexWin++ ) 
	{		
		AddText(K, \
			((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).x, \
			((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).y, \
			((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).width,  \
			((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).height, \
			1, \
			0, \
			(char *)(((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).showMsg),  \
			((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).fontName, \
			((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).fontSize, \
			0XFF, \
			0, 1, 0, \
			1, 0, 1, 0, 0, 0, 1000);
		
		AddText(K, \
			((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).x, \
			((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).y, \
			((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).width, \
			((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).height, \
			1, \
			0, \
			(char *)(((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).showMsg), \
			((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).fontName, \
			((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).fontSize, \
			0X00FF00, \
			0, 1, 1, \
			1, 0, 1, 0, 0, 0, 1000);

		AddText(K, \
			((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).x, \
			((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).y, \
			((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).width, \
			((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).height, \
			1, \
			0, \
			(char *)(((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).showMsg), \
			((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).fontName, \
			((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).fontSize, \
			0XFF, \
			0, 1, 1, \
			1, 0, 1, 0, 0, 0, 1000);
			/*
			alignment
			0:�����
			1:����
			2:�Ҷ���
			*/
	}
	
	LED_SendToScreen(&param, K);
}

// ��������
int  IMMQDWZ::SetMessageToMap( int winAddr, char *sendBuffer )
{
	char *pszStr;	

	pszStr = (char *) ((mapWinAddr[ mapIp[winAddr] ])->strushowMessage[winAddr - (mapWinAddr[mapIp[winAddr]]->winOffset)]).showMsg;
	memset(pszStr, 0, 32);
	strncpy(pszStr, sendBuffer, strlen(sendBuffer));
	
	return 0;
}
