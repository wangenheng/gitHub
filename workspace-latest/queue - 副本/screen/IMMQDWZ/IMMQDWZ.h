#pragma once

#pragma warning( disable : 4786 )
#pragma warning( disable : 4518 )
#pragma warning( disable : 4228 )

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <map>

#include "../../include/framework.h"
#include "../../basefun.h"

using namespace std;

// 青岛未知品牌IP地址一体屏

#define MAXLINE	128

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMMQDWZ.ini")->GetParamValue(key, valueBuffer)

namespace IMMQDWZNM
{
	// 第三方自定义结构体
	typedef struct DEVICE_PARAM{
		WORD  devType;
		WORD  comSpeed;
		WORD  comPort;
		WORD  comFlow;
		WORD  locPort;
		WORD  rmtPort;
		WORD  srcAddr;
		WORD  dstAddr;
		char  rmtHost[16];
		DWORD txTimeo;   //发送后等待应答时间 ====超时时间应为txTimeo*txRepeat
		DWORD txRepeat;  //失败重发次数
		DWORD txMovewin; //划动窗口
		DWORD key;
		long  pkpLength; //数据包大小
	}TDeviceParam, *PDeviceParam;

	typedef struct SENDER_PARAM{
		TDeviceParam devParam;
		long  wmHandle;
		long  wmMessage;
		long  wmLParam;
		long  notifyMode;
	}TSenderParam, *PSenderParam;

	// 请根据第三方接口自定义函数指针

	//动态链接库初始化
	typedef long (_stdcall *pFunLED_Startup)(void);
	//生成播放数据
	typedef long (_stdcall *pFunMakeRoot)(long RootType, long ColorMode, long survive = -1);
	//添加节目
	typedef long (_stdcall *pFunAddChapter)(WORD num, DWORD time, WORD wait); //time为毫秒
	//添加分区
	typedef long (_stdcall *pFunAddRegion)(WORD num, long left, long top, long width, long height, long border);
	//添加页面
	typedef long (_stdcall *pFunAddLeaf)(WORD num, DWORD time, WORD wait); //time为毫秒

	typedef long (_stdcall *pFunAddText)(WORD num, long left, long top, long width, long height, long transparent, long border,
		char* str, char* fontname, long fontsize, long fontcolor, long fontstyle, long wordwrap, long alignment, 
		long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime); //stoptime单位为毫秒

	typedef long (_stdcall *pFunLED_SendToScreen)(PSenderParam param, long index);

	// 第三方函数指针初始化
	pFunLED_Startup      LED_Startup      = NULL;
	pFunMakeRoot         MakeRoot         = NULL;
	pFunAddChapter       AddChapter       = NULL;
	pFunAddRegion        AddRegion        = NULL;
	pFunAddLeaf          AddLeaf          = NULL; 
	pFunAddText			 AddText		  = NULL;
	pFunLED_SendToScreen LED_SendToScreen = NULL;

	// 自定义数据结构 begin
	typedef struct regions {
		int  x,
		int  y,
		int  width,
		int  height,
		char showMsg[32];
		char fontName[32];
		int  fontSize;
	} Region;

	typedef struct winNumMsg {
		Region *struWinNum;		// 窗口号
		Region *strutitleName;	// 标题
		Region *strushowMessage;// 动态信息
		int     IpOffset;	// IP屏地址 取值:19 20
		int		winNum;		// 窗口数量
		int		winOffset;	// 窗口偏移量
	} WinNumMsg;

	// 窗口号映射IP地址
	typedef pair< int, string >			_KIP_;
	extern  map< int, string >			mapIp;
	// 初始化
	void InitMapIpAddr( );

	// IP地址映射一体屏区域
	typedef pair< string, WinNumMsg * >	_KWinNumMsg_;
	extern  map< string, WinNumMsg * >    mapWinAddr;
	// 初始化
	void InitMapWinAdr( );

	// 遍历索引
	extern  map<int, string>		 searchMapIp;

	// 窗口换算ip
	extern char ConvertToIPBlock[256];

	// 遍历线段树
	void VisitMapsTree( );


/************************************************************/
	// 构建线段树数据结构
	map<string, WinNumMsg *> mapWinAddr;
	map<int, string>         mapIp; 

	// 遍历索引
	map<int, string>		 searchMapIp;

	// 构建换算索引
	char ConvertToIPBlock[256];

