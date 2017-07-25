#include "SerialFun.h"

long Serial_InitialDev(int ch_port,int baund)
{
	char ch_log[1000];
	char ch_ori_com[10];

	if ( 0 == CFG_ReadStr( "CFG\\CFG.ini", "OriComm", ch_ori_com ) )
	{
		m_seriallog.Log( "[0] read ori comm  ok" );
		m_seriallog.Log( ch_ori_com );

		m_handle = OpenPort( ch_ori_com, 9600 );
		if ( m_handle == NULL ) {
			sprintf( ch_log, "[OPEN] open serial COM%s false.", ch_ori_com );
			m_seriallog.Log( ch_log );
		} else {
			sprintf( ch_log, "[OPEN] open serial COM%s over.", ch_ori_com );
			m_seriallog.Log( ch_log );
		}
	}
	return 0;
}

long Serial_CloseDev( )
{
	return 0;
}

long Serial_SetOperatorLog( char *ch_logname )
{
	m_seriallog.SetFileName(ch_logname);
	char ch_log[1000];
	sprintf(ch_log,"[0] set serial log name,name=%s",ch_logname);
	m_seriallog.Log("***************START NEW LOG***************");
	m_seriallog.Log(ch_log);

	// 加载第三方库
	LoadOtherLibrary(NULL);

	// 初始化综合屏缓冲区
	InitialZhData();

	return 0;
}

long Serial_SendTP( char *ch_data, int addr, char *showmode, char *ch_other )
{
	char ch_send[300];
	char ch_log[100];
	char ch_addr[10];

	memset( ch_addr, 0, sizeof(ch_addr) );
	sprintf(ch_addr, "1000%02d", addr);

    result = DataSend( m_handle, ch_addr, 0, 1, 1, 1, ch_data, 1 );
	if ( result == 0 ) {
		sprintf(ch_log,"[0] send tp ok ,addr=%s,data=%s",ch_addr,ch_data);
		m_seriallog.Log(ch_log);
		return 0;
	} else {
		sprintf(ch_log,"[0] send tp error ,addr=%s,data=%s",ch_addr,ch_data);
		m_seriallog.Log(ch_log);
		return -1;
	}
	
}

long Serial_SendZH( char *ch_data, int addr, int line, char *ch_other )
{	
	char ch_addr[10];
	char ch_gk[10];
	char ch_win[5];
	char ch_log[1000];
	char ch_send[30];
	int  i = 0;

	memset( ch_gk  , 0, 10 );
	memset( ch_win , 0, 5  );
	memset( ch_addr, 0, 10 );

	slipchar( 0, ch_data, ch_gk , ',' );
	slipchar( 1, ch_data, ch_win, ',' );

	sprintf( ch_send, "请%s号到%02d窗口", ch_gk, atoi(ch_win) );
	UpdateZhData( ch_send );

	for ( i=0; i<4; i++ )
	{
		sprintf( ch_addr, "%d", 300001+i );
		result = DataSend( m_handle, ch_addr, 0, 1, 1, 1, ch_zh[i], 1 );
		if ( result == 0 ) {
			sprintf( ch_log, "[0] send zh ok   , addr=%s, data=%s", ch_addr, ch_zh[i] );
			m_seriallog.Log( ch_log );
		} else {
			sprintf( ch_log, "[0] send zh error, addr=%s, data=%s", ch_addr, ch_zh[i] );
			m_seriallog.Log( ch_log );
		}
	}

	return 0;	
}

long Serial_InitialTpShow()
{
	return 0;
}

long Serial_InitialZhShow()
{
	char ch_log[1000];
	sprintf(ch_log,"[0] Serial_InitialZhShow");
	m_seriallog.Log(ch_log);
	return 0;
}

long LoadOtherLibrary(char * path)
{
	HINSTANCE hDll;		
	hDll = LoadLibrary("DLL_EX\\LcLedComm.dll");            //lcLedComm.dll

	if(hDll!=NULL)
	{
		m_seriallog.Log("LOAD ok");
		OpenPort = (LpOpenPort)GetProcAddress(hDll, "LedOpenComm");
		if(OpenPort !=NULL)
		{
			m_seriallog.Log("Get Fun port open ok");
		}
		else
		{
			m_seriallog.Log("Get Fun port open error");
		}
		ClosePort = (LpClosePort)GetProcAddress(hDll, "LedCloseComm");
		if(ClosePort !=NULL)
		{
			m_seriallog.Log("Get Fun port close ok");
		}
		else
		{
			m_seriallog.Log("Get Fun port close error");
		}
		DataSend=(LpDataSend)GetProcAddress(hDll, "LedSendData");
		if(DataSend !=NULL)
		{
			m_seriallog.Log("Get Fun datasend ok");
		}
		else
		{
			m_seriallog.Log("Get Fun datasend error");
		}
		
	}
	else
	{
		m_seriallog.Log("LOAD ERROR");
	}
	m_handle=NULL;
	return 0;
}

void slipchar(int count,char * INCHAR,char * OUTCHAR,char ch)
{	
	int i=0,j=0,k=0;
	for(i=0;i<strlen(INCHAR);i++)
	{
		if(INCHAR[i]==ch)
		{
			j++;
			if(j-1==count)
				break;
		}
		else
		{
			if(j==count)
			{
			OUTCHAR[k++]=INCHAR[i];
			}
		}
	}
}


int Slipcharcount(char * INCHAR,char ch)
{
	int i,k=0;
	for(i=0;i<strlen(INCHAR);i++)
	{
		if(INCHAR[i]==ch)
		{
			k++;
		}
	}
	return k;
}

long InitialZhData()
{
	int i=0;
	for(i=0;i<4;i++)
	{
		memset(ch_zh[i],0,30);
	}
	return 0;
}

long UpdateZhData(char * ch)
{
	int i = 0;

	for( i=0; i<3; i++ )
	{
		sprintf( ch_zh[i], "%s", ch_zh[i+1] );
	}
	sprintf( ch_zh[3], "%s", ch );

	return 0;
}