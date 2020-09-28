#pragma once
#pragma warning(disable : 4996) 

#include "Winsock2.h"
#include <iostream>
#include "Exception.h"


std::string ip;
const int N = 256;


void LibraryInitialization()
{
	WSADATA wsData;
	WORD lastVersion = MAKEWORD(2, 2);

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


std::string GetHost()
{
	char HOST[N];
	char HostName[1024];

	int checkHost = gethostname(HostName, 1024);

	if (checkHost == SOCKET_ERROR)
		throw std::exception("Unable to get host's name ", checkHost);
	else
	{
		LPHOSTENT lpHost = gethostbyname(HostName);
		if (lpHost == NULL)
			throw std::exception("Unable to get LPHOSTENT ", checkHost);
		else
		{
			strcpy_s(HOST, inet_ntoa(*((in_addr*)lpHost->h_addr_list[0])));
			return  std::string(HOST);
		}
	}
	return 0;
}