	void InitMapWinAdr( )
	{
		char winNums[8];
		int  WinNumIndex;
		
		WinNumIndex = 0;
		memset( winNums, 0, sizeof(winNums) );
		// "WINAPPMAPNUM"
		SCParamValueINI( "WINAPPMAPNUM", winNums );
		//printf("WINAPPMAPNUM:%d\n", atoi(winNums));
		for (int i=0; i<atoi(winNums); i++)
		{
			char strFormat[16];
			char strValues[512];
			char strWinNums[128];
			char strDspIpAdr[32];
			
			memset( strFormat  , 0, sizeof(strFormat)   );
			memset( strValues  , 0, sizeof(strValues)   );
			memset( strWinNums , 0, sizeof(strWinNums)  );
			memset( strDspIpAdr, 0, sizeof(strDspIpAdr) );

			sprintf( strFormat, "WINADDRTOIP%d", i+1 );
			SCParamValueINI( strFormat, strValues );

			//printf("strValues = [%s]\n", strValues );

			SplitStrings::SplitStr( 0, strValues, strWinNums , '-' );
			SplitStrings::SplitStr( 1, strValues, strDspIpAdr, '-' );
			//printf("strWinNums = [%s]\n", strWinNums );
			//printf("strDspIpAdr = [%s]\n", strDspIpAdr );

			cltrim(strWinNums);
			crtrim(strWinNums);
			cltrim(strDspIpAdr);
			crtrim(strDspIpAdr);
			//printf( "strWinNums = [%s]\n", strWinNums );
			//printf( "strDspIpAdr = [%s]\n", strDspIpAdr );

			int count = SplitStrings::SplitCharCount( strWinNums, ',');
			//printf( "count = [%d]\n", count );
			for (int index=0; index<=count; index++)
			{
				char t[32];
				memset( t, 0, sizeof(t) );
				SplitStrings::SplitStr( index, strWinNums, t, ',' );
				//printf( "t = [%s]\n", t );
				//printf( "strDspIpAdr = [%s]\n", strDspIpAdr );

				// 插入map
				//mapWinAddr.insert(_KIP_(atoi(t), strDspIpAdr));
				mapIp.insert( _KIP_(atoi(t), strDspIpAdr) );
				//printf("index = [%d]\n", index);
				// 构建遍历索引
				if (index == 0) {
					static int searchIndex = 1; // 注意static的妙用
					//printf("%d|%s\n", searchIndex, strDspIpAdr);
					searchMapIp.insert( _KIP_(searchIndex++, strDspIpAdr) );
					
				}
				// 构建换算索引
				ConvertToIPBlock[++WinNumIndex] = i+'0';
				//printf("%d|%c\n",  WinNumIndex, ConvertToIPBlock[WinNumIndex]);
			}
		}

		// 查找数据
		map<int, string>::iterator iter;
		//printf( "%s\n", mapWinAddr[1].c_str() );
		
		/*
		for (int k=0; k<15; k++)
		{
			iter = mapIp.find(k+1);
			if ( iter != mapIp.end() )
			{
				printf( "%s\n", iter->second.c_str() );
			}
			else
			{
				printf( "not find value\n" );
			}
		}
		*/
	}

