#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996) 

#include <iostream>
#include "Exception.h"

const int N = 256;

int main()
{
	SOCKET server;
	SOCKADDR_IN sockAddr;
	try
	{
		LibraryInitialization();
		SocketInitialization(server, sockAddr);

		StartServer(server);
	}
	catch (Exception err)
	{
		std::cout << err.message << err.code << std::endl;
		closesocket(server);
		WSACleanup();
		return -1;
	}
}


void LibraryInitialization()
{
	WSADATA wsData;
	WORD lastVersion = MAKEWORD(2, 2);

	int lastError = WSAStartup(lastVersion, &wsData);
	if (lastError != 0)
		throw currentException("WSAStartup failed with code: ", lastError);
}

void SocketInitialization(SOCKET server, SOCKADDR_IN sockAddr)
{
	CreateSocket(server);
	InitSocket(sockAddr, 2002);
	BindSocket(server, sockAddr);
	std::string host = GetHost();

	std::cout << "Server starter at " << host << ", port: " << sockAddr.sin_port << std::endl;
}

void CreateSocket(SOCKET &sock)
{
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		throw currentException("Creating socket failed with code: ", sock);;
}


void InitSocket(SOCKADDR_IN &sockAddr, int port)
{
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = port;
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



void StartServer(SOCKET server)
{
	SOCKET clientSocket;
	char data[N];

	while (true)
	{
		int retVal = listen(server, 10);
		if (retVal == SOCKET_ERROR)
			throw currentException("Listening failed with code: ", retVal);

		CreateClientSocket(server, clientSocket);
		ReceivingDataFromClient(clientSocket, data);
		ProcessingAndSendDAta(clientSocket, data);
	}
}	


void CreateClientSocket(SOCKET &server, SOCKET &clientSocket)
{
	SOCKADDR_IN from;
	int fromLenght = sizeof(from);

	SOCKET clientSocket = accept(server, (sockaddr*)&from, &fromLenght);
	if (clientSocket == SOCKET_ERROR)
		throw currentException("Listening failed with code: ", clientSocket);
}


void ReceivingDataFromClient(SOCKET &clientSocket, char* data)
{
	int retVal = recv(clientSocket, data, N, 0);
	if (retVal == SOCKET_ERROR)
		throw currentException("Receiving failed with code: ", retVal);
}


void ProcessingAndSendDAta(SOCKET &clientSocket, char* data)
{
	if (data[0] != 'S' && data[1] != '/n')
	{
		ReverseMessage(data);
		SendDataToClient(clientSocket, data);
	}
	else
		Shutdown(clientSocket);
}


void ReverseMessage(char* message)
{
	for (int i = 0, j = strlen(message) - 1; i < strlen(message) / 2; i++, j--)
		std::swap(message[i], message[j]);
}


void SendDataToClient(SOCKET clientSock, char* data)
{
	int retVal = send(clientSock, data, N, 0);
	if (retVal == SOCKET_ERROR)
		throw currentException("Sending failed with code: ", retVal);
}


void Shutdown(SOCKET& clientSocket)
{
	char answer[] = "Server shutdown";
	SendDataToClient(clientSocket, answer);
	closesocket(clientSocket);
	throw currentException("The server was down ", NULL);
}