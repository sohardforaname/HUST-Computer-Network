#include "server.h"

void Server::CreateListenSocket()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("wsa init failed\n");
		exit(-1);
	}
	Configuration* configuration = new Configuration;
	configuration->ReadConfigurationFile("/serverconfig");

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	mainMenu = configuration->GetValue("mainmenu");
	printf(
		"Server run at: %s:%d\n\n", 
		configuration->GetValue("ip").c_str(), 
		atoi(configuration->GetValue("port").c_str())
	);

	SOCKADDR_IN sckaddrin;
	memset(&sckaddrin, 0, sizeof(sckaddrin));
	sckaddrin.sin_family = AF_INET;
	sckaddrin.sin_port = htons(atoi(configuration->GetValue("port").c_str()));
	sckaddrin.sin_addr.s_addr = inet_addr(configuration->GetValue("ip").c_str());
	if (bind(serverSocket, (sockaddr*)(&sckaddrin), sizeof(sckaddrin)) < 0)
	{
		printf("bind failed\n");
		closesocket(serverSocket);
		exit(-1);
	}
	if (listen(serverSocket, 10) < 0)
	{
		printf("listen error\n");
		closesocket(serverSocket);
		exit(-1);
	}
	delete configuration;
}

SOCKET Server::Accept()
{
	SOCKADDR_IN clsckaddrin;
	memset(&clsckaddrin, 0, sizeof(clsckaddrin));
	int nameLen = sizeof(clsckaddrin);
	SOCKET clientSocket = accept(serverSocket, (sockaddr*)(&clsckaddrin), &nameLen);
	if (clientSocket < 0)
	{
		closesocket(clientSocket);
		printf("accept error\n");
		return -1;
	}
	printf("client address: %s:%d\n",
		inet_ntoa(clsckaddrin.sin_addr), ntohs(clsckaddrin.sin_port));
	return clientSocket;
}

void Server::Handle(SOCKET clientSocket)
{
	char* buffer = new char[1 << 10];
	memset(buffer, 0, 1 << 10);
	int receiveLength = recv(clientSocket, buffer, 1 << 10, 0);
	if (receiveLength < 0)
	{
		printf("recv error\n");
		delete[]buffer;
		return;
	}
	buffer[receiveLength] = 0;
	printf("%s\n", buffer);
	HandleRequest* request = new HandleRequest(buffer, clientSocket);
	request->SetValue(
		std::string("URI"), 
		mainMenu + request->GetFirstValue("URI"), 
		request->GetFirstValue("URI")
	);
	HandleResponse* response = new HandleResponse(*request);
	delete request;
	delete response;
	delete[]buffer;
}

void CheckShutdownKey(Server* server)
{
	while (!(GetKeyState(VK_END) >> 7))
		;
	server->~Server();
	exit(0);
}

void Server::Loop()
{	
	while (1)
	{
		SOCKET acceptedSocket = Accept();
		if (acceptedSocket < 0)
			continue;
		Handle(acceptedSocket);
	}
}

Server::Server()
{
	std::thread newThread(CheckShutdownKey, this);
	newThread.detach();
	CreateListenSocket();
}

Server::~Server()
{
	printf("Shutdown server\n");
	closesocket(serverSocket);
	WSACleanup();
}