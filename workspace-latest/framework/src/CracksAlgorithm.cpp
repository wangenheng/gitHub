#include <string.h>
#include "CracksAlgorithm.h"

// 破解-湖南双峰-欣瑞-异或算法
int CracksAlgorithm::str_xor(unsigned char *str, int beg, int end)
{
	int sum = 0;

	for (int i=beg; i<end; i++)
		sum ^= str[i];

	return sum;
}

// 协议-潍坊青州-平安力合-crc校验算法
int CracksAlgorithm::str_crc(unsigned char *str, int beg, int end, int modeValue)
{
	long sum;

	sum = 0;
	for (int i=beg; i<end; i++)
		sum += str[i];

	return sum % modeValue;

}

// 协议-重庆-未知-汉字高字节为零
void CracksAlgorithm::str_cha_hig_zro(const char *str_src, unsigned char *str_dest)
{
	int index;
	int len;

	index = 0;
	len = strlen(str_src);

	for (int i=0; i<len; i++)
	{
		if (str_src[i]&0X80) // 是汉字
		{
			str_dest[index++] = (str_src[i]&0XF0)>>4;
			str_dest[index++] =  str_src[i]&0X0F;
		}
		else // 非汉字
		{
			str_dest[index++] =  str_src[i];
		}
	}
}

//CRC16_CCITT：多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0x0000异或
int CracksAlgorithm::CRC16_CCITT(unsigned char *puchMsg, int usDataLen)  
{  
	/*
	unsigned short wCRCin = 0x0000;  
	unsigned short wCPoly = 0x1021;  
	unsigned char wChar = 0;  
    
	while (usDataLen--)     
	{  
        wChar = *(puchMsg++);  
        CracksAlgorithm::InvertUint8(&wChar,&wChar);  
        wCRCin ^= (wChar << 8);  
        for(int i = 0;i < 8;i++)  
        {  
			if(wCRCin & 0x8000)  
				wCRCin = (wCRCin << 1) ^ wCPoly;  
			else  
				wCRCin = wCRCin << 1;  
        }  
	}  
	CracksAlgorithm::InvertUint16(&wCRCin,&wCRCin);  
	return (wCRCin) ;  
	*/
	unsigned short wCRCin = 0x0000;  
	unsigned short wCPoly = 0x1021;  
	unsigned char wChar = 0;  
    
	while (usDataLen--)     
	{  
        wChar = *(puchMsg++);  
        wCRCin ^= (wChar << 8);  
        for(int i = 0;i < 8;i++)  
        {  
			if(wCRCin & 0x8000)  
				wCRCin = (wCRCin << 1) ^ wCPoly;  
			else  
				wCRCin = wCRCin << 1;  
        }  
	}  
  return (wCRCin) ;  
}  

void CracksAlgorithm::InvertUint8(unsigned char *dBuf,unsigned char *srcBuf)
{ 
	int i; 
	unsigned char tmp[4]; 
	tmp[0] = 0; 
	for(i=0;i< 8;i++) 
	{ 
		if( srcBuf[0]& (1 << i) )

			tmp[0]|=1<<(7-i); 
	} 
	dBuf[0] = tmp[0];
}

void CracksAlgorithm::InvertUint16(unsigned short *dBuf,unsigned short *srcBuf)  
{  
    int i;  
    unsigned short tmp[4];  
    tmp[0] = 0;  
    for(i=0;i< 16;i++)  
    {  
		if(srcBuf[0]& (1 << i))  
			tmp[0]|=1<<(15 - i);  
    }  
    dBuf[0] = tmp[0];  
}  