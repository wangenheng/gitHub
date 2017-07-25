#include "SocketServer.h"
#include "CyclBuffer.h"
#include "SplitStrings.h"
#include "ConfigReader.h"

SocketServer::SocketServer()
{
	char temp[8];
	memset(temp, 0, sizeof(temp));
	FWParamValueINI("LOGLEVEL", temp);
	log_socket = new Log("log\\SocketServer.log", temp);
	log_socket->WriteLog(LL_DEBUG,  "SocketServer::SocketServer() \n[%s|%d]",  __FILE__, __LINE__);
}

SocketServer::SocketServer(int port) : m_port(port)
{
	char temp[8];
	memset(temp, 0, sizeof(temp));
	FWParamValueINI("LOGLEVEL", temp);
	log_socket = new Log("log\\SocketServer.log", temp);
	log_socket->WriteLog(LL_DEBUG,  "SocketServer::SocketServer() \n[%s|%d]",  __FILE__, __LINE__);
}

SocketServer::~SocketServer()
{
	closesocket(m_listenSock);
	log_socket->WriteLog(LL_DEBUG,  "SocketServer::~SocketServer() \n[%s|%d]",  __FILE__, __LINE__);
	log_socket->CloseLogFile();
	delete log_socket;
	WSACleanup();
}

BOOL SocketServer::InitSocketLib()
{
	log_socket->WriteLog(LL_INFO,  "�������˳���...... \n[%s|%d]",  __FILE__, __LINE__);
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
		printf("WSAStartup Failed, Error=[%d]\n", WSAGetLastError());
		return false;
	}
	else
		log_socket->WriteLog(LL_INFO,  "1 ���سɹ� \n[%s|%d]",  __FILE__, __LINE__);
	return true;
}

