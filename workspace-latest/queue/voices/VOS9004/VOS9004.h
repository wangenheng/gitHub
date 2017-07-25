#pragma once
#include "../../include/framework.h"
#include <fstream>
class VOS9004 : public VoicesAdapter
{
	Declare_DynCreate(VOS9004)
public:
	VOS9004(void) {};
public:
	virtual ~VOS9004(void) {};

public:
	virtual int VoiceDevPlaye( int, char *, int, char * );

private:
	void PlayVoices( char *custmerNum, int windowsNum );
	void inline initWavFileArray( char arrayname[][128], int rows );
	void inline fillReadCusNum( char arrayname[][128], char *custmerNum );
	void inline fillReadWinNum( char arrayname[][128], int windowsNum, int *rows );
	fstream	  * m_outputFile;
};

