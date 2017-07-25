#include "EQ2008_DataStruct.h"

#define SCParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM90EQ.ini")->GetParamValue(key, valueBuffer)
#define SSParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\IMM90EQ.ini")->GetParamValue(key, valueBuffer)

map<int, WinRegion *> mapWinRegion;
map<int, int>    mapWinAdr;
map<int, User_FontSet*> mapFont;

char *cltrimEQ( char *str )
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

char *crtrimEQ( char *str )
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

void InitMapWinAddr( )
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
		char strDspIpAdr[8];
		//int  CardNum;

		memset( strFormat  , 0, sizeof(strFormat)   );
		memset( strValues  , 0, sizeof(strValues)   );
		memset( strWinNums , 0, sizeof(strWinNums)  );
		memset( strDspIpAdr, 0, sizeof(strDspIpAdr) );

		sprintf( strFormat, "WINADDRTOIP%d", i+1 );
		SSParamValueINI( strFormat, strValues );

		//printf("strValues = [%s]\n", strValues );

		SplitStrings::SplitStr( 0, strValues, strWinNums, '-' );
		SplitStrings::SplitStr( 1, strValues, strDspIpAdr, '-' );

		cltrimEQ(strWinNums);
		crtrimEQ(strWinNums);
		cltrimEQ(strDspIpAdr);
		crtrimEQ(strDspIpAdr);
		
		int count = SplitStrings::SplitCharCount( strWinNums, ',');
		//printf( "count = [%d]\n", count );
		for (int index=0; index<=count; index++)
		{
			char t[32];
			memset( t, 0, sizeof(t) );
			SplitStrings::SplitStr( index, strWinNums, t, ',' );
			// 插入map
			mapWinAdr.insert(_KII_(atoi(t), atoi(strDspIpAdr)));
		}
	}

	// 遍历数据
//	map<int, string>::iterator iter;
	//printf( "%s\n", mapWinAddr[1].c_str() );
	
//	for(i=1;i<(mapWinAdr.size())+1;i++)
//	{
//		cout<<i<<"的地址:"<<mapWinAdr[i]<<endl;
//	}
}


void InitMapWinRegion()
{
	char temp[256];
	char Values[256];
	char strFormat[256];
	memset( Values,    0, sizeof(Values) );
	
	
	SSParamValueINI( "CKNUMS", Values );

	for (int i = 1; i<atoi(Values)+1; i++)
	{
		WinRegion * region = new WinRegion;

		memset( strFormat, 0, sizeof(strFormat) );
		memset( temp,      0, sizeof(temp) );
		memset( region->dispMsg,0,sizeof(region->dispMsg));
		sprintf(strFormat, "WIN%d",i);
		SCParamValueINI(strFormat,temp);
		
		sscanf( temp, "%d %d %d %d %d %d %d %d %d %d %d %d %s ", \
			&region->numX, &region->numY, &region->numWidth, &region->numHeight,\
			&region->busnsX, &region->busnsY, &region->busnsWidth, &region->busnsHeight,\
			&region->callX, &region->callY, &region->callWidth, &region->callHeight, region->dispMsg);
		
		mapWinRegion.insert(_KIR_(i,region));
	}

	//遍历map
	/*
	for (i=1;i<=mapWinRegion.size();i++)
	{
		cout<<i<<":"<<mapWinRegion[i]->numX<<" "<<mapWinRegion[i]->numY<<" "<<mapWinRegion[i]->numWidth<<" "<<mapWinRegion[i]->numHeight<<" "<<\
			mapWinRegion[i]->busnsX<<" "<<mapWinRegion[i]->busnxY<<" "<<mapWinRegion[i]->busnsWidth<<" "<<mapWinRegion[i]->busnsHeight<<" "<<\
			mapWinRegion[i]->callX<<" "<<mapWinRegion[i]->callY<<" "<<mapWinRegion[i]->callWidth<<" "<<mapWinRegion[i]->callHeight<<" "<<mapWinRegion[i]->dispMsg<<endl;
	}
*/	
}

void InitMapFont()
{
	char fontName[128];
	char temp[256];
	User_FontSet * font1 = new User_FontSet;
	User_FontSet * font2 = new User_FontSet;
	User_FontSet * font3 = new User_FontSet;
	memset(fontName,0,sizeof(fontName));
	memset( temp, 0, sizeof(temp) );
	SCParamValueINI( "FONTNUM", temp );

	//cout<<temp<<endl;

	sscanf( temp, "%s %d %d %d %d %d %d %d %d ",\
			fontName, &font1->iFontSize, &font1->bFontBold,\
			&font1->bFontItaic, &font1->bFontUnderline, &font1->colorFont,\
			&font1->iAlignStyle,&font1->iVAlignerStyle,&font1->iRowSpace);
	font1->strFontName = (char *)fontName;
	mapFont.insert(_KSF_(1,font1));


	memset(fontName,0,sizeof(fontName));
	memset( temp, 0, sizeof(temp) );
	SCParamValueINI( "FONTBUSNS", temp );

	sscanf( temp, "%s %d %d %d %d %d %d %d %d ",\
			fontName, &font2->iFontSize, &font2->bFontBold,\
			&font2->bFontItaic, &font2->bFontUnderline, &font2->colorFont,\
			&font2->iAlignStyle,&font2->iVAlignerStyle,&font2->iRowSpace);
	font2->strFontName = (char *)fontName;
	mapFont.insert(_KSF_(2,font2));

	memset(fontName,0,sizeof(fontName));
	memset( temp, 0, sizeof(temp) );
	SCParamValueINI( "FONTCALL", temp );

	sscanf( temp, "%s %d %d %d %d %d %d %d %d ",\
			fontName, &font3->iFontSize, &font3->bFontBold,\
			&font3->bFontItaic, &font3->bFontUnderline, &font3->colorFont,\
			&font3->iAlignStyle,&font3->iVAlignerStyle,&font3->iRowSpace);
	font3->strFontName = (char *)fontName;
	mapFont.insert(_KSF_(3,font3));

	//遍历map
	for(int i = 1;i<4;i++)
	{
	
		cout<<mapFont[i]->strFontName<<"  "<< mapFont[i]->iFontSize<<"  "<< mapFont[i]->bFontBold<<"  "<<\
			mapFont[i]->bFontItaic<<"  "<< mapFont[i]->bFontUnderline<<"  "<< mapFont[i]->colorFont<<"  "<<\
			mapFont[i]->iAlignStyle<<"  "<<mapFont[i]->iVAlignerStyle<<"  "<<mapFont[i]->iRowSpace<<endl;
	}

	cout<<mapFont[1]->strFontName<<endl;
}