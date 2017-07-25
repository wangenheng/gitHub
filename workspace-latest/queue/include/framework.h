#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

#pragma warning(disable : 4786)
#pragma warning(disable : 4251)

#include <stdio.h>
#include <assert.h>
#include <winsock2.h>
#include <process.h>
#include <windows.h>
#include <iostream>
#include <map> 
#include <string> 
using namespace std; 

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")

class Adapter;

struct _declspec(dllimport) RuntimeClass
{
	char *classname;
	Adapter* (*pCreateFn)();
	
	void* CreateObject();
	static RuntimeClass* LoadByName(std::string classname);
		
	static RuntimeClass *pFirstClass;
	RuntimeClass *pBaseClass;
	RuntimeClass *pNextClass;
};

struct _declspec(dllimport) Class_List
{
	Class_List(RuntimeClass* pNewClass);
};

#define Runtime_Class(classname) \
	(&classname::class##classname)
		
#define Declare_RTTI(classname) \
	public:	\
		static  RuntimeClass  class##classname; \
		virtual RuntimeClass* GetRuntimeClass() const; \
		
#define Implement_RTTI(classname, basename) \
	static char lpsz##classname[] = #classname; \
	RuntimeClass classname::class##classname = \
	{ \
		lpsz##classname, NULL, Runtime_Class(basename), NULL \
	}; \
	static Class_List _init_##classname(&classname::class##classname); \
	RuntimeClass* classname::GetRuntimeClass() const \
	{ \
		return &classname::class##classname; \
	} \
		
#define Declare_DynCreate(classname) \
	Declare_RTTI(classname) \
	static Adapter* CreateObject(); \
	
#define Implement_DynCreate(classname, basename) \
	Adapter* classname::CreateObject() \
	{ \
		return new classname; \
	} \
	static char lpsz##classname[] = #classname; \
	RuntimeClass classname::class##classname = \
	{ lpsz##classname, classname::CreateObject, Runtime_Class(basename), NULL }; \
	static Class_List _init_##classname(&classname::class##classname); \
	RuntimeClass* classname::GetRuntimeClass() const \
	{ \
		return &classname::class##classname; \
	} \
		
class _declspec(dllimport) Adapter
{
public:
	virtual RuntimeClass* GetRuntimeClass() const
	{
		//return &Adapter::classBaseClass;
		return &Adapter::classAdapter;
	}
	virtual void printClassName()
	{
		std::cout << "Adapter!\n";
	}
public:
	static RuntimeClass classAdapter;
};

class _declspec(dllimport) ConfigReader
{
public:
	static ConfigReader* getInstance(const string &filepath);
	int GetParamValue(const string &key, string &value);
	int GetParamValue(const char *pszkey, char *pszvalue);
	static void printMap();

private:
	ConfigReader(void) {};
	ConfigReader(const string &filepath);

	int readFile(const string &filename);
	int parseContentLine(string &contentLine);

	map<string, string> paramMap;
	string m_filepath;
	static ConfigReader *m_pInstance;
	static map<string, ConfigReader*> m_ConReadClass;
};

#define FWParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\framework.ini")->GetParamValue(key, valueBuffer)

class _declspec(dllimport) CracksAlgorithm
{
public:
	CracksAlgorithm();
	~CracksAlgorithm();

public:
	// 破解-湖南双峰-欣瑞-异或算法
	static int  str_xor(unsigned char *str, int beg, int end);

	// 协议-潍坊青州-平安力合-crc校验算法
	static int  str_crc(unsigned char *str, int beg, int end, int modeValue);

	// 协议-重庆-未知-汉字高字节为零
	static void str_cha_hig_zro(const char *str_src, unsigned char *str_dest);

	// 破解 - 重庆-银达汇智 CRC16
	static int CRC16_CCITT(unsigned char *puchMsg, int usDataLen);
	
	static void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf);
	
	static void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf);

};

#define LOG_LINE_MAX 1024

enum LOG_LEVEL     // 日志级别
{		
	LL_FATAL = 0,
	LL_ERROR = 1,
	LL_WARN  = 2,    
	LL_INFO  = 3,
	LL_DEBUG = 4,
};

class _declspec(dllimport) Log
{
public:
    Log(void):m_openSuccess(false), m_LogLevel(LL_ERROR), m_showLogFlag(true), 
		m_maxLogFileSize(10 * 1024 *1024)
    {
    }
    Log(const char *filePath, LOG_LEVEL level = LL_ERROR);
	Log(const wchar_t *filePath, LOG_LEVEL level = LL_ERROR);
	Log(const char *filePath, char *level);
    virtual ~Log(void)
    {
        if (m_openSuccess)
        {
			CloseLogFile();
        }
    }

