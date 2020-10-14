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

std::unordered_set<std::string> HandleRequest::GetValueSet(const std::string& key)
{
	return (*parser->Get(key)).second;
}

std::string HandleRequest::GetFirstValue(const std::string& key)
{
	return *GetValueSet(key).begin();
}

void HandleRequest::SetValue
(
	const std::string& key, 
	const std::string& newValue, 
	const std::string& oriValue
)
{
	auto& container = (*parser->Get(key)).second;
	container.erase(oriValue);
	container.insert(newValue);
}

SOCKET HandleRequest::GetRequestSocket()
{
	return clientSocket;
}

