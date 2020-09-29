#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996) 

#include "ServerInitialization.h"


void StartServer(SOCKET &server);
void StartListening(SOCKET& server);
void CreateClientSocket(SOCKET& server, SOCKET& clientSocket, SOCKADDR_IN &from);
void ReceiveDataFromClient(SOCKET& clientSocket, char* data);
void ProcessingData(SOCKET &clientSock, char* data);
void Shutdown(SOCKET& clientSock, char* data);
void ReverseMessage(char* message);
void SendDataToClient(SOCKET &clientSock, char* data);


int main(void)
{
	SOCKET server;
	SOCKADDR_IN sockAddr;
	LPHOSTENT hostEnt;

	try
	{
		LibraryInitialization();
		SocketInitialization(server, sockAddr, hostEnt);

		StartServer(server);
	}
	catch (Exception err)
	{
		std::cout << err.message << err.code << std::endl;
		if (err.code == NULL)
			return 0;

		closesocket(server);
		WSACleanup();
		return -1;
	}
}


void StartServer(SOCKET &server)
{
	SOCKET clientSocket;
	SOCKADDR_IN from;
	char data[N];

	while (true)
	{
		StartListening(server);
		CreateClientSocket(server, clientSocket, from);

		ReceiveDataFromClient(clientSocket, data);
		ProcessingData(clientSocket, data);
		ReverseMessage(data);
		
		SendDataToClient(clientSocket, data);
	}
}	


void StartListening(SOCKET &server)
{
	int retVal = listen(server, 10);
	if (retVal == SOCKET_ERROR)
		throw currentException("Listening failed with code: ", WSAGetLastError());
}


void CreateClientSocket(SOCKET &server, SOCKET &clientSocket, SOCKADDR_IN &from)
{
	int fromLenght = sizeof(from);

	clientSocket = accept(server, (sockaddr*)&from, &fromLenght);
	if (clientSocket == SOCKET_ERROR)
		throw currentException("Listening failed with code: ", WSAGetLastError());
	std::cout << "New connection accepted from " << inet_ntoa(from.sin_addr) << htons(from.sin_port) << std::endl;
}


void ReceiveDataFromClient(SOCKET &clientSocket, char* data)
{
	int retVal = recv(clientSocket, data, N, 0);
	if (retVal == SOCKET_ERROR)
		throw currentException("Receiving failed with code: ", WSAGetLastError());
}


void ProcessingData(SOCKET &clientSock,char* data)
{
	if (strncmp(data, "stop", 4) == 0)
	{
		char message[] = "The server was down";
		Shutdown(clientSock, message);
	}
	else
		ReverseMessage(data);
}


void Shutdown(SOCKET& clientSock, char* data)
{
	SendDataToClient(clientSock, data);
	throw currentException("The server was down", NULL);
}


void ReverseMessage(char* message)
{
	for (int i = 0, j = strlen(message) - 1; i < strlen(message) / 2; i++, j--)
		std::swap(message[i], message[j]);
}


void SendDataToClient(SOCKET& clientSock, char* data)
{
	int retVal = send(clientSock, data, strlen(data), 0);
	if (retVal == SOCKET_ERROR)
		throw currentException("Sending failed with code: ", WSAGetLastError());
}