    bool GetOpenStatus() const
    {
        return m_openSuccess;
    }

    void OpenLogFile(const char *pFilePath, LOG_LEVEL level = LL_ERROR);
    void OpenLogFile(const wchar_t *pFilePath, LOG_LEVEL level = LL_ERROR);
    void WriteLog(LOG_LEVEL level, const char *pLogText, ...);
    void WriteLog(string logText, LOG_LEVEL level = LL_ERROR);
    void WriteLogEx(LOG_LEVEL level, const char *pLogText, ...);
	size_t GetLogFileSize();
	void ClearLogFile();
	void CloseLogFile();

    void SetLogLevel(LOG_LEVEL level = LL_ERROR)
    {
        m_LogLevel = level;
    }
    LOG_LEVEL GetLogLevel() const
    {
        return m_LogLevel;
    }

    void SetShowFlag(bool flag = true)
    {
        m_showLogFlag = flag;
    }
    bool GetShowFlag() const
    {
        return m_showLogFlag;
    }

    void SetMaxLogFileSize(size_t size)
    {
        m_maxLogFileSize = size;
    }
    size_t GetMaxLogFileSize() const
    {
        return m_maxLogFileSize;
    }

private:
    Log(const Log &Log){};

protected:
	//string W2A(const wchar_t *pWcsStr);  // wchar_t转化为string
    string ConvertToRealLogText(const char *pLogText, LOG_LEVEL level = LL_ERROR);
    const  std::string &StringFormat(std::string &srcString, const char *pFormatString, ...);
protected:
    //fstream   m_fileOut;
	FILE     *m_file;
    bool      m_openSuccess;    // 日志文件打开状态 true表示已经成功打开 否则没有打开
	string    m_logFilePath;    // 日志文件完整路径 string类型

protected:
    LOG_LEVEL m_LogLevel;       // 日志记录级别 默认为LL_ERROR
	bool      m_showLogFlag;    // 显示日志标志  默认为true
	size_t    m_maxLogFileSize; // 日志文件最大大小 超过该大小将被删除 默认为10MB
};

class _declspec(dllimport) Runnable
{
public:
	virtual ~Runnable() {};
	virtual void Run() = 0;
};

class _declspec(dllimport) ScreenAdapter : public Adapter
{
	Declare_DynCreate(ScreenAdapter)
public:
	ScreenAdapter(void);
public:
	virtual ~ScreenAdapter(void);

public:
	// 串口通信接口，操作条屏，综合屏

	// 打开串口设备
	virtual int  InitiDev		 ( ) { return 0; }

	// 关闭串口设备
	virtual int  CloseDev		 ( ) { return 0; }

	// 向条屏发送信息
	virtual int  SendStripScreen ( char *, char *, char *, char * ) { return 0; }

	// 向综合屏发送信息
	virtual int  SendIntegScreen ( char *, int, int, char * ) { return 0; }

	// 条屏初始化显示
	virtual int  InitStripScreen ( ) { return 0; }

	// 综合屏初始化显示
	virtual int  InitIntegScreen ( ) { return 0; }

};

class _declspec(dllimport) ScreenDispatch : public Runnable
{
public:
	ScreenDispatch( );
    ScreenDispatch( const char *classname );
	virtual ~ScreenDispatch( );
	void Run( );

private:
    int  do_process( );
	void ScreenSlipString( const char *, char *, char *, char *, char * );

private:
	const char    *m_classname;
    RuntimeClass  *m_prc;
    ScreenAdapter *m_screenAdapter;
	Log           *log_Screen;
};


#define MAXLINELENGTH	1024*2

class _declspec(dllimport) SerialPort
{														 
public:
	// contruction and destruction
	SerialPort();
	virtual		~SerialPort();

	// port initialisation											
	BOOL		InitPort(int portnr = 1, int baud = 9600, char parity = 'N', int databits = 8, int stopsbits = 1);
	void        ClosePort();

	int         ReadToPort(char* str, int strlen);
	int 		WriteToPort(char* str, int strlen);
	
	void        ResetCommBuffer();
	
private:
	// COM口端口号
	HANDLE				m_hComm;
	// handles
	UINT				m_nPortNr;
	BOOL          		m_isOpen;
	
	COMMTIMEOUTS		m_CommTimeouts;
	DCB					m_dcb;
	
