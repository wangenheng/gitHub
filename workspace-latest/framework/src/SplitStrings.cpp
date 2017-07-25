#include <string.h>
#include "SplitStrings.h"

int SplitStrings::SplitCharCount(const char *str, char ch)
{
	int index;
	int count;
	int len;

	count  = 0;
	len = strlen(str);

	for(index=0; index<len; index++)
	{
		if(str[index] == ch)
		{
			count++;
		}
	}

	return count;
}

int SplitStrings::CheckDataFormat(char *str, char ch)
{
	return SplitCharCount(str, ch);
}

void SplitStrings::StringFormat(const char *stringformat)
{
	
}

void SplitStrings::SplitStr(int index, const char *srcstr, char *dststr, char ch)
{
	int i, j, k;
	int len;

	j = k = 0;
	len = strlen(srcstr);
	for(i=0; i<len; i++)
	{
		if (srcstr[i] == ch)
		{
			j++;
			if (j-1 == index)
				break;
		}
		else
		{
			if (j == index)
				dststr[k++] = srcstr[i];
		}
	}
}