	void InitMapIpAddr( )
	{
		char temp[256];
		char Values[256];
		char strFormat[256];

		memset( Values, 0, sizeof(Values) );
		SCParamValueINI( "IPNUM", Values );
		//printf( "Values = [%s]\n", Values );

		for (int i=0; i<atoi(Values); i++)
		{
			WinNumMsg *winNumMsg = new WinNumMsg;

			memset( temp, 0, sizeof(temp) );
			memset( strFormat, 0, sizeof(strFormat) );
			sprintf( strFormat, "IP%d", i+1 );
			SCParamValueINI( strFormat, temp );
			//printf("%s,temp = [%s]\n", strFormat, temp);

			char IpAddress[256];
			char IPOffset[8];
			char strWinNum[8];
			char strWinOffset[8];

			memset( IpAddress   , 0, sizeof(IpAddress)    );
			memset( IPOffset    , 0, sizeof(IPOffset)     );
			memset( strWinNum   , 0, sizeof(strWinNum)    );
			memset( strWinOffset, 0, sizeof(strWinOffset) );

			SplitStrings::SplitStr( 0, temp, IpAddress   , '|' );
			SplitStrings::SplitStr( 1, temp, IPOffset    , '|' );
			SplitStrings::SplitStr( 2, temp, strWinNum   , '|' );
			SplitStrings::SplitStr( 3, temp, strWinOffset, '|' );
			
			winNumMsg->IpOffset			= atoi( IPOffset     );
			winNumMsg->winNum			= atoi( strWinNum    );
			winNumMsg->winOffset		= atoi( strWinOffset );
			winNumMsg->struWinNum		= new Region[ atoi(strWinNum) ];
			winNumMsg->strutitleName	= new Region[ atoi(strWinNum) ];
			winNumMsg->strushowMessage  = new Region[ atoi(strWinNum) ];

			//printf("winNumMsg->IpOffset = [%d]\n", winNumMsg->IpOffset);
			for (int index=0; index<atoi(strWinNum); index++)
			{
				char strFormatTemp[256];
				memset( strFormatTemp, 0, sizeof(strFormatTemp) );
				memset( temp, 0, sizeof(temp) );
				sprintf( strFormatTemp, "%sWIN%d", strFormat, index+1 );
				SCParamValueINI( strFormatTemp, temp ); // IP1WIN1 = IP1WIN1REGION1|IP1WIN1REGION2|IP1WIN1REGION3

				//printf("%s\n", strFormatTemp);

				char pszRegion1[256];
				char pszRegion2[256];
				char pszRegion3[256];

				memset( pszRegion1, 0, sizeof(pszRegion1) );
				memset( pszRegion2, 0, sizeof(pszRegion2) );
				memset( pszRegion3, 0, sizeof(pszRegion3) );
				
				SplitStrings::SplitStr( 0, temp, pszRegion1, '|' );
				SplitStrings::SplitStr( 1, temp, pszRegion2, '|' );
				SplitStrings::SplitStr( 2, temp, pszRegion3, '|' );

				char pszRegion1Info[256];
				char pszRegion2Info[256];
				char pszRegion3Info[256];
				memset( pszRegion1Info, 0, sizeof(pszRegion1Info) );
				memset( pszRegion2Info, 0, sizeof(pszRegion2Info) );
				memset( pszRegion3Info, 0, sizeof(pszRegion3Info) );
				SCParamValueINI( pszRegion1, pszRegion1Info );
				SCParamValueINI( pszRegion2, pszRegion2Info );
				SCParamValueINI( pszRegion3, pszRegion3Info );

				sscanf( pszRegion1Info, "%d %d %d %d %s %s %d",	\
					&(winNumMsg->struWinNum[index].x), &(winNumMsg->struWinNum[index].y), \
					&(winNumMsg->struWinNum[index].width), &(winNumMsg->struWinNum[index].height), \
					winNumMsg->struWinNum[index].showMsg, winNumMsg->struWinNum[index].fontName, \
					&(winNumMsg->struWinNum[index].fontSize) );

				sscanf( pszRegion2Info, "%d %d %d %d %s %s %d",	\
					&(winNumMsg->strutitleName[index].x), &(winNumMsg->strutitleName[index].y), \
					&(winNumMsg->strutitleName[index].width), &(winNumMsg->strutitleName[index].height), \
					winNumMsg->strutitleName[index].showMsg, winNumMsg->strutitleName[index].fontName, \
					&(winNumMsg->strutitleName[index].fontSize) );

				sscanf( pszRegion3Info, "%d %d %d %d %s %s %d",	\
					&(winNumMsg->strushowMessage[index].x), &(winNumMsg->strushowMessage[index].y), \
					&(winNumMsg->strushowMessage[index].width), &(winNumMsg->strushowMessage[index].height), \
					winNumMsg->strushowMessage[index].showMsg, winNumMsg->strushowMessage[index].fontName, \
					&(winNumMsg->strushowMessage[index].fontSize) );
			}

			// 插入map
			//mapIp.insert( _KV_(IpAddress, area) );
			mapWinAddr.insert( _KWinNumMsg_(IpAddress, winNumMsg) );
			//printf("插入一个\n");

		}

		/*
		// 查找数据
		map<string, WinNumMsg *>::iterator iter;
		//iter = mapIp.find("192.168.0.198");
		iter = mapWinAddr.find("140.24.45.13");
		//if ( iter != mapIp.end() )
		if ( iter != mapWinAddr.end() )
		{
			for (int i=0; i<8; i++)
			{
				printf( "WinNum   X:%d, Y:%d, Width:%d, Height:%d, ShowMsg:%s, FontName:%s, FontSize:%d\n",
					(iter->second)->struWinNum[i].x, (iter->second)->struWinNum[i].y, \
					(iter->second)->struWinNum[i].width, (iter->second)->struWinNum[i].height, \
					(iter->second)->struWinNum[i].showMsg, (iter->second)->struWinNum[i].fontName, \
					(iter->second)->struWinNum[i].fontSize );

				printf( "TitleName   X:%d, Y:%d, Width:%d, Height:%d, ShowMsg:%s, FontName:%s, FontSize:%d\n",
					(iter->second)->strutitleName[i].x, (iter->second)->strutitleName[i].y, \
					(iter->second)->strutitleName[i].width, (iter->second)->strutitleName[i].height, \
					(iter->second)->strutitleName[i].showMsg, (iter->second)->strutitleName[i].fontName, \
					(iter->second)->strutitleName[i].fontSize );

				printf( "ShowMsg   X:%d, Y:%d, Width:%d, Height:%d, ShowMsg:%s, FontName:%s, FontSize:%d\n",
					(iter->second)->strushowMessage[i].x, (iter->second)->strushowMessage[i].y, \
					(iter->second)->strushowMessage[i].width, (iter->second)->strushowMessage[i].height, \
					(iter->second)->strushowMessage[i].showMsg, (iter->second)->strushowMessage[i].fontName, \
					(iter->second)->strushowMessage[i].fontSize );
			}
		}
		else
		{
			cout << "Do not Find" << endl;
		}
		*/
	}

