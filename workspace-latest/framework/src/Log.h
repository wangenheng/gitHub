#ifndef __LOG_H__
#define __LOG_H__

#pragma warning(disable : 4251)

#include <iostream>
#include <string>

using namespace std;

#define LOG_LINE_MAX 1024

enum LOG_LEVEL     // 日志级别
{		
	LL_FATAL = 0,
	LL_ERROR = 1,
	LL_WARN  = 2,    
	LL_INFO  = 3,
	LL_DEBUG = 4,
};

class _declspec(dllexport) Log
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

    void OpenLogFile( const char *pFilePath   , LOG_LEVEL level = LL_ERROR);
    void OpenLogFile( const wchar_t *pFilePath, LOG_LEVEL level = LL_ERROR);
    void WriteLog   ( LOG_LEVEL level, const char *pLogText, ...  );
    void WriteLog   ( string logText , LOG_LEVEL level = LL_ERROR );
    void WriteLogEx ( LOG_LEVEL level, const char *pLogText, ...  );
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
    Log(const Log &Log) { };

protected:
    string ConvertToRealLogText( const char *pLogText, LOG_LEVEL level = LL_ERROR );
    const  std::string &StringFormat( string &srcString, const char *pFormatString, ... );

protected:
	FILE     *m_file;
    bool      m_openSuccess;    // 日志文件打开状态 true表示已经成功打开 否则没有打开
	string    m_logFilePath;    // 日志文件完整路径 string类型

protected:
    LOG_LEVEL m_LogLevel;       // 日志记录级别 默认为LL_ERROR
	bool      m_showLogFlag;    // 显示日志标志  默认为true
	size_t    m_maxLogFileSize; // 日志文件最大大小 超过该大小将被删除 默认为10MB
};

#endif
