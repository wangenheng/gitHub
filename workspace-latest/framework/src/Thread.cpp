#include "Thread.h"

Thread::Thread(void) : m_pRunnable(NULL), m_bRun(false)
{
}

Thread::~Thread(void)
{
}

Thread::Thread(Runnable *pRunnable) : 
	m_ThreadName(""),
	m_pRunnable(pRunnable),
	m_bRun(false)
{
}

Thread::Thread(const char *ThreadName, Runnable *pRunnable) :
	m_ThreadName(ThreadName),
	m_pRunnable(pRunnable),
	m_bRun(false)
{
}

Thread::Thread(std::string ThreadName, Runnable *pRunnable) :
	m_ThreadName(ThreadName),
	m_pRunnable(pRunnable),
	m_bRun(false)
{
}

bool Thread::Start(bool bSuspend)
{
	if (m_bRun)
		return true;
	if (bSuspend)
	{
		m_handle = (HANDLE)_beginthreadex( 
						NULL, 
						0, 
						StatiThreadFunc, 
						this,
						CREATE_SUSPENDED, 
						&m_ThreadID
					);
	}
	else
	{
		m_handle = (HANDLE)_beginthreadex(
						NULL,
						0,
						StatiThreadFunc,
						this,
						0,
						&m_ThreadID
					);
	}
	//m_bRun = (m_handle != NULL);

	return m_bRun;
}

void Thread::Run()
{
	if (!m_bRun)
		return ;
	if (m_pRunnable != NULL)
		m_pRunnable->Run();
	m_bRun = false;
}

void Thread::Join(int timeout)
{
	if (m_handle == NULL || !m_bRun)
		return ;
	if (timeout <= 0)
		timeout = INFINITE;
	::WaitForSingleObject(m_handle, timeout);
}

void Thread::Resume()
{
	if (m_handle == NULL || !m_bRun)
		return ;
	::ResumeThread(m_handle);
}

void Thread::Suspend()
{
	if (m_handle == NULL || !m_bRun)
		return ;
	::SuspendThread(m_handle);
}

bool Thread::Terminate(unsigned long ExitCode)
{
	if (m_handle == NULL || !m_bRun)
		return true;
	if (::TerminateThread(m_handle, ExitCode))
	{
		::CloseHandle(m_handle);
		return true;
	}
	return false;
}

unsigned int Thread::GetThreadID()
{
	return m_ThreadID;
}

std::string Thread::GetThreadName()
{
	return m_ThreadName;
}

void Thread::SetThreadName(std::string ThreadName)
{
	m_ThreadName = ThreadName;
}

void Thread::SetThreadName(const char *ThreadName)
{
	if (ThreadName == NULL)
		m_ThreadName = "";
	else
		m_ThreadName = ThreadName;
}

unsigned int Thread::StatiThreadFunc(void *arg)
{
	Thread *pThread = (Thread *)arg;
	pThread->m_bRun = true;
	pThread->Run();
	return 0;
}