#pragma once

#include "Winsock2.h"
#include <iostream>
#include "Exception.h"


std::string ip;


void LibraryInitialization()
{
	WSADATA wsData;
	WORD lastVersion = MAKEWORD(2, 2);

	LPHOSTENT hostEnt = gethostbyname("localhost");
	if (!hostEnt)
		throw currentException("Host doesn't exist.", NULL);

	int lastError = WSAStartup(lastVersion, &wsData);
	if (lastError != 0)
		throw currentException("WSAStartup failed with code: ", lastError);
}


SOCKET CreateSocket()
{
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		throw INVALID_SOCKET;

	return sock;
}


SOCKADDR_IN InitSocket(int port)
{	
	SOCKADDR_IN sockAddr;

	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = port;
	sockAddr.sin_addr.s_addr = INADDR_ANY;

	return sockAddr;
}


void BindSocket(SOCKET socket, SOCKADDR_IN sockAddr)
{
	int bindSocket = bind(socket, (LPSOCKADDR)&sockAddr, sizeof(sockAddr));
	if (bindSocket == SOCKET_ERROR)
		throw std:: exception("Bind socket failed with code ", bindSocket);
}


void StartServer()
{

}