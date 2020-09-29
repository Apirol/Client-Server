#pragma once
#pragma once
#include "/Users/redut/source/repos/Client-Server/Client-Server/Exception.h"


const int N = 256;


void LibraryInitialization();
void SocketInitialization(SOCKET& server, SOCKADDR_IN& sockAddr, std::string ip, int port, LPHOSTENT &hostEnt);
void CreateSocket(SOCKET& sock);
void InitClientSocket(SOCKADDR_IN& sockAddr, std::string ip, int port);
void BindSocket(SOCKET socket, SOCKADDR_IN sockAddr);
void GetHost(LPHOSTENT lpHost);


void LibraryInitialization()
{
	WSADATA wsData;
	WORD lastVersion = MAKEWORD(2, 2);

	int lastError = WSAStartup(lastVersion, &wsData);
	if (lastError != 0)
		throw currentException("WSAStartup failed with code: ", lastError);
}


void SocketInitialization(SOCKET& client, SOCKADDR_IN& sockAddr, std::string ip, int port, LPHOSTENT &hostEnt)
{
	CreateSocket(client);
	InitClientSocket(sockAddr, ip, port);
	GetHost(hostEnt);

	std::cout << "Client started at " << hostEnt->h_addr_list[0] << ", port: " << sockAddr.sin_port << std::endl;
}


void CreateSocket(SOCKET& sock)
{
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		throw currentException("Creating socket failed with code: ", sock);;
}


void InitClientSocket(SOCKADDR_IN& sockAddr, std::string ip, int port)
{
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
}


void GetHost(LPHOSTENT hostEnt)
{
	LPHOSTENT lpHost = gethostbyname("localhost");
	if (lpHost == NULL)
		throw currentException("Unable to get LPHOSTENT ", WSAGetLastError());
}