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

std::string HandleRequest::GetFirstValue(const std::string& key)
{
	auto hashSet = parser->Get(key);
	if (!hashSet.size())
		return std::string();
	return *parser->Get(key).begin();
}

void HandleRequest::SetValue
(
	const std::string& key, 
	const std::string& newValue, 
	const std::string& oriValue
)
{
	parser->Update(key, newValue, oriValue);
}

SOCKET HandleRequest::GetRequestSocket()
{
	return clientSocket;
}

