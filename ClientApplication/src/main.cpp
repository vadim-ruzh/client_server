#include <iostream>
#include "ClientApplication.hpp"

int main(int argc, char* argv[])
{
	Client::Configuration ActualConfiguration = Client::GetConfiguration(argc, argv,
	Client::Configuration // Default configuration
	{
		"127.0.0.1",
		"8787",
		100,
		100
	});

	std::cout << "Launching the TcpClient application" << "\n";

	Client::Application client;
	client.Start(std::move(ActualConfiguration));

	std::cout << "TcpClient application stopped " << "\n";

	return 0;
}