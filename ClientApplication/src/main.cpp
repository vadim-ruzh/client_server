#include <iostream>

#include "ClientApplication.hpp"

int main(int argc, char* argv[])
{
	ClientAppConfig ActualConfiguration = GetClientAppConfig(argc, argv,
		ClientAppConfig // Default configuration
		{
			"127.0.0.1",
			8787,
			1,
			5
		});

	std::cout << "Launching the TcpClient application" << "\n";

	ClientApplication client;
	client.Launch(std::move(ActualConfiguration));

	std::cout << "TcpClient application stopped " << "\n";

	return 0;
}