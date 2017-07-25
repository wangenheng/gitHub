#include "VOS9004.h"
#include <time.h>
Implement_DynCreate(VOS9004,VoicesAdapter)

int VOS9004::VoiceDevPlaye( int readMode, char *custmerNum, int windowsNum, char *informatin )
{
	//std::cout << "无语音" << std::endl;
	//PlayVoices( custmerNum, windowsNum );
	char filePath[128];
	time_t t = time ( 0 ); 
	char tmp[64];
	string strt;
	string strd;
	strftime( tmp, sizeof(tmp), "%H:%M:%S",localtime(&t) );
	
	strt = tmp;
	strftime( tmp, sizeof(tmp), "%Y-%m-%d",localtime(&t) );
	sprintf(filePath,"log\\Voice(%s).txt",tmp);
	strd = tmp;


	
	//printf("%s\n",filePath);
	this->m_outputFile = new fstream;
	const char* c_s = filePath;	
	m_outputFile->open ( c_s,ofstream::out|ofstream::app ); //打开日志文件
	*m_outputFile<<"["<<strd<<" "<<strt<<"]    请，"<<custmerNum<<"号到"<<windowsNum<<"号窗口"<<endl;
	
	delete m_outputFile; 


	return 0;
}

//AddFile6.wav -> 请
//AddFile7.wav -> 号到
//AddFile8.wav -> 号窗口
void VOS9004::PlayVoices( char *custmerNum, int windowsNum )
{
	int  i = 0;
	int  rows = 0;
	char wavFileName[32];
	char wavFileArray[16][128];
	char strFormat[64];

	// 播报 -> 前奏
	memset( strFormat, 0, sizeof(strFormat) );
	memset( wavFileName, 0, sizeof(wavFileName) );
	sprintf( wavFileName, "lib\\wav\\AddFile1.wav" );
	PlaySound( wavFileName, NULL, SND_FILENAME|SND_ASYNC );
	Sleep( 1600 );

	// 播报 -> 请
	memset( wavFileName, 0, sizeof(wavFileName) );
	sprintf( wavFileName, "lib\\wav\\AddFile6.wav" );
	PlaySound( wavFileName, NULL, SND_FILENAME|SND_ASYNC );
	Sleep( 800 );

	// 播报 -> 客户号码
	initWavFileArray(wavFileArray, 16);
	fillReadCusNum(wavFileArray, custmerNum);
	for ( i=0; i<strlen(custmerNum); i++ ) {
		PlaySound( wavFileArray[i], NULL, SND_FILENAME|SND_ASYNC|SND_NOWAIT );
		Sleep( 600 );
	}

	// 播报 -> 号到
	memset( wavFileName, 0, sizeof(wavFileName) );
	sprintf( wavFileName, "lib\\wav\\AddFile7.wav" );
	PlaySound( wavFileName, NULL, SND_FILENAME|SND_ASYNC );
	Sleep( 700 );

	// 播报 -> 窗口
	initWavFileArray(wavFileArray, 16);
	fillReadWinNum( wavFileArray, windowsNum, &rows );
	for ( i=0; i<rows; i++ ) {
		PlaySound( wavFileArray[i], NULL, SND_FILENAME|SND_ASYNC );
		Sleep( 600 );
	}

	// 播报 -> 号窗口
	sprintf( wavFileName, "lib\\wav\\AddFile8.wav" ); 
	PlaySound( wavFileName, NULL, SND_FILENAME|SND_ASYNC );
	Sleep( 2000 );
}

void inline VOS9004::initWavFileArray(char arrayname[][128], int rows)
{
	for (int index=0; index<rows; index++) {
		memset(arrayname, 0, sizeof(arrayname[index]));
	}
}

// 预处理:阅读顾客编号
void inline VOS9004::fillReadCusNum( char arrayname[][128], char *custmerNum )
{
	int i;
	for (i=0; i<strlen(custmerNum); i++) {
		if (custmerNum[i]>='a'&&custmerNum[i]<='z' || custmerNum[i]>='A'&&custmerNum[i]<='Z')
			sprintf( arrayname[i], "lib\\\\wav\\\\%c.wav", custmerNum[i] );
		else if (custmerNum[i]>='0' && custmerNum[i]<='9')
			sprintf( arrayname[i], "lib\\\\wav\\\\%cH.wav", custmerNum[i] );
	}
}

