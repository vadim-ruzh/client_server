#include <iostream>
#include "ServerApplication.hpp"

int main(int argc, char* argv[])
{
	Server::Configuration ActualConfiguration = Server::GetConfiguration(argc, argv,
	Server::Configuration // Default configuration
	{
		8787,
		3,
		100,
		"./log/ServerAppLog_%m-%d_%H-%M.log"
	});

	std::cout << "Launching the TcpServer application" << "\n";

	Server::Application server;
	server.Start(std::move(ActualConfiguration));

	std::cout << "TcpServer application stopped " << "\n";

	return 0;
}
