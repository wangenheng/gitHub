#ifndef __SCREENADAPTER_H__
#define __SCREENADAPTER_H__

#include "Adapter.h"

class _declspec(dllexport) ScreenAdapter : public Adapter
{
	Declare_DynCreate(ScreenAdapter)
public:
	ScreenAdapter(void);
public:
	virtual ~ScreenAdapter(void);

public:
	// ����ͨ�Žӿڣ������������ۺ���

	// �򿪴����豸
	virtual int  InitiDev		 ( ) { return 0; }

	// �رմ����豸
	virtual int  CloseDev		 ( ) { return 0; }

	// ������������Ϣ
	virtual int  SendStripScreen ( char *, char *, char *, char * ) { return 0; }

	// ���ۺ���������Ϣ
	virtual int  SendIntegScreen ( char *, int, int, char * ) { return 0; }

	// ������ʼ����ʾ
	virtual int  InitStripScreen ( ) { return 0; }

	// �ۺ�����ʼ����ʾ
	virtual int  InitIntegScreen ( ) { return 0; }

};

#endif // end __SCREENADAPTER_H__
