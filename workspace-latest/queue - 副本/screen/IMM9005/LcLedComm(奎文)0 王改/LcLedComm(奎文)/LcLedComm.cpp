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

//���� ��  �򿪶˿�
extern "C" HANDLE  PASCAL EXPORT  LedOpenComm(CString Commname, int Baudrate_int);

//���� ��  �رն˿�
extern "C" int  PASCAL EXPORT     LedCloseComm(HANDLE HComm);

//����     ���봰�ھ��,�򴰿�����������
extern "C" int  PASCAL EXPORT    LedSendData(HANDLE HComm ,CString ID,int PlayModl,int Playspeed,int PlayWait,int PlayNum,CString PlayData,int StreenType );

//����     ���봰�ھ��,���Ĵ������Ĺ̻���Ϣ
extern "C" int  PASCAL EXPORT    LedSetSolidData(HANDLE HComm ,CString ID,int PlayModl,int Playspeed,int PlayWait,int PlayNum,CString PlayData,int StreenType );

//����    ������������
extern "C" int  PASCAL EXPORT    LedSetScreen(HANDLE HComm ,CString Screen_id,CString NewId);

//���봮��/���ں�
HANDLE  PASCAL EXPORT LedOpenComm(CString Commname, int Baudrate_int)
{     
	memset(port,0,sizeof(port));
	strncpy(port,(LPCTSTR)Commname,sizeof(port));
	if(0 != LedPort.OpenPort(port))
	{
		SevErrWritelog(1,"��com[%s]ʧ��",port);
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
	unsigned char   LedData[321] = {0};		//�����Ҫ��ʾ������

	//jinqing add 2011-03-22
	sprintf(LedId,"%s",ID);
	SevErrWritelog(0," ���ŵ�ַ[%s]",LedId);
	if( strlen(LedId) != 6)
	{
		SevErrWritelog(0," ���ŵ�ַ����[%d]���󣬵�ַ����ӦΪ6��ʵ�ʴ���Ϊ[%s]",ID);
		return -1;
	}

	sprintf((char*)LedData,"%s",PlayData);
	if( strlen((char*)LedData) >= 320)
	{
		SevErrWritelog(0," ��Ҫ��ʾ������̫��[%d]!", strlen((char*)LedData));
		return -1;
	}
	else if (strlen((char*)LedData) == 0 )
	{
		strcpy((char*)LedData,"��ӭ����");
	}

	unsigned  char  Sendled[250];
	unsigned  char  Gcode[10];			//����ַ
// 	char			FileName[100];		
// 	char			ShowData[128];		
	/*char			LedId[10];	*/		
	/*unsigned char   LedData[321];		//�����Ҫ��ʾ������*/
	CString			strOpenShow;		//Ĭ����ʾ���ִ�
	
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
// 		::GetPrivateProfileString("LedShow","OPenData","��ӭ����",ShowData,200,FileName);
// 
// 		strOpenShow.Format("%s",ShowData);
// 		PlayData=strOpenShow;
// 	}
	
	//jinqing modify 2011-03-22
/*
		0	��ֹ
		1	����
		2	��Ľ
		3	��Ļ
		4	����
		5	��˸
		6	��ֱ��ҳ
		7	ˮƽ��ҳ
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

	//������Ҫ��ʾ�ִ��ĳ���
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
	//������Ҫ��ʾ�ִ����ۼӺ�
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
				SevErrWritelog(1," д����ʧ��%s",Sendled);
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
			
			//дǰ����������������
			PurgeComm(HComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			if(!WriteFile(HComm,Sendled,i+1,&nBytesWrite,NULL))
			{
				SevErrWritelog(1," д����ʧ��%s",Sendled);
				return -1;
			}
		} 
		Sleep(40);
	}

/*	SevErrWritelog(0," after for");*/

	return 0 ;
}

//���Ĺ̻���Ϣ
//jinqing add 20110323
int PASCAL EXPORT LedSetSolidData(HANDLE HComm ,CString ID,int PlayModl,int Playspeed,int PlayWait,int PlayNum,CString PlayData,int StreenType )
{
	char			LedId[10] = {0};
	unsigned char   LedData[321] = {0};		//�����Ҫ��ʾ������

	//jinqing add 2011-03-22
	sprintf(LedId,"%s",ID);
	if( strlen(LedId) != 6)
	{
		SevErrWritelog(0," ���ŵ�ַ����[%d]���󣬵�ַ����ӦΪ6��ʵ�ʴ���Ϊ[%s]",ID);
		return -1;
	}

	sprintf((char*)LedData,"%s",PlayData);
	if( strlen((char*)LedData) >= 320)
	{
		SevErrWritelog(0," ��Ҫ��ʾ������̫��[%d]!", strlen((char*)LedData));
		return -1;
	}

	unsigned  char  Sendled[250];
	unsigned  char  Gcode[10];			//����ַ
	char			FileName[100];		
	char			ShowData[128];		
	/*char			LedId[10];	*/		
	/*unsigned char   LedData[321];		//�����Ҫ��ʾ������*/
	CString			strOpenShow;		//Ĭ����ʾ���ִ�
	
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
		::GetPrivateProfileString("LedShow","OPenData","��ӭ����",ShowData,200,FileName);

		strOpenShow.Format("%s",ShowData);
		PlayData=strOpenShow;
	}

	/*sprintf((char*)LedData,"%s",PlayData);*/
	
	//jinqing modify 2011-03-22
/*
		0	��ֹ
		1	����
		2	��Ľ
		3	��Ļ
		4	����
		5	��˸
		6	��ֱ��ҳ
		7	ˮƽ��ҳ
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

	//������Ҫ��ʾ�ִ��ĳ���
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
	//������Ҫ��ʾ�ִ����ۼӺ�
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
				SevErrWritelog(1," д����ʧ��%s",Sendled);
			}
		}
		else
		{  			
			DWORD nBytesWrite;
			COMMTIMEOUTS timeouts;
			
			GetCommTimeouts(HComm,&timeouts);
			timeouts.WriteTotalTimeoutMultiplier = 40;
			SetCommTimeouts(HComm,&timeouts);
			
			//дǰ����������������
			PurgeComm(HComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			if(!WriteFile(HComm,Sendled,i+1,&nBytesWrite,NULL))
			{
				SevErrWritelog(1," д����ʧ��%s",Sendled);
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
		
		//дǰ����������������
		PurgeComm(HComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
		if(!WriteFile(HComm,Sendled,i+1,&nBytesWrite,NULL))
			return -1;
	} 
	return  0;	
}
