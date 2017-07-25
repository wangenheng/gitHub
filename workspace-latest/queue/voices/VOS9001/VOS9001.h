#pragma once
#include "../../include/framework.h"

class VOS9001 : public VoicesAdapter
{
	Declare_DynCreate(VOS9001)
public:
	VOS9001(void) {};
	virtual ~VOS9001(void) {};

public:
	virtual int VoiceDevPlaye( int, char *, int, char * );

private:
	void PlayVoices( char *custmerNum, int windowsNum );
	void inline initWavFileArray(char arrayname[][128], int rows);
	void inline fillReadWinNum( char arrayname[][128], int windowsNum, int *rows );
};