//预处理:阅读窗口号
void inline VOS9004::fillReadWinNum(char ch_num[][128], int windowsNum, int *rows)
{
	int j=0;
	int index;
	int value = windowsNum;
	int flag;
	char ch_temp[128];

	memset(ch_temp, 0, sizeof(ch_temp));
	sprintf(ch_temp, "%d", value);
	index = strlen(ch_temp);
	flag  = index;
	
	for (int i=0; i<strlen(ch_temp); i++)
	{
		// 千位
		if (index == 4) 
		{
			sprintf(ch_num[j++], "lib\\\\wav\\\\%cH.wav", ch_temp[i]);
			sprintf(ch_num[j++], "lib\\\\wav\\\\1000H.wav");
			index--;
			// 1000
			if (ch_temp[i+1] == '0' && ch_temp[i+2] == '0' && ch_temp[i+3] == '0')
			{
				break;
			}
			// 1001 1002 1003 1004 1005 1006 1007 1008 1009
			else if (ch_temp[i+1] == '0' && ch_temp[i+2] == '0' && ch_temp[i+3] != '0')
			{
				sprintf(ch_num[j++], "lib\\\\wav\\\\0H.wav");
				sprintf(ch_num[j++], "lib\\\\wav\\\\%cH.wav", ch_temp[i+3]);
				break;
			}
			// 1010 1020 1030 1040 1050 1060 1070 1080 1090
			else if (ch_temp[i+1] == '0' && ch_temp[i+2] != '0')
			{
				sprintf(ch_num[j++], "lib\\\\wav\\\\0H.wav");
				//sprintf(ch_num[j++], "wav\\\\%c.wav", ch_temp[i+3]);
				i++;
				index--;
				flag=3;
			}
			// 1100
		}
		// 百位
		else if (index == 3) 
		{
			sprintf(ch_num[j++], "lib\\\\wav\\\\%cH.wav", ch_temp[i]);
			sprintf(ch_num[j++], "lib\\\\wav\\\\100H.wav");
			index--;
			// 100
			if (ch_temp[i+1] == '0' && ch_temp[i+2] == '0')
			{
				break;
			}
			// 101 102 103 104 105 106 107 108 109
			else if (ch_temp[i+1] == '0' && ch_temp[i+2] != '0')
			{
				sprintf(ch_num[j++], "lib\\\\wav\\\\0H.wav");
				sprintf(ch_num[j++], "lib\\\\wav\\\\%cH.wav", ch_temp[i+2]);
				break;
			}
			
		}
		// 十位
		else if (index == 2) 
		{
			if (flag == 3 || flag == 4) {
				sprintf(ch_num[j++], "lib\\\\wav\\\\%cH.wav", ch_temp[i]);
				// 110
				if (ch_temp[i] == '1' && ch_temp[i+1] == '0')
				{
					sprintf(ch_num[j++], "lib\\\\wav\\\\10H.wav");
					index--;
					break;
				}
				// 111 112 113 114 115 116 117 118 119
				else if (ch_temp[i] == '1' && ch_temp[i+1] != '0')
				{
					sprintf(ch_num[j++], "lib\\\\wav\\\\10H.wav");
					index--;
				}
				// 120 130 140 150 160 170 180 190
				else if (ch_temp[i] != '1' && ch_temp[i+1] == '0')
				{
					sprintf(ch_num[j++], "lib\\\\wav\\\\10H.wav");
					index--;
					break;
				}
				// 121~129 131~139 141~149 151~159 161~169 171~179 181~189 191~199
				else
				{
					sprintf(ch_num[j++], "lib\\\\wav\\\\10H.wav");
					index--;
				}

			}
			if (flag == 2) {
				sprintf(ch_num[j++], "lib\\\\wav\\\\%cH.wav", ch_temp[i]);
				// 10
				if (ch_temp[i] == '1' && ch_temp[i+1] == '0') 
				{ 
					sprintf(ch_num[j-1], "lib\\\\wav\\\\10H.wav");
					index--;
					break;
				} 
				// 11 12 13 14 15 16 17 18 19
				else if (ch_temp[i] == '1' && ch_temp[i+1] != '0') 
				{ 
					sprintf(ch_num[j-1], "lib\\\\wav\\\\10H.wav");
					index--;
				} 
				// 20 30 40 50 60 70 80 90
				else if (ch_temp[i+1] == '0') 
				{ 
					sprintf(ch_num[j++], "lib\\\\wav\\\\10H.wav");
					index--;
					break;
				} 
				// 21~29 31~39 41~49 51~59 61~69 71~79 81~89 91~99
				else 
				{ 
					sprintf(ch_num[j++], "lib\\\\wav\\\\10H.wav");
					index--;
				}
			}
		}
		// 个位
		else if (index == 1) // 1 2 3 4 5 6 7 8 9
		{ 
			sprintf(ch_num[j++], "lib\\\\wav\\\\%cH.wav", ch_temp[i]);
		}
	}

	*rows = j;
}
