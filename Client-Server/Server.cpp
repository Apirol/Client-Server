#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996) 

#include "ServerInitialization.h"


void StartServer(SOCKET &server);
void CreateClientSocket(SOCKET& server, SOCKET& clientSocket);
void ReceiveDataFromClient(SOCKET& clientSocket, char* data);
int ProcessingData(char* data);
void ReverseMessage(char* message);
void SendDataToClient(SOCKET clientSock, char* data);


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
		closesocket(server);
		WSACleanup();
		return -1;
	}
}


void StartServer(SOCKET &server)
{
	SOCKET clientSocket;
	char data[N];

	while (true)
	{
		StartListening(server);
		CreateClientSocket(server, clientSocket);

		ReceiveDataFromClient(clientSocket, data);
		ProcessingData(data);
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


void CreateClientSocket(SOCKET &server, SOCKET &clientSocket)
{
	SOCKADDR_IN from;
	int fromLenght = sizeof(from);

	clientSocket = accept(server, (sockaddr*)&from, &fromLenght);
	if (clientSocket == SOCKET_ERROR)
		throw currentException("Listening failed with code: ", clientSocket);
}


void ReceiveDataFromClient(SOCKET &clientSocket, char* data)
{
	int retVal = recv(clientSocket, data, N, 0);
	if (retVal == SOCKET_ERROR)
		throw currentException("Receiving failed with code: ", retVal);
}


int ProcessingData(char* data)
{
	if (data[0] == 'S' && data[1] == '/n')
		throw currentException("The server was down: ", NULL);

	return 0;
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