	//读端口间隔时间(两个字符之间的间隔时间)
	DWORD 				m_readInterval;
	//读端口间隔时间(两个字符之间的间隔时间)
	DWORD 				m_readMutiplier;
	//写端口间隔时间(两个字符之间的间隔时间)
	DWORD 				m_writeMutiplier;
	//串口通讯特性
	//波特率
	DWORD 				m_BaudRate;
	//奇偶校验
	BYTE  				m_Parity;
	//数据位
	BYTE  				m_ByteSize;
	//停止位
	BYTE  				m_StopBits;
};

#define FD_SETSIZES 	512
#define MAX_LINE_SIZE	512

class _declspec(dllimport) SocketServer
{
public:
	SocketServer();
	//SocketServer(int port) : m_port(port) {printf("SocketServer::SocketServer\n");};
	SocketServer(int port);
	virtual ~SocketServer();
	
public:
    bool   InitSocket();
    void   Running();
    
protected:
	BOOL   InitSocketLib();
	int    Process(char *recvMessage);
     
private:
	WSADATA wsaData;
	
	SOCKET m_listenSock;
	SOCKET m_clientSock;
	struct sockaddr_in m_serverAddr;
	struct sockaddr_in m_clientAddr;
	int    m_clientAddrLen;
	int    m_serverAddrLen;
	
	int    m_port;
	
	// 
	fd_set m_socketSet;
	fd_set m_socketSetBak;
	int    m_sockcount ;

	// 
	char   m_recvBuffer[MAX_LINE_SIZE];
	char   m_sendBuffer[MAX_LINE_SIZE];
	int    m_bytes;

	Log   *log_socket;
	
};

class _declspec(dllimport) SplitStrings
{
public:
	static void StringFormat(const char *stringformat);
	static int  CheckDataFormat(char *str, char ch);
	static void SplitStr(int index, const char *srcstr, char *dststr, char ch);
	static int  SplitCharCount(const char *str, char ch);
	
};

class _declspec(dllimport) Thread : public Runnable
{
private:
	explicit Thread(const Thread &rhs);
	
public:
	Thread();
	Thread(Runnable *pRunnable);
	Thread(const char *ThreadName, Runnable *pRunnable = NULL);
	Thread(std::string ThreadName, Runnable *pRunnable = NULL);
	virtual ~Thread(void);
	
	//	开始运行线程
	//	@arg bSuspend 开始运行时是否挂起
	bool Start(bool bSuspend = false);
	
	//	运行的线程函数，可以使用派生类重写此函数
	virtual void Run();
		
	//	当前执行此函数线程等待线程结束
	//	@arg timeout 等待超时时间，如果为负数，等待无限时长
	void Join(int timeout = -1);
	
	//	恢复挂起的线程
	void Resume();
	
	//	挂起线程
	void Suspend();
	
	//	终止线程的执行
	bool Terminate(unsigned long ExitCode);
	
	unsigned int GetThreadID();
	std::string  GetThreadName();
	void SetThreadName(std::string ThreadName);
	void SetThreadName(const char *ThreadName);
	
private:
	static unsigned int WINAPI StatiThreadFunc(void *arg);
	
private:
	HANDLE          m_handle;
	Runnable *const m_pRunnable;
	unsigned int    m_ThreadID;
	std::string     m_ThreadName;
	volatile bool   m_bRun;
};

class _declspec(dllimport) VoicesAdapter : public Adapter
{
	Declare_DynCreate(VoicesAdapter)
public:
	VoicesAdapter(void);
public:
	virtual ~VoicesAdapter(void);

public:

	//virtual int InitiVoiceDev( ) { return 0; };

	//virtual int CloseVoiceDev( ) { return 0; };

	virtual int VoiceDevPlaye( int, char *, int, char * ) { return 0; };

};

class _declspec(dllimport) VoicesDispatch : public Runnable
{
public:
    VoicesDispatch( const char *classname );
    virtual ~VoicesDispatch();
	void  Run();

private:
    int   do_process( );
	void  VoicesSlipString( const char *, int *, char *, int *, char * );

private:
    const char    *m_classname;
    RuntimeClass  *m_prc;
    VoicesAdapter *m_VoicesAdapter;
};

class _declspec(dllimport) Queue
{
public:
	Queue();
	virtual ~Queue();
public:
	void Run();
private:
	// SocketServer
	SocketServer   *m_socketServer;

	// 写屏幕线程
	ScreenDispatch *m_screenDispatch;
	Thread         *m_screenThread;	

	// 语音播报线程
	VoicesDispatch *m_voicesDispatch;
	Thread         *m_voicesThread;

	// 设备状态更新线程
	//DeviceDispatch *m_deviceDispatch;
	//Thread         *m_deviceThread;
};


#endif // end __FRAMEWORK_H__