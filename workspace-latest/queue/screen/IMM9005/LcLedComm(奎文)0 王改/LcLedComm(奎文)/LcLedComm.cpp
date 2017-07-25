// LcLedComm.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "LcLedComm.h"
#include "PortFun.h"
#include "Drvapi.h"
#include <time.h>

#include "DIRECT.H"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CLcLedCommApp

BEGIN_MESSAGE_MAP(CLcLedCommApp, CWinApp)
//{{AFX_MSG_MAP(CLcLedCommApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLcLedCommApp construction

CLcLedCommApp::CLcLedCommApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLcLedCommApp object

CLcLedCommApp	theApp;
HANDLE			HComm;
CPortFun		LedPort;
char			port[10];

//功能 ：  打开端口
extern "C" HANDLE  PASCAL EXPORT  LedOpenComm(CString Commname, int Baudrate_int);

//功能 ：  关闭端口
extern "C" int  PASCAL EXPORT     LedCloseComm(HANDLE HComm);

//功能     传入窗口句柄,向窗口屏发送数据
extern "C" int  PASCAL EXPORT    LedSendData(HANDLE HComm ,CString ID,int PlayModl,int Playspeed,int PlayWait,int PlayNum,CString PlayData,int StreenType );

//功能     传入窗口句柄,更改窗口屏的固化信息
extern "C" int  PASCAL EXPORT    LedSetSolidData(HANDLE HComm ,CString ID,int PlayModl,int Playspeed,int PlayWait,int PlayNum,CString PlayData,int StreenType );

//功能    重新设置屏号
extern "C" int  PASCAL EXPORT    LedSetScreen(HANDLE HComm ,CString Screen_id,CString NewId);

//传入串口/并口号
HANDLE  PASCAL EXPORT LedOpenComm(CString Commname, int Baudrate_int)
{     
	memset(port,0,sizeof(port));
	strncpy(port,(LPCTSTR)Commname,sizeof(port));
	if(0 != LedPort.OpenPort(port))
	{
		SevErrWritelog(1,"打开com[%s]失败",port);
	}
	
	return  LedPort.m_hPort;
}


int  PASCAL EXPORT LedCloseComm(HANDLE HComm)
{
	if (HComm != NULL)
	{
		CloseHandle(HComm);
		HComm = NULL;
    }

	LedPort.ClosePort();

	return  0;
}

