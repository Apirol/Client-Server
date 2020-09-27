#pragma comment(lib, "Ws2_32.lib")

#include "Winsock2.h"
#include "SocketHeader.h"
#include "Exception.h"


int main()
{
	try
	{
		Initialization();
		SOCKET server = CreateSocket();
		SOCKADDR_IN sockAddr = InitSocket(2002);
		BindSocket(server, sockAddr);
	}
	catch (Exception err)
	{
		std::cout << err.message << err.code << std::endl;
		return -1;
	}
}