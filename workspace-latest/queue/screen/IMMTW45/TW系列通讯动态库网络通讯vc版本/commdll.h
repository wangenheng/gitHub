#include<windows.h>
int  (_stdcall*NetShut)(int,char*,int);  
int  (_stdcall*NetPicSendData)(int,char*,int,byte[],long,int,int,int,int);
int  (_stdcall*NetListSend)(int ,char*,byte[],long,int);
int  (_stdcall*NetPicSendOnce)(int,char*,byte[],long,int,int,int,int,int,int,int);
int  (_stdcall*GetArrayFromPic)(HDC,byte[],long,int,int,int);
int  (_stdcall*NetSendPicOnce)(int,char*,HDC,int,int,int,int,int,int,int);
int  (_stdcall*NetSendPicData)(int,char*,int,HDC,int,int,int,int);
long (_stdcall*SocketOpen)(int);
int  (_stdcall*SocketClose)(long);
int  (_stdcall*SendData)(long,char*,long,byte[],long,int);
int  (_stdcall*funDataSend)(int,char*,int,byte[],long);
int  (_stdcall*ReadData)(long,byte[],long);
int  (_stdcall*DrawRectString)(HDC,char*,char*,int,int,int,int,int,int);
HDC  (_stdcall*GDCCreate)(int,int);
int  (_stdcall*GDCFree)(HDC);

HINSTANCE hInstance;
int test_Initialize(void)
{
  if ((hInstance=LoadLibrary("EV_SocketUdpDll.dll"))!=NULL)
  {
      NetShut=(int (_stdcall *)(int,char*,int))GetProcAddress(hInstance,"NetShut");
      NetPicSendData=(int (_stdcall *)(int,char*,int,byte[],long,int,int,int,int))GetProcAddress(hInstance,"NetPicSendData");
      NetListSend=(int (_stdcall *)(int ,char*,byte[],long,int))GetProcAddress(hInstance,"NetListSend");
      NetPicSendOnce=(int (_stdcall *)(int,char*,byte[],long,int,int,int,int,int,int,int))GetProcAddress(hInstance,"NetPicSendOnce");
      GetArrayFromPic=(int (_stdcall *)(HDC,byte[],long,int,int,int))GetProcAddress(hInstance,"GetArrayFromPic");
      NetSendPicOnce=(int (_stdcall *)(int,char*,HDC,int,int,int,int,int,int,int))GetProcAddress(hInstance,"NetSendPicOnce");
      NetSendPicData=(int (_stdcall *)(int,char*,int,HDC,int,int,int,int))GetProcAddress(hInstance,"NetSendPicData");
      SocketOpen=(long (_stdcall *)(int))GetProcAddress(hInstance,"SocketOpen");
      SocketClose=(int (_stdcall *)(long))GetProcAddress(hInstance,"SocketClose");
      SendData=(int (_stdcall *)(long,char*,long,byte[],long,int))GetProcAddress(hInstance,"SendData");
      funDataSend=(int (_stdcall *)(int,char*,int,byte[],long))GetProcAddress(hInstance,"funDataSend");
      ReadData=(int (_stdcall *)(long,byte[],long))GetProcAddress(hInstance,"ReadData");
      DrawRectString=(int (_stdcall *)(HDC,char*,char*,int,int,int,int,int,int))GetProcAddress(hInstance,"DrawRectString");
      GDCCreate=(HDC (_stdcall *)(int,int))GetProcAddress(hInstance,"GDCCreate");
      GDCFree=(int (_stdcall *)(HDC))GetProcAddress(hInstance,"GDCFree");
  }
  return 0;
}