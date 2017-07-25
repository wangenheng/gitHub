#ifndef __CREACKSALGORITHEM_H__
#define __CREACKSALGORITHEM_H__

class _declspec(dllexport) CracksAlgorithm
{
public:
	CracksAlgorithm();
	~CracksAlgorithm();

public:
	// �ƽ�-����˫��-����-����㷨
	static int  str_xor(unsigned char *str, int beg, int end);

	// Э��-Ϋ������-ƽ������-crcУ���㷨
	static int  str_crc(unsigned char *str, int beg, int end, int modeValue);

	// Э��-����-δ֪-���ָ��ֽ�Ϊ��
	static void str_cha_hig_zro(const char *str_src, unsigned char *str_dest);

	// Э�� - ����-������� CRC16
	static int CRC16_CCITT(unsigned char *puchMsg, int usDataLen);
	
	static void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf);
	
	static void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf);
};

#endif