int PASCAL EXPORT LedSendData(HANDLE HComm ,CString ID,int PlayModl,int Playspeed,int PlayWait,int PlayNum,CString PlayData,int StreenType )
{
	char			LedId[10] = {0};
	unsigned char   LedData[321] = {0};		//存放需要显示的数据

	//jinqing add 2011-03-22
	sprintf(LedId,"%s",ID);
	SevErrWritelog(0," 屏号地址[%s]",LedId);
	if( strlen(LedId) != 6)
	{
		SevErrWritelog(0," 屏号地址长度[%d]错误，地址长度应为6，实际传入为[%s]",ID);
		return -1;
	}

	sprintf((char*)LedData,"%s",PlayData);
	if( strlen((char*)LedData) >= 320)
	{
		SevErrWritelog(0," 需要显示的数据太长[%d]!", strlen((char*)LedData));
		return -1;
	}
	else if (strlen((char*)LedData) == 0 )
	{
		strcpy((char*)LedData,"欢迎光临");
	}

	unsigned  char  Sendled[250];
	unsigned  char  Gcode[10];			//屏地址
// 	char			FileName[100];		
// 	char			ShowData[128];		
	/*char			LedId[10];	*/		
	/*unsigned char   LedData[321];		//存放需要显示的数据*/
	CString			strOpenShow;		//默认显示的字串
	
	memset(Gcode,0,sizeof(Gcode));
	/*memset(LedId,0,sizeof(LedId));*/
	/*memset(LedData,0,sizeof(LedData));*/
	memset(Sendled,0,sizeof(Sendled));

	memcpy(Sendled,"\x23\x14\x24\x15\x13\x18",6);

	/*strncpy(LedId,(LPCTSTR)ID,sizeof(LedId));*/
	memcpy(&Sendled[6],LedId,sizeof(LedId));

	Sendled[12]='D';
	
// 	memset( FileName, 0, sizeof(FileName));
// 	memset( ShowData, 0, sizeof(ShowData));
// 	
// 	_getcwd( FileName, sizeof(FileName));
// 	strcat( FileName, "\\Server.ini" );

// 	if (PlayData == "")
// 	{
// 		PlayModl	=::GetPrivateProfileInt("LedShow","StartPlayModl",1,FileName); 
// 		::GetPrivateProfileString("LedShow","OPenData","欢迎光临",ShowData,200,FileName);
// 
// 		strOpenShow.Format("%s",ShowData);
// 		PlayData=strOpenShow;
// 	}
	
	//jinqing modify 2011-03-22
/*
		0	静止
		1	左推
		2	关慕
		3	开幕
		4	上推
		5	闪烁
		6	垂直百页
		7	水平百页
*/
	if( PlayModl < 0 )
		PlayModl = 0;
	else if( PlayModl > 7)
		PlayModl = 7;

	Sendled[13]=PlayModl;

	if( Playspeed < 1)
		Playspeed = 1;
	else if( Playspeed > 15)
		Playspeed = 15;
	
    Sendled[14]=Playspeed;

	if( Playspeed < 0)
		Playspeed = 0;
	else if( Playspeed > 15)
		Playspeed = 15;

	Sendled[15]=PlayWait;

	if( PlayNum < 1)
		PlayNum = 1;
	else if( PlayNum > 255)
		PlayNum = 255;

	Sendled[16]=PlayNum;

	//计算需要显示字串的长度
	int len=0,Count=0;	
	int i=0;
	//strncpy((char*)LedData,(LPCTSTR)PlayData,sizeof(LedData));

	while(LedData[i]!=0)
	{
		if(LedData[i]>=0x80)
		{			
			i=i+2;
			Count++;
		}
		else
		{			
			i++;
			Count++;
		} 
	}

	len=i;

    Sendled[17]=Count;
	memcpy(&Sendled[18],LedData,len);

	/////////////////////////////////////
	
	Sendled[18+len+6]=13;	
	Sendled[18+len+5]=2;	
	Sendled[18+len+4]=3;
	Sendled[18+len+3]=12;	
	Sendled[18+len+2]=13;
	Sendled[18+len+1]=56;
	Sendled[18+len]=7;
	
	//////////////////////////
	//计算需要显示字串的累加和
	int sum=0;
    i=12;

   	while(Sendled[i]!=0 || i<=18+len+6)
	{
		sum+=Sendled[i];
		i++;
	}

    Sendled[i] =sum%256;
	//i++; //12.11
    //Sendled[i] =0xFF; //12.11

	for (int j=0;j<3;j++)
	{
		if (HComm==NULL)
		{
			if(LedPort.WritePort(Sendled,i+1)<0)
			{
				SevErrWritelog(1," 写串口失败%s",Sendled);
				return -1;
			}
		}
		else
		{  			
			DWORD nBytesWrite;
			COMMTIMEOUTS timeouts;
			
			GetCommTimeouts(HComm,&timeouts);
			timeouts.WriteTotalTimeoutMultiplier = 40;
			SetCommTimeouts(HComm,&timeouts);
			
			//写前清空输入输出缓冲区
			PurgeComm(HComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			if(!WriteFile(HComm,Sendled,i+1,&nBytesWrite,NULL))
			{
				SevErrWritelog(1," 写串口失败%s",Sendled);
				return -1;
			}
		} 
		Sleep(40);
	}

/*	SevErrWritelog(0," after for");*/

	return 0 ;
}

//更改固化信息
//jinqing add 20110323
int PASCAL EXPORT LedSetSolidData(HANDLE HComm ,CString ID,int PlayModl,int Playspeed,int PlayWait,int PlayNum,CString PlayData,int StreenType )
{
	char			LedId[10] = {0};
	unsigned char   LedData[321] = {0};		//存放需要显示的数据

	//jinqing add 2011-03-22
	sprintf(LedId,"%s",ID);
	if( strlen(LedId) != 6)
	{
		SevErrWritelog(0," 屏号地址长度[%d]错误，地址长度应为6，实际传入为[%s]",ID);
		return -1;
	}

	sprintf((char*)LedData,"%s",PlayData);
	if( strlen((char*)LedData) >= 320)
	{
		SevErrWritelog(0," 需要显示的数据太长[%d]!", strlen((char*)LedData));
		return -1;
	}

	unsigned  char  Sendled[250];
	unsigned  char  Gcode[10];			//屏地址
	char			FileName[100];		
	char			ShowData[128];		
	/*char			LedId[10];	*/		
	/*unsigned char   LedData[321];		//存放需要显示的数据*/
	CString			strOpenShow;		//默认显示的字串
	
	memset(Gcode,0,sizeof(Gcode));
	/*memset(LedId,0,sizeof(LedId));*/
	/*memset(LedData,0,sizeof(LedData));*/
	memset(Sendled,0,sizeof(Sendled));

	memcpy(Sendled,"\x23\x14\x24\x15\x13\x18",6);

	/*strncpy(LedId,(LPCTSTR)ID,sizeof(LedId));*/
	memcpy(&Sendled[6],LedId,sizeof(LedId));

	Sendled[12]='H';
	
	memset( FileName, 0, sizeof(FileName));
	memset( ShowData, 0, sizeof(ShowData));
	
	_getcwd( FileName, sizeof(FileName));
	strcat( FileName, "\\Server.ini" );
	
	if (PlayData == "")
	{
		PlayModl	=::GetPrivateProfileInt("LedShow","StartPlayModl",1,FileName); 
		::GetPrivateProfileString("LedShow","OPenData","欢迎光临",ShowData,200,FileName);

		strOpenShow.Format("%s",ShowData);
		PlayData=strOpenShow;
	}

	/*sprintf((char*)LedData,"%s",PlayData);*/
	
	//jinqing modify 2011-03-22
/*
		0	静止
		1	左推
		2	关慕
		3	开幕
		4	上推
		5	闪烁
		6	垂直百页
		7	水平百页
*/
	if( PlayModl < 0 )
		PlayModl = 0;
	else if( PlayModl > 7)
		PlayModl = 7;

	Sendled[13]=PlayModl;

	if( Playspeed < 1)
		Playspeed = 1;
	else if( Playspeed > 15)
		Playspeed = 15;
	
    Sendled[14]=Playspeed;

	if( Playspeed < 0)
		Playspeed = 0;
	else if( Playspeed > 15)
		Playspeed = 15;

	Sendled[15]=PlayWait;

	if( PlayNum < 1)
		PlayNum = 1;
	else if( PlayNum > 255)
		PlayNum = 255;

	Sendled[16]=PlayNum;

	//计算需要显示字串的长度
	int len=0,Count=0;	
	int i=0;
	//strncpy((char*)LedData,(LPCTSTR)PlayData,sizeof(LedData));

	while(LedData[i]!=0)
	{
		if(LedData[i]>=0x80)
		{			
			i=i+2;
			Count++;
		}
		else
		{			
			i++;
			Count++;
		} 
	}

	len=i;

    Sendled[17]=Count;
	memcpy(&Sendled[18],LedData,len);

	/////////////////////////////////////
	
	Sendled[18+len+6]=7;	
	Sendled[18+len+5]=7;	
	Sendled[18+len+4]=7;
	Sendled[18+len+3]=7;	
	Sendled[18+len+2]=7;
	Sendled[18+len+1]=7;
	Sendled[18+len]=7;
	
	//////////////////////////
	//计算需要显示字串的累加和
	int sum=0;
    i=12;
   	while(Sendled[i]!=0 || i<=18+len+6)
	{
		sum+=Sendled[i];
		i++;
	}
    Sendled[i] =sum%256;
	i++; //12.11
    Sendled[i] =0xFF; //12.11
	for (int j=0;j<3;j++)
	{		
		if (HComm==NULL)
		{
			if(LedPort.WritePort(Sendled,i+1)<0)
			{
				SevErrWritelog(1," 写串口失败%s",Sendled);
			}
		}
		else
		{  			
			DWORD nBytesWrite;
			COMMTIMEOUTS timeouts;
			
			GetCommTimeouts(HComm,&timeouts);
			timeouts.WriteTotalTimeoutMultiplier = 40;
			SetCommTimeouts(HComm,&timeouts);
			
			//写前清空输入输出缓冲区
			PurgeComm(HComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			if(!WriteFile(HComm,Sendled,i+1,&nBytesWrite,NULL))
			{
				SevErrWritelog(1," 写串口失败%s",Sendled);
				return -1;
			}
		} 
		Sleep(40);
	}
	return 0 ;
}

int  PASCAL EXPORT   LedSetScreen(HANDLE HComm ,CString Screen_id,CString NewId)
{
	unsigned  char  Sendled[100];
	unsigned  char  Gcode[6];
	char      LedOldId[6];
    char     LedNewId[6];
	unsigned char      LedData[30];
	memset(Gcode,0,sizeof(Gcode));
	memset(LedOldId,0,sizeof(LedOldId));
	memset(LedNewId,0,sizeof(LedNewId));
	memset(LedData,0,sizeof(LedData));
	memset(Sendled,0,sizeof(Sendled));
	
	memcpy(Sendled,"\x23\x14\x24\x15\x13\x18",6);
	strncpy(LedOldId,(LPCTSTR)Screen_id,sizeof(LedOldId));
    strncpy(LedNewId,(LPCTSTR)NewId,sizeof(LedNewId));
	memcpy(&Sendled[6],LedOldId,6);
    Sendled[12]='S';
    Sendled[13]='W';
	memcpy(&Sendled[14],LedNewId,6);
    int    i=12;
	int    sum=0;
   	while(Sendled[i]!=0)
	{
		sum+=Sendled[i];
		i++;
	}
	
	Sendled[i] =sum;
	
	if (HComm==NULL)
	{
		LedPort.WritePort(Sendled,i+1);
	}
    else
	{   
		
		DWORD nBytesWrite;
		COMMTIMEOUTS timeouts;
		
		
		GetCommTimeouts(HComm,&timeouts);
		timeouts.WriteTotalTimeoutMultiplier = 30;
		SetCommTimeouts(HComm,&timeouts);
		
		//写前清空输入输出缓冲区
		PurgeComm(HComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
		if(!WriteFile(HComm,Sendled,i+1,&nBytesWrite,NULL))
			return -1;
	} 
	return  0;	
}
