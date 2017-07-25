#include "DataStruct.h"

#define SSParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMMTW45.ini")->GetParamValue(key, valueBuffer)
#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMMTW45.ini")->GetParamValue(key, valueBuffer)

map<string, Area *> mapIp;
map<int, string>    mapWinAddr;

char *cltrim( char *str )
{
    char    *pstart, *ptr;
	
    if ( str == NULL )
        return NULL;

    pstart = ptr = str;

    while ( *( ( unsigned char * )ptr ) == 0x20 || *ptr == '\t' || \
            *ptr == '\r' || *ptr == '\n' || *ptr == '[' || *ptr == '>' )
        ptr ++;

    while ( *ptr != 0x0 )
        *str ++ = *ptr ++;

    *str = 0x0;

    return pstart;
}

char *crtrim( char *str )
{
    char    *ptr;

    if ( str == NULL )
        return NULL;

    ptr = str + strlen( str ) - 1;

    while ( ptr >= str && ( *( ( unsigned char * )ptr ) == 0x20 || \
            *ptr == '\t' ||  *ptr == '\r' || *ptr == '\n' || *ptr == ']' ))
        *ptr -- = 0x0;

    return str;
}

void InitMapWinAdr( )
{
	char winNums[8];
	
	memset( winNums, 0, sizeof(winNums) );
	// "WINAPPMAPNUM"
	//FWParamValueINI( "WINAPPMAPNUM", winNums );
	SCParamValueINI( "WINAPPMAPNUM", winNums );
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
		SSParamValueINI( strFormat, strValues );

		//printf("strValues = [%s]\n", strValues );

		SplitStrings::SplitStr( 0, strValues, strWinNums, '-' );
		SplitStrings::SplitStr( 1, strValues, strDspIpAdr, '-' );

		cltrim(strWinNums);
		crtrim(strWinNums);
		cltrim(strDspIpAdr);
		crtrim(strDspIpAdr);

		int count = SplitStrings::SplitCharCount( strWinNums, ',');
		//printf( "count = [%d]\n", count );
		for (int index=0; index<=count; index++)
		{
			char t[32];
			memset( t, 0, sizeof(t) );
			SplitStrings::SplitStr( index, strWinNums, t, ',' );

			// 插入map
			mapWinAddr.insert(_KIP_(atoi(t), strDspIpAdr));
		}
	}

	// 查找数据
	map<int, string>::iterator iter;
	//printf( "%s\n", mapWinAddr[1].c_str() );
}

void InitMapIpAddr( )
{
	char temp[256];
	char Values[256];
	char strFormat[256];
	memset( Values, 0, sizeof(Values) );
	SSParamValueINI( "IPNUM", Values );
	//printf( "Values = [%s]\n", Values );

	for (int i=0; i<atoi(Values); i++)
	{
		// 1. 第一级 map, 即: map索引
		Area *area = new Area;

		// 2. 第二级 map, 窗口map
		memset( temp, 0, sizeof(temp) );
		memset( strFormat, 0, sizeof(strFormat) );
		sprintf( strFormat, "IP%d", i+1 );
		SSParamValueINI( strFormat, temp );
		//printf( "temp = [%s]\n", temp );

		char IpAddress[256];
		char strWinNum[8];
		char strWinOffset[8];

		memset( IpAddress   , 0, sizeof(IpAddress)    );
		memset( strWinNum   , 0, sizeof(strWinNum)    );
		memset( strWinOffset, 0, sizeof(strWinOffset) );

		SplitStrings::SplitStr( 0, temp, IpAddress   , '|' );
		SplitStrings::SplitStr( 1, temp, strWinNum   , '|' );
		SplitStrings::SplitStr( 2, temp, strWinOffset, '|' );

		area->winNum    = atoi( strWinNum );
		area->winOffset = atoi( strWinOffset );
		area->region    = new Region  [ atoi(strWinNum)*2 ];
		area->hdcStyle  = new HdcStyle[ atoi(strWinNum)*2 ];
		for (int index=0; index<atoi(strWinNum)*2; index++)
		{
			char strFormatTemp[256];
			memset( strFormatTemp, 0, sizeof(strFormatTemp) );
			memset( temp, 0, sizeof(temp) );
			sprintf( strFormatTemp, "%sWIN%d", strFormat, index+1 );
			//printf( "strFormatTemp = [%s]\n", strFormatTemp );
			SSParamValueINI( strFormatTemp, temp );
			//printf( "temp = [%s]\n", temp );

			char pszHDC[256];
			char pszRegin[256];
			memset( pszHDC  , 0, sizeof(pszHDC)   );
			memset( pszRegin, 0, sizeof(pszRegin) );
			SplitStrings::SplitStr( 0, temp, pszHDC  , '|' );
			SplitStrings::SplitStr( 1, temp, pszRegin, '|' );

			//printf( "pszHDC   = [%s]\n", pszHDC );
			//printf( "pszRegin = [%s]\n", pszRegin );
			char pszHDCInfo[256];
			char pszReginInfo[256];
			memset( pszHDCInfo, 0, sizeof(pszHDCInfo) );
			memset( pszReginInfo, 0, sizeof(pszReginInfo) );
			SSParamValueINI( pszHDC, pszHDCInfo );
			//printf( "[%s] = [%s]\n", pszHDC, pszHDCInfo );

			SSParamValueINI( pszRegin, pszReginInfo );
			//printf( "[%s] = [%s]\n", pszRegin, temps );

			sscanf( pszHDCInfo, "%d %d %d %d %s %s %d %d",	\
							&(area->hdcStyle[index].X), &(area->hdcStyle[index].Y), \
							&(area->hdcStyle[index].Width), &(area->hdcStyle[index].Height), \
							area->hdcStyle[index].DisplayMsg, area->hdcStyle[index].FontName, \
							&(area->hdcStyle[index].FontSize), &(area->hdcStyle[index].FontColor) );
			sscanf( pszReginInfo, "%d %d %d %d %d %d %d %d",	\
							&(area->region[index].X), &(area->region[index].Y), \
							&(area->region[index].Width), &(area->region[index].Height), \
							&(area->region[index].PicWidth), &(area->region[index].PicHeight),
							&(area->region[index].TimeOut), &(area->region[index].Flag) );
		}

		// 插入map
		mapIp.insert( _KV_(IpAddress, area) );
	}

	// 查找数据
	map<string, Area *>::iterator iter;
	iter = mapIp.find("192.168.15.109");
	if ( iter != mapIp.end() )
	{
		for (int i=0; i<2; i++)
		{
			
			printf( "Region   X:%d, Y:%d, Width:%d, Height:%d, PicWidth:%d, PicHeight:%d\n",
				(iter->second)->region[i].X, (iter->second)->region[i].Y, \
				(iter->second)->region[i].Width, (iter->second)->region[i].Height, \
				(iter->second)->region[i].PicWidth, (iter->second)->region[i].PicHeight );

			printf( "hdcStyle X:%d, Y:%d, Width:%d, Height:%d, DisplayMsg:%s, FontName:%s\nFontSize:%d, FontColor:%d\n", \
				(iter->second)->hdcStyle[i].X, (iter->second)->hdcStyle[i].Y, \
				(iter->second)->hdcStyle[i].Width, (iter->second)->hdcStyle[i].Height, \
				(iter->second)->hdcStyle[i].DisplayMsg, (iter->second)->hdcStyle[i].FontName, \
				(iter->second)->hdcStyle[i].FontSize, (iter->second)->hdcStyle[i].FontColor );
			
		}
	}
	else
	{
		cout << "Do not Find" << endl;
	}
}