#pragma once


#include "Winsock2.h"
#include <iostream>


void Initialization()
{
	WSADATA wsData;
	WORD lastVersion = MAKEWORD(2, 2);

	int lastError = WSAStartup(lastVersion, &wsData);
	if (lastError != 0)
	{
		std::cout << "WSAStartup() failed" << std::endl;
		WSACleanup();
		exit(-1);
	}
}


void InitSocket()
{

}


void InitSocket(SOCKET socketToInit, int port)
{
	if (socketToInit == INVALID_SOCKET)
	{
		std::cout << "INVALID SOCKET error" << std::endl;
		WSACleanup();
		exit(-1);
	}
	
}

void checkForInvalidSocket()
{

}

