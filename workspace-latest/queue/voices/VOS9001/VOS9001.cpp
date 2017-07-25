#include "VOS9001.h"

Implement_DynCreate(VOS9001, VoicesAdapter)

int VOS9001::VoiceDevPlaye( int readMode, char *custmerNum, int windowsNum, char *informatin )
{
	PlayVoices( custmerNum, windowsNum );

	return 0;
}

// custmerNum:A001
// windowsNum:2
void VOS9001::PlayVoices( char *custmerNum, int windowsNum )
{
	int  i = 0;
	int  rows = 0;
	char wavFileName[32];
	char wavFileArray[16][128];

	// 播报 -> 前奏
	memset( wavFileName, 0, sizeof(wavFileName) );
	sprintf( wavFileName, "lib\\wav\\AddFile1.wav" );
//	PlaySound( wavFileName, NULL, SND_FILENAME|SND_ASYNC );
//	Sleep( 1600 );

	// 播报 -> 请
	memset( wavFileName, 0, sizeof(wavFileName) );
	sprintf( wavFileName, "lib\\wav\\AddFile2.wav" );
	PlaySound( wavFileName, NULL, SND_FILENAME|SND_ASYNC );
	Sleep( 700 );

	// 播报 -> 客户号码
	initWavFileArray(wavFileArray, 16);
	for ( i=0; i<strlen(custmerNum); i++ ) {
		sprintf( wavFileArray[i], "lib\\\\wav\\\\%c.wav", custmerNum[i] );
	}
	for ( i=0; i<strlen(custmerNum); i++ ) {
		PlaySound( wavFileArray[i], NULL, SND_FILENAME|SND_ASYNC|SND_NOWAIT );
		Sleep( 500 );
	}

	// 播报 -> 号到
	memset( wavFileName, 0, sizeof(wavFileName) );
	sprintf( wavFileName, "lib\\wav\\AddFile3.wav" );
	PlaySound( wavFileName, NULL, SND_FILENAME|SND_ASYNC );
	Sleep( 600 );

	// 播报 -> 窗口
	initWavFileArray(wavFileArray, 16);
	fillReadWinNum( wavFileArray, windowsNum, &rows );
	for ( i=0; i<rows; i++ ) {
		PlaySound( wavFileArray[i], NULL, SND_FILENAME|SND_ASYNC );
		Sleep( 500 );
	}

	// 播报 -> 号窗口
	sprintf( wavFileName, "lib\\wav\\AddFile4.wav" ); 
	PlaySound( wavFileName, NULL, SND_FILENAME|SND_ASYNC );
	Sleep( 2000 );
}

void inline VOS9001::initWavFileArray(char arrayname[][128], int rows)
{
	for (int index=0; index<rows; index++) {
		memset(arrayname, 0, sizeof(arrayname[index]));
	}
}

void inline VOS9001::fillReadWinNum( char ch_num[][128], int windowsNum, int *rows )
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
			sprintf(ch_num[j++], "lib\\\\wav\\\\%c.wav", ch_temp[i]);
			sprintf(ch_num[j++], "lib\\\\wav\\\\1000.wav");
			index--;
			// 1000
			if (ch_temp[i+1] == '0' && ch_temp[i+2] == '0' && ch_temp[i+3] == '0')
			{
				break;
			}
			// 1001 1002 1003 1004 1005 1006 1007 1008 1009
			else if (ch_temp[i+1] == '0' && ch_temp[i+2] == '0' && ch_temp[i+3] != '0')
			{
				sprintf(ch_num[j++], "lib\\\\wav\\\\0.wav");
				sprintf(ch_num[j++], "lib\\\\wav\\\\%c.wav", ch_temp[i+3]);
				break;
			}
			// 1010 1020 1030 1040 1050 1060 1070 1080 1090
			else if (ch_temp[i+1] == '0' && ch_temp[i+2] != '0')
			{
				sprintf(ch_num[j++], "lib\\\\wav\\\\0.wav");
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
			sprintf(ch_num[j++], "lib\\\\wav\\\\%c.wav", ch_temp[i]);
			sprintf(ch_num[j++], "lib\\\\wav\\\\100.wav");
			index--;
			// 100
			if (ch_temp[i+1] == '0' && ch_temp[i+2] == '0')
			{
				break;
			}
			// 101 102 103 104 105 106 107 108 109
			else if (ch_temp[i+1] == '0' && ch_temp[i+2] != '0')
			{
				sprintf(ch_num[j++], "lib\\\\wav\\\\0.wav");
				sprintf(ch_num[j++], "lib\\\\wav\\\\%c.wav", ch_temp[i+2]);
				break;
			}
			
		}
		// 十位
		else if (index == 2) 
		{
			if (flag == 3 || flag == 4) {
				sprintf(ch_num[j++], "lib\\\\wav\\\\%c.wav", ch_temp[i]);
				// 110
				if (ch_temp[i] == '1' && ch_temp[i+1] == '0')
				{
					sprintf(ch_num[j++], "lib\\\\wav\\\\10.wav");
					index--;
					break;
				}
				// 111 112 113 114 115 116 117 118 119
				else if (ch_temp[i] == '1' && ch_temp[i+1] != '0')
				{
					sprintf(ch_num[j++], "lib\\\\wav\\\\10.wav");
					index--;
				}
				// 120 130 140 150 160 170 180 190
				else if (ch_temp[i] != '1' && ch_temp[i+1] == '0')
				{
					sprintf(ch_num[j++], "lib\\\\wav\\\\10.wav");
					index--;
					break;
				}
				// 121~129 131~139 141~149 151~159 161~169 171~179 181~189 191~199
				else
				{
					sprintf(ch_num[j++], "lib\\\\wav\\\\10.wav");
					index--;
				}

			}
			if (flag == 2) {
				sprintf(ch_num[j++], "lib\\\\wav\\\\%c.wav", ch_temp[i]);
				// 10
				if (ch_temp[i] == '1' && ch_temp[i+1] == '0') 
				{ 
					sprintf(ch_num[j-1], "lib\\\\wav\\\\10.wav");
					index--;
					break;
				} 
				// 11 12 13 14 15 16 17 18 19
				else if (ch_temp[i] == '1' && ch_temp[i+1] != '0') 
				{ 
					sprintf(ch_num[j-1], "lib\\\\wav\\\\10.wav");
					index--;
				} 
				// 20 30 40 50 60 70 80 90
				else if (ch_temp[i+1] == '0') 
				{ 
					sprintf(ch_num[j++], "lib\\\\wav\\\\10.wav");
					index--;
					break;
				} 
				// 21~29 31~39 41~49 51~59 61~69 71~79 81~89 91~99
				else 
				{ 
					sprintf(ch_num[j++], "lib\\\\wav\\\\10.wav");
					index--;
				}
			}
		}
		// 个位
		else if (index == 1) // 1 2 3 4 5 6 7 8 9
		{ 
			sprintf(ch_num[j++], "lib\\\\wav\\\\%c.wav", ch_temp[i]);
		}
	}

	*rows = j;
}
