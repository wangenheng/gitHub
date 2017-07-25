#ifndef __CREACKSALGORITHEM_H__
#define __CREACKSALGORITHEM_H__

class _declspec(dllexport) CracksAlgorithm
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

	// 协议 - 重庆-银达汇智 CRC16
	static int CRC16_CCITT(unsigned char *puchMsg, int usDataLen);
	
	static void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf);
	
	static void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf);
};

#endif
