#pragma once
#include <iostream>



struct Exception {
	std::string message;
	int code;
};


Exception currentException(std::string message, int code);


Exception currentException(std::string message, int code)
{
	Exception currenException;

	currenException.code = code;
	currenException.message = message;

	return currenException;
}