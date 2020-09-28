#pragma once
#include <iostream>
#include "Winsock2.h"



struct Exception {
	std::string message;
	int code;
};


Exception currentException(std::string message, int code);


Exception currentException(std::string message, int code)
{
	Exception currentException;

	currentException.code = code;
	currentException.message = message;

	return currentException;
}