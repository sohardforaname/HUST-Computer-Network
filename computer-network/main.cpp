#include "server.h"
int main()
{
	Server* server = new Server;
	server->Loop();
	delete server;
	return 0;
}