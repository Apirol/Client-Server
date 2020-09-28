#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996) 

#include "ServerInitialization.h"


void StartServer(SOCKET server);
void CreateClientSocket(SOCKET& server, SOCKET& clientSocket);
void ReceivingDataFromClient(SOCKET& clientSocket, char* data);
void ProcessingAndSendDAta(SOCKET& clientSocket, char* data);
void ReverseMessage(char* message);
void SendDataToClient(SOCKET clientSock, char* data);
void Shutdown(SOCKET& clientSocket);


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

	clientSocket = accept(server, (sockaddr*)&from, &fromLenght);
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