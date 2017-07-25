// VoicesDispatch.h
#include <iostream>
#include "VoicesDispatch.h"
#include "CyclBuffer.h"
#include "SplitStrings.h"

using namespace std;

VoicesDispatch::VoicesDispatch( const char *classname )
{
    m_classname = classname;
    m_prc = RuntimeClass::LoadByName( m_classname );
    if (m_prc == NULL) {
        cout << m_classname << endl;
        return ;
    }
    m_VoicesAdapter = (VoicesAdapter *)m_prc->CreateObject();
    //cout << "VoicesDispatch::" << m_classname << endl;
}

VoicesDispatch::~VoicesDispatch()
{
    delete m_VoicesAdapter;
    cout << "~VoicesDispatch::"<< m_classname << endl;
}

void VoicesDispatch::Run()
{
	printf("VoicesDispatch::Run()\n");
	do_process();
}

int VoicesDispatch::do_process()
{
	char recvBuffer[128];	
	int  readMode;			// ��ģʽ
	char custmerNum[8];		// �ͻ����
	int  windowsNum;		// ���ڱ��
	char informatin[8];		// �˿�

	while (true)
	{
		readMode   = 0;
		windowsNum = 0;
		memset(recvBuffer, 0, sizeof(recvBuffer));
		memset(custmerNum, 0, sizeof(custmerNum));
		memset(informatin, 0, sizeof(informatin));

		cyclBuffer.ReadVoicesData(recvBuffer);
		// recvBuffer :"2|1|B001|2|�˿�"
		// readMode   : .|1|....|.|....
		// custmerNum :	.|.|B001|.|....
		// windowsNum : .|.|....|2|....
		// informatin : .|.|....|.|�˿�
		VoicesSlipString( recvBuffer, &readMode, custmerNum, &windowsNum, informatin );
		m_VoicesAdapter->VoiceDevPlaye( readMode, custmerNum, windowsNum, informatin );
	}

	return 0;
}

void VoicesDispatch::VoicesSlipString( const char *recvBuffer, int *readMode, char *custmerNum, int *windowsNum, char *informatin )
{
	// 0 1  2   3  4
	//"2|1|B001|2|�˿�"
	char temp[16];

	// readMode
	memset(temp, 0, sizeof(temp));
	SplitStrings::SplitStr( 1, recvBuffer, temp      , '|' );
	*readMode   = atoi( temp );

	// �˿����
	SplitStrings::SplitStr( 2, recvBuffer, custmerNum, '|' );

	// ���ڱ��
	memset(temp, 0, sizeof(temp));
	SplitStrings::SplitStr( 3, recvBuffer, temp      , '|' );
	*windowsNum = atoi( temp );

	// ��Ϣ
	//SplitStrings::SplitStr( 4, recvBuffer, informatin, '|' );
}
