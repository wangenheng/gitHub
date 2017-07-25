#ifndef	SerialFun_H
#define SerialFun_H

//#include <afx.h>
#include <Windows.h>

#include <string.h>
#include "LogFile.h"
#include "cfgfile.h"
#include "PortFun.h"

typedef HANDLE (__stdcall *LpOpenPort) (char *, long);
typedef int    (__stdcall *LpClosePort)(HANDLE);
typedef int    (__stdcall *LpDataSend) (HANDLE, char *, int, int, int, int, char *, int);

//static var
static  HANDLE       m_handle;
static	LpOpenPort   OpenPort;
static	LpClosePort  ClosePort;
static	LpDataSend   DataSend;
	
static LogFile m_seriallog;
//export Function
extern "C" long __declspec(dllexport) Serial_InitialDev( int ch_port, int baund );
extern "C" long __declspec(dllexport) Serial_CloseDev( );
extern "C" long __declspec(dllexport) Serial_SetOperatorLog( char *ch_logname );
extern "C" long __declspec(dllexport) Serial_SendTP( char *ch_data, int addr, char *showmode, char *ch_other );
extern "C" long __declspec(dllexport) Serial_SendZH( char *ch_data, int addr, int line, char *ch_other );
extern "C" long __declspec(dllexport) Serial_InitialTpShow( );
extern "C" long __declspec(dllexport) Serial_InitialZhShow( );
extern "C" long __declspec(dllexport) Serial_GetDevStatus( int mode, int dev_addr );

//inter function
long   LoadOtherLibrary( char *path );
void   slipchar( int count, char *INCHAR, char *OUTCHAR, char ch );
int    Slipcharcount( char *INCHAR, char ch );
static UINT TimeCount( LPVOID lpParam );


long InitialZhData( );
long UpdateZhData( char * ch );
//static globe var
//inter struct
//static HANDLE m_handle=NULL;
static MyPort m_comm;
static char ch_zh[4][30];
int    result;

#endif