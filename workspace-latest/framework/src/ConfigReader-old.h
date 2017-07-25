#ifndef __CONFIGREADER_H__
#define __CONFIGREADER_H__

#pragma warning( disable : 4786 )
#pragma warning( disable : 4251 )

#include <iostream>
#include <map> 
#include <string> 
using namespace std; 

class _declspec(dllexport) ConfigReader
{
public:
	static ConfigReader* getInstance( const string &filepath );
	int         GetParamValue( const string &key, string &value );
	int         GetParamValue( const char *pszKey, char *pszValue );
	static void printMap();

private:
	ConfigReader( void ) {};
	ConfigReader( const string &filepath );

	int readFile(const string &filename);
	int parseContentLine(string &contentLine);

	map<string, string> paramMap;
	string m_filepath;
	static ConfigReader *m_pInstance;
	static map<string, ConfigReader*> m_ConReadClass;
};

#define FWParamValueINI(key, valueBuffer) 	ConfigReader::getInstance("cfg\\framework.ini")->GetParamValue(key, valueBuffer)

#endif // end __CONFIG_H__
