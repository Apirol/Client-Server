#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996) 

#include "ServerInitialization.h"
#include <sstream>


void StartServer(SOCKET &server);
void StartListening(SOCKET& server);
void CreateClientSocket(SOCKET& server, SOCKET& clientSocket, SOCKADDR_IN &from);
void ReceiveDataFromClient(SOCKET& clientSocket, std::string &data);
void ProcessingData(SOCKET &clientSock, std::string &data);
void Shutdown(SOCKET& clientSock, std::string data);
void ReverseMessage(std::string &data);
void SendDataToClient(SOCKET &clientSock, std::string data);


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
	std::string data;

	while (true)
	{
		StartListening(server);
		CreateClientSocket(server, clientSocket, from);

		ReceiveDataFromClient(clientSocket, data);
		ProcessingData(clientSocket, data);
		
		SendDataToClient(clientSocket, data);
	}
}	


void StartListening(SOCKET &server) // начинаем прослушивания
{
	int retVal = listen(server, 10);
	if (retVal == SOCKET_ERROR)
		throw currentException("Listening failed with code: ", WSAGetLastError());

	std::cout << "Start listening.." << std::endl;
}


void CreateClientSocket(SOCKET &server, SOCKET &clientSocket, SOCKADDR_IN &from) // создаём новый сокет клиента при подключении
{
	int fromLenght = sizeof(from);

	clientSocket = accept(server, (sockaddr*)&from, &fromLenght);
	if (clientSocket == SOCKET_ERROR)
		throw currentException("Accepting failed with code: ", WSAGetLastError());
	std::cout << "New connection accepted from " << inet_ntoa(from.sin_addr) << htons(from.sin_port) << std::endl;
}


void ReceiveDataFromClient(SOCKET &clientSocket, std::string &data) // Получений данных от клиента
{
	char buffer[N];

	int result = recv(clientSocket, buffer, N, 0);
	if (result == SOCKET_ERROR)
		throw currentException("Receiving failed with code: ", WSAGetLastError());

	buffer[result] = '\0';
	data = buffer;
	std::cout << "Data was recieved" << std::endl;
}


void ProcessingData(SOCKET &clientSock, std::string &data) // Обработка данных
{
	if (data == "stop")
	{
		std::string message = "The server was down";
		Shutdown(clientSock, message);
	}
	else
		ReverseMessage(data);
}


void Shutdown(SOCKET& clientSock, std::string message) // Выключение сервера 
{
	SendDataToClient(clientSock, message);
	throw currentException("The server was down", NULL);
}


void SendDataToClient(SOCKET& clientSock, std::string data) // Отправка преобразованных данных клиенту
{
	int result = send(clientSock, data.c_str(), data.size(), 0);
	if (result == SOCKET_ERROR)
		throw currentException("Sending failed with code: ", WSAGetLastError());
}


inline void ReverseMessage(std::string &data) // Переворачиваем сообщение
{
	std::reverse(data.begin(), data.end());
}