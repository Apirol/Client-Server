#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996) 

#include "/Users/redut/source/repos/Client-server/Client-server/Exception.h"
#include "ClientInitialization.h"


void ConnectToServer(SOCKET& client, SOCKADDR_IN& serverInfo);
void SendMessageToServer(SOCKET& client, std::string message);
void RecieveAnswer(SOCKET& client, char* answer);


int main()
{
	SOCKET client;
	SOCKADDR_IN sockAddr;
	LPHOSTENT hostEnt;

	try
	{
		std::string ip;
		std::cout << "Enter IP: ";
		std::cin >> ip;

		LibraryInitialization();
		SocketInitialization(client, sockAddr, ip, hostEnt);

		std::string message;
		std::cout << "Enter message: ";
		std::cin >> message;

		ConnectToServer(client, sockAddr);
		SendMessageToServer(client, message);

		char answer[256];
		RecieveAnswer(client, answer);
		std::cout << answer;

		closesocket(client);
		WSACleanup();
	}
	catch (Exception err)
	{
		std::cout << err.message << err.code << std::endl;
		closesocket(client);
		WSACleanup();
		return -1;
	}
}



void ConnectToServer(SOCKET &client, SOCKADDR_IN &serverInfo)
{
	int lastError = connect(client, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
	if (lastError == SOCKET_ERROR)
		throw currentException("Connecting failed with code: ", WSAGetLastError());
	std::cout << "Connected sucessfully" << std::endl;
}


void SendMessageToServer(SOCKET& client, std::string message)
{
	int lastError = send(client, message.c_str(), message.size(), 0);
	if (lastError == SOCKET_ERROR)
		throw currentException("Sending failed with code: ", WSAGetLastError());
	std::cout << "Sending sucessfully" << std::endl;
}


void RecieveAnswer(SOCKET& client, char* answer)
{
	int lastError = recv(client, answer, 256, 0);
	if (lastError == SOCKET_ERROR)
		throw currentException("Receiving failed with code: ", WSAGetLastError());
	std::cout << "Receive sucessfully" << std::endl;
}