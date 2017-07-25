#ifndef __SPLITSTR_H__
#define __SPLITSTR_H__

class _declspec(dllexport) SplitStrings
{
public:
	static void StringFormat(const char *stringformat);
	static int  CheckDataFormat(char *str, char ch);
	static void SplitStr(int index, const char *srcstr, char *dststr, char ch);
	static int  SplitCharCount(const char *str, char ch);
	
};

#endif // end __SPLITSTR_H__