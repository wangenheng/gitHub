#include<windows.h>
int  (_stdcall*SendDatafun)(int ,long ,int ,char[] ,int ,int ,int ,int );  
int  (_stdcall*SendDatagen)(int ,long ,int ,int ,char[],int ,int ,int ,int );
int  (_stdcall*SendDatagen16c)(int ,long ,int ,int ,char[],int ,int ,int ,int ,int);
int  (_stdcall*dataSend)(int ,int ,char[],int ,int ,int ,int );
int  (_stdcall*dataSendgen)(int ,int ,int ,char[] ,int ,int ,int ,int );
int  (_stdcall*picSend)(int ,int ,int ,char[],int );
int  (_stdcall*plainSend)(int ,int ,int ,byte[]);
HANDLE (_stdcall*portOpen)(char[] ,long ,char ,byte ,byte );
int  (_stdcall*portClose)(HANDLE);
int  (_stdcall*setWindowNo)(int,int,int,int,char[]);
int  (_stdcall*setWindowNogen)(int,int,int,int,int,char[]);
int  (_stdcall*ComShut)(int,int,int,int);
int  (_stdcall*SendDataZHLine)(int ,long ,int ,char[] ,int );  

HINSTANCE hInstance;
int test_Initialize(void)
{
  if ((hInstance=LoadLibrary("dlltpzp.dll"))!=NULL)
  {
	  SendDatafun=(int (_stdcall *)(int ,long ,int ,char[] ,int ,int ,int ,int ))GetProcAddress(hInstance,"SendDatafun");
      SendDatagen=(int (_stdcall *)(int ,long ,int ,int ,char[],int ,int ,int ,int ))GetProcAddress(hInstance,"SendDatagen");
      SendDatagen16c=(int (_stdcall *)(int ,long ,int ,int ,char[],int ,int ,int ,int ,int))GetProcAddress(hInstance,"SendDatagen16c");
      dataSend=(int (_stdcall *)(int ,int ,char[],int ,int ,int ,int ))GetProcAddress(hInstance,"dataSend");
      dataSendgen=(int (_stdcall *)(int ,int ,int ,char[] ,int ,int ,int ,int ))GetProcAddress(hInstance,"dataSendgen");
      picSend=(int (_stdcall *)(int ,int ,int ,char[],int ))GetProcAddress(hInstance,"picSend");
      plainSend=(int (_stdcall *)(int ,int ,int ,byte[]))GetProcAddress(hInstance,"plainSend");
	  portOpen=(HANDLE (_stdcall *)(char[] ,long ,char ,byte ,byte ))GetProcAddress(hInstance,"portOpen");
	  portClose=(int (_stdcall *)(HANDLE))GetProcAddress(hInstance,"portClose");
	  setWindowNo=(int (_stdcall *)(int,int,int,int,char[]))GetProcAddress(hInstance,"setWindowNo");
      setWindowNogen=(int (_stdcall *)(int,int,int,int,int,char[]))GetProcAddress(hInstance,"setWindowNogen");
	  ComShut=(int (_stdcall *)(int ,int ,int ,int ))GetProcAddress(hInstance,"ComShut");
	  SendDataZHLine=(int (_stdcall *)(int ,long ,int ,char[] ,int ))GetProcAddress(hInstance,"SendDataZHLine");
    
  }
  return 0;
}