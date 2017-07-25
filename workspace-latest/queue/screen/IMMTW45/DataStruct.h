#ifndef __DATASTRUCT_H__
#define __DATASTRUCT_H__

#pragma warning( disable : 4786 )

#include <stdio.h>
#include <map>
#include <string>
#include <iostream>

#include "../../include/framework.h"

using namespace std;

// HdcStyle
typedef struct {
	int		X;				// ������ʼX����
	int		Y;				// ������ʼY����
	int		Width;			// �������������
	int		Height;			// �������������
	char    DisplayMsg[512]; // Ĭ����ʾ��Ϣ
	char    FontName[32];	// ����
	int		FontSize;		// �����С
	int		FontColor;		// ������ɫ
} HdcStyle;

// Region
typedef struct {
	int		X;			// �岥������ʼX����(8�ı���)
	int		Y;			// �岥������ʼY����
	int		Width;		// �岥�����ȵ��� (8�ı���)
	int		Height;		// �岥����߶ȵ���
	int		PicWidth;	// �岥ͼƬ��ȵ��� (8�ı���)
	int		PicHeight;	// �岥ͼƬ�߶ȵ���
	int		TimeOut;	// ��ʱ�岥
	int		Flag;		// �Ƿ�����ʱ Ĭ�ϲ�����
} Region;

// Area
typedef struct {
	
	Region	   *region;		// ����
	HdcStyle   *hdcStyle;	// ����ͼƬ
	int			winNum;		// ��������
	int			winOffset;	// ����ƫ����
} Area;

// IP��ַӳ��һ��������
typedef pair< string, Area * >	_KV_;
extern map<string, Area *> mapIp;
void InitMapIpAddr( );

// ���ں�ӳ��IP��ַ
typedef pair< int, string >     _KIP_;
extern map<int, string> mapWinAddr;
void InitMapWinAdr( );

#endif // end __DATASTRUCT_H__