#include "handlerequest.h"

HandleRequest::HandleRequest()
{
	clientSocket = -1;
	parser = nullptr;
}

HandleRequest::HandleRequest(char* buffer, const SOCKET& socket)
{
	clientSocket = socket;
	parser = new HttpParser(buffer);
}

HandleRequest::~HandleRequest()
{
	closesocket(clientSocket);
	delete parser;
}

std::unordered_set<std::string> HandleRequest::GetValueSet(const std::string& value)
{
	return (*parser->Get(value)).second;
}

std::string HandleRequest::GetFirstValue(const std::string& value)
{
	return *GetValueSet(value).begin();
}

SOCKET HandleRequest::GetRequestSocket()
{
	return clientSocket;
}

