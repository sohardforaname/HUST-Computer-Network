#pragma once
#include "httpparser.h"
#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

class HandleRequest
{
private:
	SOCKET clientSocket;
	HttpParser* parser;

public:
	HandleRequest();
	HandleRequest(char* buffer, const SOCKET& socket);
	SOCKET GetRequestSocket();
	std::unordered_set<std::string> GetValueSet(const std::string& value);
	std::string GetFirstValue(const std::string& value);
	~HandleRequest();
};