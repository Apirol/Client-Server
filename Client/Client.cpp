#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996) 

#include "/Users/redut/source/repos/Client-server/Client-server/Exception.h"
#include "ClientInitialization.h"
#include <string>


void ConnectToServer(SOCKET& client, SOCKADDR_IN& serverInfo);
void SendMessageToServer(SOCKET& client, std::string message);
void RecieveAnswer(SOCKET& client, std::string &answer);


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
		std::cin.ignore();

		LibraryInitialization();
		SocketInitialization(client, sockAddr, ip, hostEnt);

		std::string message;
		std::cout << "Enter message: ";
		std::getline(std::cin, message);

		ConnectToServer(client, sockAddr);
		SendMessageToServer(client, message);

		std::string answer;
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



void ConnectToServer(SOCKET &client, SOCKADDR_IN &serverInfo) // Устанавливаем соединения с сервером
{
	int lastError = connect(client, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
	if (lastError == SOCKET_ERROR)
		throw currentException("Connecting failed with code: ", WSAGetLastError());
	std::cout << "Connected sucessfully" << std::endl;
}


void SendMessageToServer(SOCKET& client, std::string message) // Отправляем сообщение серверу
{
	int result = send(client, message.c_str(), message.size(), 0);
	if (result == SOCKET_ERROR)
		throw currentException("Sending failed with code: ", WSAGetLastError());
	std::cout << "Sending sucessfully" << std::endl;
}


void RecieveAnswer(SOCKET& client, std::string &answer) // Получаем ответ
{
	char buffer[N];

	int result = recv(client, buffer, N, 0);
	if (result == SOCKET_ERROR)
		throw currentException("Receiving failed with code: ", WSAGetLastError());

	buffer[result] = '\0'; // Добавляем символ конца строки
	answer = buffer; 
	std::cout << "Receive sucessfully" << std::endl;
}