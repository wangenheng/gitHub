#ifndef __SCREENDISPATCH_H__
#define __SCREENDISPATCH_H__

#include "ScreenAdapter.h"
#include "Runnable.h"
#include "Log.h"

class _declspec(dllexport) ScreenDispatch : public Runnable
{
public:
	ScreenDispatch( );
    ScreenDispatch( const char *classname );
	virtual ~ScreenDispatch( );
	void Run( );

private:
    int  do_process( );
	void ScreenSlipString( const char *, char *, char *, char *, char * );

private:
	const char    *m_classname;
    RuntimeClass  *m_prc;
    ScreenAdapter *m_screenAdapter;
	Log           *log_Screen;
};

#endif // end __SCREENDISPATCH_H__