bool SocketServer::InitSocket()
{
	InitSocketLib();
	
	m_listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if ( m_listenSock == INVALID_SOCKET ) {
		log_socket->WriteLog(LL_ERROR,  "socket() Failed, Error = [%d]\n[%s|%d]", WSAGetLastError(),  __FILE__, __LINE__);
		return 1;
	}
	else
		log_socket->WriteLog(LL_INFO,  "2 �Ѵ��������׽���:[%d]\n[%s|%d]", m_listenSock,  __FILE__, __LINE__);
	
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port   = htons(m_port);
	m_serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	//ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		
	if (bind(m_listenSock, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR)
	{
		log_socket->WriteLog(LL_ERROR,  "bind() Failed, Error=[%d]\n[%s|%d]", WSAGetLastError(),  __FILE__, __LINE__);
		return 1;
	}
	else
		log_socket->WriteLog(LL_INFO,  "3 �󶨳ɹ�������IP��ַ:[%s],�˿ں�:[%d]\n[%s|%d]", inet_ntoa(m_serverAddr.sin_addr), \
			ntohs(m_serverAddr.sin_port),  __FILE__, __LINE__);
		
	if (listen(m_listenSock, 100) == SOCKET_ERROR)
	{
		log_socket->WriteLog(LL_ERROR,  "listen Failed, Error=[%d]\n[%s|%d]", WSAGetLastError(),  __FILE__, __LINE__);
		return 1;
	}
	else
		log_socket->WriteLog(LL_INFO,  "4 �������״̬\n[%s|%d]", __FILE__, __LINE__);
		
	return true;
}

void SocketServer::Running()
{
	int status;
	FD_ZERO(&m_socketSet);
	FD_SET(m_listenSock, &m_socketSet);
	FD_ZERO(&m_socketSetBak);
	
	status = 0;
	while( true)
	{
		m_socketSetBak = m_socketSet;
		m_sockcount = select(0, &m_socketSetBak, NULL, NULL, NULL);
		if (m_sockcount > 0)
		{
			for (int i = 0; i < (int)m_socketSetBak.fd_count; i++)
			{
				if (m_socketSetBak.fd_array[i] == m_listenSock)
				{
					if (m_socketSet.fd_count < FD_SETSIZES)
					{
						m_clientAddrLen = sizeof(m_clientAddr);
						m_clientSock = accept(m_listenSock, (sockaddr*)&m_clientAddr, &m_clientAddrLen);
						FD_SET(m_clientSock, &m_socketSet);
						log_socket->WriteLog(LL_DEBUG, "����һ���ͻ������ӣ��Է���ַ:[%s],�˿ں�:[%d]\n[%s|%d]", \
							inet_ntoa(m_clientAddr.sin_addr), ntohs(m_clientAddr.sin_port), __FILE__, __LINE__);
						log_socket->WriteLog(LL_DEBUG, "������ÿͻ��˵��׽���Ϊ:%d\n[%s|%d]", m_clientSock, __FILE__, __LINE__);
					}
					else
					{
						log_socket->WriteLog(LL_DEBUG, "��������̫��\n[%s|%d]", __FILE__, __LINE__);
						continue;
					}
				}
				else
				{
					memset(m_recvBuffer, 0, sizeof(m_recvBuffer));
					m_bytes = recv(m_socketSetBak.fd_array[i], m_recvBuffer, sizeof(m_recvBuffer), 0);
					if (m_bytes == SOCKET_ERROR)
					{
						log_socket->WriteLog(LL_DEBUG, "���ܿͻ���%d�Ƿ��ر�!!\n[%s|%d]", \
							m_socketSetBak.fd_array[i], __FILE__, __LINE__);
						log_socket->WriteLog(LL_DEBUG, "���ߵ���recv() Failed,Error=[%d]\n[%s|%d]", \
							WSAGetLastError(), __FILE__, __LINE__);
						closesocket(m_socketSetBak.fd_array[i]);
						FD_CLR(m_socketSetBak.fd_array[i], &m_socketSet);
					}
					else if (m_bytes > 0)
					{
						log_socket->WriteLog(LL_DEBUG, "���յ�����[%d]������:%s\n[%s|%d]", \
							m_socketSetBak.fd_array[i], m_recvBuffer, __FILE__, __LINE__);
						memset(m_sendBuffer, 0, sizeof(m_sendBuffer));
						
						status = Process(m_recvBuffer);

						//strncpy(m_sendBuffer, "Hello client! I am server", strlen("Hello client! I am server"));
						sprintf(m_sendBuffer, "%d", status);
						m_bytes = send(m_socketSetBak.fd_array[i], m_sendBuffer, strlen(m_sendBuffer), 0);
						if (m_bytes == SOCKET_ERROR)
						{
							log_socket->WriteLog(LL_ERROR, "send() Failed, Error=[%d]\n[%s|%d]", \
								WSAGetLastError(), __FILE__, __LINE__);
							break;
						}
						else if (m_bytes <= 0)
						{
							log_socket->WriteLog(LL_ERROR, "��Ϣ����ʧ��!!\n[%s|%d]", __FILE__, __LINE__);
							break;
						}
						else
						{
							log_socket->WriteLog(LL_DEBUG, "���ͻ�[%d]��Ϣ�ѷ��ͣ���Ϣ����%d�ֽ�\n[%s|%d]", \
								m_socketSetBak.fd_array[i], m_bytes, __FILE__, __LINE__);
						}
					}
					else
					{
						log_socket->WriteLog(LL_INFO, "�ͻ���[%d]���ٷ������ݣ������ر����ӣ�Ϊ�ͻ������Ӵ������׽��ֽ��ر�!!\n[%s|%d]", \
							m_socketSetBak.fd_array[i], __FILE__, __LINE__);
						closesocket(m_socketSetBak.fd_array[i]);
						FD_CLR(m_socketSetBak.fd_array[i], &m_socketSet);
					}
				}
			}
		}
		else
		{
			log_socket->WriteLog(LL_ERROR, "select() Failed, Error=[%d]\n[%s|%d]", \
							WSAGetLastError(), __FILE__, __LINE__);
			break;
		}
	}
}

int SocketServer::Process(char *recvMessage)
{
	int  status = 0;
	char flag = recvMessage[0];

	if (strcmp(recvMessage, "WATCH_MANAGE") == 0)
		return 9999;
	if (SplitStrings::CheckDataFormat(recvMessage, '|') != 4)
		return -1;
	
	switch (flag)
	{
	case '1': // ScreenData
		log_socket->WriteLog(LL_INFO, "ScreenData recvMessage = [%s]\n[%s|%d]", \
							recvMessage, __FILE__, __LINE__);
		cyclBuffer.WriteScreenData(recvMessage);
		
		break;
	case '2': // VoicesData
		log_socket->WriteLog(LL_INFO, "VoicesData recvMessage = [%s]\n[%s|%d]", \
							recvMessage, __FILE__, __LINE__);
		cyclBuffer.WriteVoicesData(recvMessage);
		
		break;
	default:
		status = -1;
		break;
	}

	return status;
}
