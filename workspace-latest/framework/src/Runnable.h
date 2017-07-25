#ifndef __RUNNABLE_H__
#define __RUNNABLE_H__

class _declspec(dllexport) Runnable
{
public:
	virtual ~Runnable() {};
	virtual void Run() = 0;
};

#endif // end __RUNNABLE_H__