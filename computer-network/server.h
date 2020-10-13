#pragma once
#include "handleresponse.h"
#include "configuration.h"

class Server
{
private:
	SOCKET serverSocket;
	char* buffer;
	std::string GetTypeFromFilePath(const char* filePath);

private:
	void CreateListenSocket();
	SOCKET Accept();
	void Handle(SOCKET clientSocket);

public:
	Server();
	~Server();
	void Loop();
};