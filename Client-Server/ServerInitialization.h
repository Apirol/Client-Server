#pragma once
#include "Exception.h"


void LibraryInitialization();
void SocketInitialization(SOCKET& server, SOCKADDR_IN& sockAddr, LPHOSTENT &hostEnt);
void CreateSocket(SOCKET& sock);
void InitClientSocket(SOCKADDR_IN& sockAddr, int port);
void BindSocket(SOCKET socket, SOCKADDR_IN sockAddr);
void GetHost(LPHOSTENT &lpHost);


const int N = 256;


void LibraryInitialization()
{
	WSADATA wsData;
	WORD lastVersion = MAKEWORD(2, 2);

	int lastError = WSAStartup(lastVersion, &wsData);
	if (lastError != 0)
		throw currentException("WSAStartup failed with code: ", lastError);
}

void SocketInitialization(SOCKET& server, SOCKADDR_IN& sockAddr, LPHOSTENT &hostEnt)
{
	CreateSocket(server);
	InitClientSocket(sockAddr, 2002);
	BindSocket(server, sockAddr);
	GetHost(hostEnt);

	std::cout << "Server started at " << inet_ntoa(sockAddr.sin_addr) << ", port: " << sockAddr.sin_port << std::endl;
}

void CreateSocket(SOCKET& sock)
{
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		throw currentException("Creating socket failed with code: ", sock);;
}


void InitClientSocket(SOCKADDR_IN& sockAddr, int port)
{
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_addr.s_addr = INADDR_ANY;
}


void BindSocket(SOCKET socket, SOCKADDR_IN sockAddr)
{
	int bindSocket = bind(socket, (LPSOCKADDR)&sockAddr, sizeof(sockAddr));
	if (bindSocket == SOCKET_ERROR)
		throw currentException("Bind socket failed with code: ", WSAGetLastError());
}


void GetHost(LPHOSTENT &hostEnt)
{
	LPHOSTENT lpHost = gethostbyname("localhost");
	if (lpHost == NULL)
		throw currentException("Unable to get LPHOSTENT ", WSAGetLastError());
}