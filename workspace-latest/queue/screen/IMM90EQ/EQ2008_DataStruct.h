#ifndef _DATASTRUCTURE_H_
#define _DATASTRUCTURE_H_

#pragma warning( disable : 4786 )

#include <stdio.h>
#include <map>
#include <string>
#include <iostream>

#include "../../include/framework.h"

using namespace std;

typedef struct						//��������
{
	char*		strFontName;		//���������
	int			iFontSize;			//����Ĵ�С
	BOOL		bFontBold;			//�����Ƿ�Ӵ�
	BOOL		bFontItaic;			//�����Ƿ���б��
	BOOL		bFontUnderline;		//�����Ƿ���»���
	COLORREF	colorFont;			//�������ɫ
	int			iAlignStyle;		//���뷽ʽ
									//0�� ����� 
									//1������ 
									//2���Ҷ���
	int         iVAlignerStyle;		//���¶��뷽ʽ
									//0-������
									//1-���¾���
									//2-�׶���
	int         iRowSpace;			//�м��
}User_FontSet;

typedef struct						//��������
{
	int			iX;					//���ڵ����X
	int			iY;					//���ڵ����Y
	int			iWidth;				//����Ŀ��
	int			iHeight;			//����ĸ߶�
	int			iFrameMode;			//�߿����ʽ
	COLORREF	FrameColor;			//�߿���ɫ
}User_PartInfo;

typedef struct _User_MoveSet
{
	int			iActionType;		//��Ŀ�任��ʽ
	int			iActionSpeed;		//��Ŀ�Ĳ����ٶ�	
	BOOL		bClear;				//�Ƿ���Ҫ�������
	int			iHoldTime;			//����Ļ��ͣ����ʱ��
	int         iClearSpeed;		//�����ʾ�����ٶ�
	int			iClearActionType;	//��Ŀ����ı任��ʽ
	int         iFrameTime;
}User_MoveSet;

//����ʱ���ڡ� 
typedef struct _User_Timer
{
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	DWORD			ReachTimeYear;		//������
	DWORD			ReachTimeMonth;		//������
	DWORD			ReachTimeDay;		//������
	DWORD			ReachTimeHour;		//����ʱ
	DWORD			ReachTimeMinute;	//�����
	DWORD			ReachTimeSecond;	//������
	BOOL			bDay;				//�Ƿ���ʾ�� 0������ʾ 1����ʾ
	BOOL			bHour;				//�Ƿ���ʾСʱ
	BOOL			bMin;				//�Ƿ���ʾ����
	BOOL			bSec;				//�Ƿ���ʾ��
	BOOL			bMulOrSingleLine;	//���л��Ƕ���
	char			*chTitle;			//�����ʾ����
}User_Timer;


//���¶ȴ��ڡ�
typedef struct _User_Temperature
{
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	char			*chTitle;			//����
	int				DisplayType;		//��ʾ��ʽ��0���� 1��C
}User_Temperature;

//������ʱ�䴰�ڡ� 
typedef struct _User_DateTime
{
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	int				iDisplayType;		//��ʾ���
	char			*chTitle;			//�����ʾ����
	BOOL			bYearDisType;		//���λ��0 ��4��1��2λ
	BOOL			bMulOrSingleLine;	//���л��Ƕ���
	BOOL			bYear;
	BOOL			bMouth;
	BOOL			bDay;
	BOOL			bWeek;
	BOOL			bHour;
	BOOL			bMin;
	BOOL			bSec;
}User_DateTime;

//��ͼ�Ŀ� 
typedef struct _User_Bmp
{
	User_PartInfo	PartInfo;			 //������Ϣ
}User_Bmp;

//�������ı���
typedef struct _User_SingleText
{
	char			*chContent;         //��ʾ����
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	User_MoveSet	MoveSet;			//������ʽ����
}User_SingleText;

//���ı��� 
typedef struct _User_Text
{
	char			*chContent;         //��ʾ����
	User_PartInfo	PartInfo;			//������Ϣ
	COLORREF		BkColor;			//������ɫ
	User_FontSet	FontInfo;			//��������
	User_MoveSet	MoveSet;			//������ʽ����
}User_Text;

//��RTF�ļ��� 
typedef struct _User_RTF
{
	char            *strFileName;       //RTF�ļ���
	User_PartInfo	PartInfo;			//������Ϣ
	User_MoveSet    MoveSet;			//������ʽ����
}User_RTF;

//����
typedef struct{
	int numX;
	int numY;
	int numWidth;
	int numHeight;
	int busnsX;
	int busnsY;
	int busnsWidth;
	int busnsHeight;
	int callX;
	int callY;
	int callWidth;
	int callHeight;
	char dispMsg[256];
}WinRegion;
typedef pair< int, WinRegion * >	_KIR_;
extern map<int, WinRegion *> mapWinRegion;
void InitMapWinRegion( );

// ���ں�ӳ���ַ
typedef pair< int, int >     _KII_;
extern map<int, int> mapWinAdr;
void InitMapWinAddr( );
//����ӳ��
typedef pair< int, User_FontSet* >     _KSF_;
extern map<int, User_FontSet*> mapFont;
void InitMapFont();

#endif _DATASTRUCTURE_H_
