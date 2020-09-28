#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996) 

#include "/Users/redut/source/repos/Client-server/Client-server/Exception.h"
#include "ClientInitialization.h"


void CreateClientSocket(SOCKET& client, SOCKET& clientSocket);
void ReceivingDataFromClient(SOCKET& clientSocket, char* data);
void ProcessingAndSendDAta(SOCKET& clientSocket, char* data);
void ReverseMessage(char* message);
void SendDataToClient(SOCKET clientSock, char* data);
void Shutdown(SOCKET& clientSocket);


int main()
{
	SOCKET client;
	SOCKADDR_IN sockAddr;

	try
	{
		std::string ip;
		std::cout << "¬ведите IP сервера: ";
		std::cin >> ip;

		int port;
		std::cout << "¬ведите порт сервера: ";
		std::cin >> port;

		LibraryInitialization();
		SocketInitialization(client, sockAddr, ip, port);

		std::string message;
		std::cout << "¬ведите сообщение: ";
		std::cin >> message;
		SendMessageToServer(client, message);

		
		
	
		
	}
	catch (Exception err)
	{
		std::cout << err.message << err.code << std::endl;
		closesocket(client);
		WSACleanup();
		return -1;
	}
}



void ConnectToServer(SOCKET &client, LPSOCKADDR_IN serverInfo)
{
	int lastError = connect(client, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
	if (lastError == SOCKET_ERROR)
		throw currentException("Connecting failed with code: ", lastError);
	std::cout << "Connected sucessfully" << std::endl;
}


void SendMessageToServer(SOCKET& client, std::string message)
{
	int lastError = send(client, message.c_str(), message.size(), 0);
	if (lastError == SOCKET_ERROR)
		throw currentException("Sending failed with code: ", lastError);
	std::cout << "Sending sucessfully";
}


void ReceivingAnswer(SOCKET& client, char* answer)
{
	int lastError = recv(client, answer, 256, 0);
	if (lastError == SOCKET_ERROR)
		throw currentException("Receiving failed with code: ", lastError);
	std::cout << "Receiving sucessfully";
}


void ProcessingAndSendDAta(SOCKET& clientSocket, char* data)
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
	char answer[] = "client shutdown";
	SendDataToClient(clientSocket, answer);
	closesocket(clientSocket);
	throw currentException("The client was down ", NULL);
}