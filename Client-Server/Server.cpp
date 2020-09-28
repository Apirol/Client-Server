#pragma comment(lib, "Ws2_32.lib")

#include "Winsock2.h"
#include "SocketController.h"
#include "Exception.h"


int main()
{
	try
	{
		LibraryInitialization();

		SOCKET server = CreateSocket();
		SOCKADDR_IN sockAddr = InitSocket(2002);

		BindSocket(server, sockAddr);
		std::string host = GetHost();
		std::cout << "Server starter at " << host << ", port: " << sockAddr.sin_port << std::endl;
	}
	catch (Exception err)
	{
		std::cout << err.message << err.code << std::endl;
		WSACleanup();
		return -1;
	}
}


void StartServer()
{

}


void ReverseMessage(std::string &message)
{
	std::reverse(message.begin(), message.end());
}