	void VisitMapsTree( )
	{
		

		for (int index=0; index<2; index++)
		{
			mapWinAddr[ searchMapIp[index+1] ]->IpOffset;
			mapWinAddr[ searchMapIp[index+1] ]->winNum;
			mapWinAddr[ searchMapIp[index+1] ]->winOffset;

			//printf("IpOffset = [%d]\n", mapWinAddr[ searchMapIp[index+1] ]->IpOffset);
			//printf("winNum = [%d]\n", mapWinAddr[ searchMapIp[index+1] ]->winNum);
			//printf("winOffset = [%d]\n", mapWinAddr[ searchMapIp[index+1] ]->winOffset);

			for ( int indexWin=0; indexWin < mapWinAddr[ searchMapIp[index+1] ]->winNum; indexWin++ ) 
			{
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).x;
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).y;
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).width;
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).height;
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).showMsg;
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).fontName;
				((mapWinAddr[ searchMapIp[index+1] ])->struWinNum[indexWin]).fontSize;

				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).x;
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).y;
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).width;
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).height;
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).showMsg;
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).fontName;
				((mapWinAddr[ searchMapIp[index+1] ])->strutitleName[indexWin]).fontSize;

				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).x;
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).y;
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).width;
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).height;
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).showMsg;
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).fontName;
				((mapWinAddr[ searchMapIp[index+1] ])->strushowMessage[indexWin]).fontSize;
			}
		}

		/*
		map<int, string>::iterator iter;
		for(int i=0; i<2; i++)
		{
			iter = searchMapIp.find(i+1);
			if ( iter != searchMapIp.end() )
			{
				printf( "%s\n", iter->second.c_str() );
			}
			else
			{
				printf( "not find value\n" );
			}
		}
		*/
	}
/************************************************************/
	// 自定义数据结构 end

	// 综合屏 上送显示结构体
	typedef struct IntegScreenSndInfo {
		long  addr;
	} IntegScreenSndInfo;

	// 窗口屏 默认显示结构体
	typedef struct StripScreenDefInfo {
		long  addr;
		char  defMessage[MAXLINE];
	} StripScreenDefInfo;

	// 综合屏 默认显示结构体
	typedef struct IntegScreenDefInfo {
		long  addr;
		char  defMessage[MAXLINE];
	} IntegScreenDefInfo;
}

using namespace IMMQDWZNM;

class IMMQDWZ : public ScreenAdapter
{
	Declare_DynCreate(IMMQDWZ)
public:
	IMMQDWZ(void);
public:
	virtual ~IMMQDWZ(void);

protected:
	// 串口通信接口，操作条屏，综合屏

	// 打开串口设备
	virtual int InitiDev		( );

	// 关闭串口设备
	virtual int CloseDev		( );

	// 向条屏发送信息
	virtual int SendStripScreen ( char *, char *, char *, char * );

	// 向综合屏发送信息
	virtual int SendIntegScreen ( char *, int, int, char * );

	// 条屏初始化显示
	virtual int InitStripScreen ( );

	// 综合屏初始化显示
	virtual int InitIntegScreen ( );

private: // 辅助函数
	int loadStripScreenInfo( );
	int loadIntegScreenInfo( );
	int LoadLibrarys( char *pathname );

private: // 发送IP一体屏辅助函数
	// 刷新IP一体屏
	void SendMessageToScream( int winAddr, int index);
	// 设置文字
	int  SetMessageToMap( int winAddr, char *sendBuffer );

private:
	Log		*IMMQDWZSlog;
	bool	m_initStrip; // 是否初始化窗口屏
	bool	m_initInteg; // 是否初始化综合屏
	int		m_stripDefNums; // 窗口屏数目
	StripScreenDefInfo stripScreenDefInfo[MAXLINE*2];
	int		m_integDefNums; // 综合屏数目
	IntegScreenDefInfo integScreenDefInfo[MAXLINE*2];
	int		m_integSndNums; // 综合屏数目
	IntegScreenSndInfo integScreenSndInfo[MAXLINE*2];
	char    showFormat[64]; // 综合屏显示格式


private: // 屏幕显示相关参数
	int		m_ComPort;
	int		m_ComBaund;
	int		m_StripAddr;
	int		m_ShowColor;
	int		m_ShowMode;
	int		m_ShowTime;
	int		m_ShowSpeed;
};
