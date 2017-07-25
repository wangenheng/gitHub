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
	int		X;				// 文字起始X坐标
	int		Y;				// 文字起始Y坐标
	int		Width;			// 文字所在区域宽
	int		Height;			// 文字所在区域高
	char    DisplayMsg[512]; // 默认显示信息
	char    FontName[32];	// 字体
	int		FontSize;		// 字体大小
	int		FontColor;		// 字体颜色
} HdcStyle;

// Region
typedef struct {
	int		X;			// 插播区域起始X坐标(8的倍数)
	int		Y;			// 插播区域起始Y坐标
	int		Width;		// 插播区域宽度点数 (8的倍数)
	int		Height;		// 插播区域高度点数
	int		PicWidth;	// 插播图片宽度点数 (8的倍数)
	int		PicHeight;	// 插播图片高度点数
	int		TimeOut;	// 超时插播
	int		Flag;		// 是否开启超时 默认不开启
} Region;

// Area
typedef struct {
	
	Region	   *region;		// 分区
	HdcStyle   *hdcStyle;	// 文字图片
	int			winNum;		// 窗口数量
	int			winOffset;	// 窗口偏移量
} Area;

// IP地址映射一体屏区域
typedef pair< string, Area * >	_KV_;
extern map<string, Area *> mapIp;
void InitMapIpAddr( );

// 窗口号映射IP地址
typedef pair< int, string >     _KIP_;
extern map<int, string> mapWinAddr;
void InitMapWinAdr( );

#endif // end __DATASTRUCT_H__