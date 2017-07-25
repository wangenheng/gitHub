#include<windows.h>

int  (_stdcall*ComShut)(int,long,int,int);
int  (_stdcall*ComSetAutoOpen)(int,long,int,int,int,int,int,int);
int  (_stdcall*ComCheckDateTime)(int,long,int,int);
int  (_stdcall*ComSetLightness)(int,long,int,int);
int  (_stdcall*ComSendList)(int,long,int,byte[],long,int);
int  (_stdcall*ComSendPicData)(int,long,int,int,HDC,int,int,int,int);
int  (_stdcall*ComSetCom)(int,long,int,int,long);

int  (_stdcall*DrawRectString)(HDC,char*,char*,int,int,int,int,int,int);
HDC  (_stdcall*GDCCreate)(int,int);
int  (_stdcall*GDCFree)(HDC);

HINSTANCE hInstance;
int test_Initialize(void)
{
  if ((hInstance=LoadLibrary("EV_SocketUdpDll.dll"))!=NULL)
  {
      ComShut=(int (_stdcall *)(int,long,int,int))GetProcAddress(hInstance,"ComShut");
	  ComSetAutoOpen=(int (_stdcall *)(int,long,int,int,int,int,int,int))GetProcAddress(hInstance,"ComSetAutoOpen");
	  ComCheckDateTime=(int (_stdcall *)(int,long,int,int))GetProcAddress(hInstance,"ComCheckDateTime");
	  ComSetLightness=(int (_stdcall *)(int,long,int,int))GetProcAddress(hInstance,"ComSetLightness");
	  ComSendList=(int (_stdcall *)(int,long,int,byte[],long,int))GetProcAddress(hInstance,"ComSendList");
	  ComSendPicData=(int (_stdcall *)(int,long,int,int,HDC,int,int,int,int))GetProcAddress(hInstance,"ComSendPicData");
	  ComSetCom=(int (_stdcall *)(int,long,int,int,long))GetProcAddress(hInstance,"ComSetCom");


      DrawRectString=(int (_stdcall *)(HDC,char*,char*,int,int,int,int,int,int))GetProcAddress(hInstance,"DrawRectString");
      GDCCreate=(HDC (_stdcall *)(int,int))GetProcAddress(hInstance,"GDCCreate");
      GDCFree=(int (_stdcall *)(HDC))GetProcAddress(hInstance,"GDCFree");
  }
  return 0;
}