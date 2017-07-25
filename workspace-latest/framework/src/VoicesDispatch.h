// VoicesDispatch.h
#ifndef __VOICESDISPATCH_H__
#define __VOICESDISPATCH_H__

#include "VoicesAdapter.h"
#include "Runnable.h"

class _declspec(dllexport) VoicesDispatch : public Runnable
{
public:
    VoicesDispatch( const char *classname );
    virtual ~VoicesDispatch();
	void  Run();

private:
    int   do_process( );
	void  VoicesSlipString( const char *, int *, char *, int *, char * );
	//void  VoicesSlipChar( int, const char *, char *, char );

private:
    const char    *m_classname;
    RuntimeClass  *m_prc;
    VoicesAdapter *m_VoicesAdapter;
};

#endif // end __VOICESDISPATCH_H__
