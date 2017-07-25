#ifndef __VOICESADAPTER_H__
#define __VOICESADAPTER_H__

#include <windows.h>
#pragma comment(lib, "winmm.lib")

#include "Adapter.h"

class _declspec(dllexport) VoicesAdapter : public Adapter
{
	Declare_DynCreate(VoicesAdapter)
public:
	VoicesAdapter(void);
public:
	virtual ~VoicesAdapter(void);

public:

	//virtual int InitiVoiceDev( ) { return 0; };

	//virtual int CloseVoiceDev( ) { return 0; };

	virtual int VoiceDevPlaye( int, char *, int, char * ) { return 0; };

};

#endif // end __VOICESADAPTER_H__
