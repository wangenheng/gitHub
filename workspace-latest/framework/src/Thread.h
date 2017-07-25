#ifndef __THREAD_H__
#define __THREAD_H__

#pragma warning(disable : 4251)

#include <string>
#include <windows.h>
#include <process.h>

#include "Runnable.h"

class _declspec(dllexport) Thread : public Runnable
{
private:
	explicit Thread(const Thread &rhs);
	
public:
	Thread();
	Thread(Runnable *pRunnable);
	Thread(const char *ThreadName, Runnable *pRunnable = NULL);
	Thread(std::string ThreadName, Runnable *pRunnable = NULL);
	virtual ~Thread(void);
	
	//	��ʼ�����߳�
	//	@arg bSuspend ��ʼ����ʱ�Ƿ����
	bool Start(bool bSuspend = false);
	
	//	���е��̺߳���������ʹ����������д�˺���
	virtual void Run();
		
	//	��ǰִ�д˺����̵߳ȴ��߳̽���
	//	@arg timeout �ȴ���ʱʱ�䣬���Ϊ�������ȴ�����ʱ��
	void Join(int timeout = -1);
	
	//	�ָ�������߳�
	void Resume();
	
	//	�����߳�
	void Suspend();
	
	//	��ֹ�̵߳�ִ��
	bool Terminate(unsigned long ExitCode);
	
	unsigned int GetThreadID();
	std::string  GetThreadName();
	void SetThreadName(std::string ThreadName);
	void SetThreadName(const char *ThreadName);
	
private:
	static unsigned int WINAPI StatiThreadFunc(void *arg);
	
private:
	HANDLE          m_handle;
	Runnable *const m_pRunnable;
	unsigned int    m_ThreadID;
	std::string     m_ThreadName;
	volatile bool   m_bRun;
};

#endif // end __THREAD_H__
