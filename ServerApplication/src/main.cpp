#include "../include/ServerApplication.hpp"

int main(int argc, char* argv[])
{
	ServerAppConfig ActualConfiguration = GetServerAppConfig(argc, argv,
	ServerAppConfig // Default configuration
	{
		8787,
		3,
		100,
		"./log/ServerAppLog_%m-%d_%H-%M.log"
	});

	std::cout << "Launching the TcpServer application" << "\n";

	ServerApplication server;
	server.Launch(std::move(ActualConfiguration));

	std::cout << "TcpServer application stopped " << "\n";

	return 0;
}
