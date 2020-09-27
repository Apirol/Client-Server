#pragma comment(lib, "Ws2_32.lib")

#include "Winsock2.h"
#include "SocketHandler.h"
#include "Exception.h"


int main()
{
	try
	{
		LibraryInitialization();

		SOCKET server = CreateSocket();
		SOCKADDR_IN sockAddr = InitSocket(2002);

		BindSocket(server, sockAddr);
	}
	catch (Exception err)
	{
		std::cout << err.message << err.code << std::endl;
		WSACleanup();
		return -1;
	}
}

void ReverseMessage(std::string &message)
{
	std::reverse(message.begin(), message.end());
}