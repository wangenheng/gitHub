#include "ConfigReader.h"
#include <fstream>
#include <cctype>
#include <algorithm>

using namespace std;

#define INI_FILE_NAME "framework.ini"

ConfigReader *ConfigReader::m_pInstance;
map<string, ConfigReader*> ConfigReader::m_ConReadClass;

static inline string &ltrim(string &s)
{
	s.erase( s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))) );
	return s;
}

static inline string &rtrim(string &s)
{
	s.erase( find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end() );
	return s;
}

static inline string &trim(string &s)
{
	return ltrim(rtrim(s));
}

ConfigReader::ConfigReader(const string &filepath)
{
	m_filepath = filepath;
}

ConfigReader* ConfigReader::getInstance(const string &filepath)
{
	string key = filepath;
	map<string, ConfigReader *>::iterator iter = m_ConReadClass.find(key);
	if (iter != m_ConReadClass.end()) {
		m_pInstance = iter->second;
		return m_pInstance;
	} else {
		m_pInstance = new ConfigReader(filepath);
		m_ConReadClass.insert(make_pair(key, m_pInstance));
		return m_pInstance;
	}
}

int ConfigReader::GetParamValue(const string &key, string &valueBuffer)
{
	if (paramMap.empty())  {
		readFile(m_filepath);
	}

	map<string, string>::iterator iter = paramMap.find(key);
	if (iter != paramMap.end()) {
		valueBuffer = iter->second;
		return 0;
	} else {
		return 1;
	}
}

int ConfigReader::GetParamValue(const char *pszkey, char *pszvalue)
{
	if (paramMap.empty())  {
		readFile(m_filepath);
	}

	string key = pszkey;
	string valueBuffer;
	map<string, string>::iterator iter = paramMap.find(key);
	if (iter != paramMap.end()) {
		valueBuffer = iter->second;
		strncpy(pszvalue, valueBuffer.c_str(), strlen(valueBuffer.c_str()));
		return 0;
	} else {
		return 1;
	}
}

void ConfigReader::printMap()
{
	map<string, ConfigReader *>::iterator iter;
	for (iter=m_ConReadClass.begin(); iter!=m_ConReadClass.end(); ++iter)
		cout << "Key: " << iter->first << "\t" << "value: " << &iter->second << endl;
}

int ConfigReader::readFile(const string &filename)
{
	std::string strFilename(filename.begin(), filename.end());
	ifstream infile(strFilename.c_str());
	string buffer;
	while (getline(infile, buffer)) {
		parseContentLine(buffer);
	}
	return 0;
}

int ConfigReader::parseContentLine(string &contentLine)
{
	
	contentLine = trim(contentLine);
	if (contentLine.size() < 1) {
		return 0;
	}

	if (contentLine.substr(0, 1) == "#"|| contentLine.substr(0, 1) == ";" || \
		contentLine.substr(0, 1) == " ") {
		return 0;
	}
	
	string::size_type equalPos = contentLine.find_first_of("=");
	string::size_type startPos = 0;
	string::size_type endPos   = contentLine.size() - 1;

	if (equalPos <= startPos || equalPos > endPos) {
		return -1;
	}

	string temp = contentLine.substr(startPos, equalPos);
	string key  = rtrim(temp);

	temp = contentLine.substr(equalPos + 1, endPos);
	string value = ltrim(temp);
	
	paramMap.insert(std::make_pair(key, value));
	
	return 0;
}
