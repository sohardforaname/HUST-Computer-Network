#pragma once
#include "HandleResponse.h"
#include "Configuration.h"
#include "Log.h"
#include <Windows.h>
#include <thread>

class Server
{
private:
	SOCKET serverSocket;
	char* buffer;
	std::string mainMenu;

private:
	void CreateListenSocket();
	SOCKET Accept();
	void Handle(SOCKET clientSocket);

public:
	Server();
	~Server();
	void Loop();
};