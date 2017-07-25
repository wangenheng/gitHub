#include <ctime>
#include <string>
#include <cstdarg>
#include <stdio.h>

#include "Log.h"

Log::Log(const char *filePath, LOG_LEVEL level):m_showLogFlag(true), m_LogLevel(level),
	m_maxLogFileSize(10 * 1024 * 1024), m_logFilePath(filePath)
{
	m_file = fopen(filePath, "a+");
	m_openSuccess = m_file!=NULL ? true : false;
}

Log::Log(const char *filePath, char *level):m_showLogFlag(true),
	m_maxLogFileSize(10 * 1024 * 1024), m_logFilePath(filePath)
{
	m_file = fopen(filePath, "a+");
	m_openSuccess = m_file!=NULL ? true : false;

	if (strncmp(level, "DEBUG", strlen("DEBUG")) == 0)
		m_LogLevel = LL_DEBUG;
	else if (strncmp(level, "INFO", strlen("INFO")) == 0)
		m_LogLevel = LL_INFO;
	else if (strncmp(level, "WARN", strlen("WARN")) == 0)
		m_LogLevel = LL_WARN;
	else if (strncmp(level, "ERROR", strlen("ERROR")) == 0)
		m_LogLevel = LL_ERROR;
	else if (strncmp(level, "FATAL", strlen("FATAL")) == 0)
		m_LogLevel = LL_FATAL;
}


void Log::OpenLogFile(const char *pFilePath, LOG_LEVEL level)
{
	if (pFilePath)
	{
		if (m_openSuccess)
		{
			CloseLogFile();
		}
		m_file = fopen(pFilePath, "a+");
	}
	m_openSuccess = m_file!=NULL ? true : false;
	m_LogLevel    = level;
	m_logFilePath = pFilePath;
}

size_t Log::GetLogFileSize()
{
	if (!m_openSuccess)
	{
		return 0;
	}
	long curPos = ftell(m_file);
	fseek(m_file, 0, SEEK_END);
	long pos = ftell(m_file);
	fseek(m_file, 0, curPos);

	return static_cast<size_t>(pos);
}

void Log::WriteLog(string logText, LOG_LEVEL level)
{
    if (m_openSuccess && level <= m_LogLevel)
    {
        string flag;
        if (level <= LL_FATAL)
        {
        	flag = "<FATAL> ";
        }
        else if (level == LL_ERROR)
        {
        	flag = "<ERROR> ";
        }
        else if (level == LL_WARN)
        {
        	flag = "<WARN> ";
        }
        else if (level == LL_INFO)
        {
        	flag = "<INFO> ";
        }
        else
        {
        	flag = "<DEBUG> ";
        }

		time_t nowTime = time(NULL);
        struct tm *pLocalTime = localtime(&nowTime);
		char timeString[128] = {0};
		sprintf(timeString, "[%04d-%02d-%02d %02d:%02d:%02d] ", pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1, 
			pLocalTime->tm_mday, pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);
		fprintf(m_file, "%s", timeString);

        if (m_showLogFlag)
        {
        	logText = flag + logText + "\n";
        }
        if (m_maxLogFileSize > 0 && GetLogFileSize() > m_maxLogFileSize)
		{
			ClearLogFile();
		}
		fprintf(m_file, "%s", logText.c_str());
		fflush(m_file);
    }
    return;
}

void Log::WriteLog(LOG_LEVEL level, const char *pLogText, ...)
{
    va_list args;
    char logText[LOG_LINE_MAX] = { 0 };
    va_start(args, pLogText);
	vsprintf(logText, pLogText, args);
    WriteLog(logText, level);
}

const string& Log::StringFormat(std::string &srcString, const char *pFormatString, ...)
{
    va_list args;
    char tempString[LOG_LINE_MAX*2] = { 0 };
    va_start(args, pFormatString);
	vsprintf(tempString, pFormatString, args);
    srcString = tempString;
    return srcString;
}

string Log::ConvertToRealLogText(const char *pLogText, LOG_LEVEL level)
{
    if (NULL == pLogText)
    {
        return string("");
    }

    time_t nowTime = time(NULL);
    struct tm *pLocalTime = localtime(&nowTime);

    string strLogText(pLogText);
    string::size_type pos = strLogText.find("$(");
    while (pos != string::npos)
    {
        string::size_type tempPos = strLogText.find(")", pos);
        if (tempPos != string::npos)
        {
            string strSubString = strLogText.substr(pos, tempPos - pos + 1);
            if (strSubString == string("$(DATE)"))
            {
                StringFormat(strSubString, "%04d-%02d-%02d", pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1, pLocalTime->tm_mday);
            }
            else if (strSubString == string("$(TIME)"))
            {
                StringFormat(strSubString, "%02d:%02d:%02d", pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);
            }
            else if (strSubString == string("$(DATETIME)"))
            {
                StringFormat(strSubString, "%04d-%02d-%02d %02d:%02d:%02d", pLocalTime->tm_year + 1900, 
                    pLocalTime->tm_mon + 1, pLocalTime->tm_mday, pLocalTime->tm_hour, pLocalTime->tm_min, 
                    pLocalTime->tm_sec);
            }
            else if (strSubString == string("$(LEVELFLAG)"))
            {
                if (LL_ERROR == level)
                {
                    strSubString = "ERROR";
                }
                else if (LL_WARN == level)
                {
                    strSubString = "WARN";
                }
                else if (LL_INFO == level)
                {
                    strSubString = "INFORMATION";
                }
            }
            strLogText.replace(pos, tempPos - pos + 1, strSubString);
            pos = strLogText.find("$(", tempPos + 1);
        }
        else 
        {
            break;
        }
    }

    return strLogText;
}

void Log::WriteLogEx(LOG_LEVEL level, const char *pLogText, ...)
{
    if (m_openSuccess && level <= m_LogLevel)
    {
        va_list args;
        char szLogText[LOG_LINE_MAX] = {0};
        va_start(args, pLogText);
		vsprintf(szLogText, pLogText, args);
        string strRealLogText = ConvertToRealLogText(szLogText, level);
        if (m_maxLogFileSize > 0 && GetLogFileSize() > m_maxLogFileSize)
        {
            ClearLogFile();
        }
		fprintf(m_file, "%s", strRealLogText.c_str());
    }
}

void Log::CloseLogFile()
{
	fflush(m_file);
	fclose(m_file);
    m_openSuccess = false;
}

void Log::ClearLogFile()
{
    if (!m_openSuccess || m_logFilePath == string("")) { return ; }

    CloseLogFile();
	m_file = fopen(m_logFilePath.c_str(), "w+");
    m_openSuccess = m_file!=NULL ? true : false;
}
