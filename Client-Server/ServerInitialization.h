#pragma once
#include "Exception.h"


const int N = 256;


void LibraryInitialization();
void SocketInitialization(SOCKET& server, SOCKADDR_IN& sockAddr);
void CreateSocket(SOCKET& sock);
void InitClientSocket(SOCKADDR_IN& sockAddr, int port);
void BindSocket(SOCKET socket, SOCKADDR_IN sockAddr);
std::string GetHost();


void LibraryInitialization()
{
	WSADATA wsData;
	WORD lastVersion = MAKEWORD(2, 2);

	int lastError = WSAStartup(lastVersion, &wsData);
	if (lastError != 0)
		throw currentException("WSAStartup failed with code: ", lastError);
}

void SocketInitialization(SOCKET& server, SOCKADDR_IN& sockAddr)
{
	CreateSocket(server);
	InitClientSocket(sockAddr, 2002);
	BindSocket(server, sockAddr);
	std::string host = GetHost();

	std::cout << "Server started at " << host << ", port: " << sockAddr.sin_port << std::endl;
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
		throw currentException("Bind socket failed with code: ", bindSocket);
}


std::string GetHost()
{
	char HOST[N];
	char HostName[1024];

	int checkHost = gethostname(HostName, 1024);

	if (checkHost == SOCKET_ERROR)
		throw currentException("Unable to get host's name ", checkHost);
	else
	{
		LPHOSTENT lpHost = gethostbyname(HostName);
		if (lpHost == NULL)
			throw currentException("Unable to get LPHOSTENT ", checkHost);
		else
		{
			strcpy_s(HOST, inet_ntoa(*((in_addr*)lpHost->h_addr_list[0])));
			return  std::string(HOST);
		}
	}
	return 0;
}