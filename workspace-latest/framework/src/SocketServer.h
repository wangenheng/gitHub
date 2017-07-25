// SocketServer.h
#ifndef __SOCKETSERVER_H__
#define __SOCKETSERVER_H__

#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "Log.h"

#define FD_SETSIZES 	512
#define MAX_LINE_SIZE	512

class _declspec(dllexport) SocketServer
{
public:
	SocketServer();
	//SocketServer(int port) : m_port(port) {printf("SocketServer::SocketServer\n");};
	SocketServer(int port);
	virtual ~SocketServer();
	
public:
    bool   InitSocket();
    void   Running();
    
protected:
	BOOL   InitSocketLib();
	int    Process(char *recvMessage);
     
private:
	WSADATA wsaData;
	
	SOCKET m_listenSock;
	SOCKET m_clientSock;
	struct sockaddr_in m_serverAddr;
	struct sockaddr_in m_clientAddr;
	int    m_clientAddrLen;
	int    m_serverAddrLen;
	
	int    m_port;
	
	// 
	fd_set m_socketSet;
	fd_set m_socketSetBak;
	int    m_sockcount ;

	// 
	char   m_recvBuffer[MAX_LINE_SIZE];
	char   m_sendBuffer[MAX_LINE_SIZE];
	int    m_bytes;

	Log   *log_socket;
	
};

#endif // end __SOCKETSERVER_H__