#pragma once
#include "../../include/framework.h"

class VOS9002 : public VoicesAdapter
{
	Declare_DynCreate(VOS9002)
public:
	VOS9002(void) {};
public:
	virtual ~VOS9002(void) {};

public:
	virtual int VoiceDevPlaye( int, char *, int, char * );

private:
	void PlayVoices( char *custmerNum, int windowsNum );
	void inline initWavFileArray( char arrayname[][128], int rows );
	void inline fillReadCusNum( char arrayname[][128], char *custmerNum );
	void inline fillReadWinNum( char arrayname[][128], int windowsNum, int *rows );
};

