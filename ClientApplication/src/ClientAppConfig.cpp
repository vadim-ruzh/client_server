#include "../include/ClientAppConfig.hpp"

ClientAppConfig GetClientAppConfig(int argc, char* argv[], const ClientAppConfig& defaultSettings)
{
	ClientAppConfig actualConfig = defaultSettings;

	static struct option longOptions[] =
		{
			{ "address", required_argument, nullptr, 'a' },
			{ "port", required_argument, nullptr, 'p' },
			{ "wait-connect", required_argument, nullptr, 'w' },
			{ "reconnects", required_argument, nullptr, 'r' },
			{ "default-config", no_argument, nullptr, 'd' },
			{ "help", no_argument, nullptr, 'h' }
		};

	int optIndex;
	int option;
	while ((option = getopt_long(argc, argv, "a:p:w:r:dh", longOptions, &optIndex)) != EOF)
	{
		switch (option)
		{
		case 'a':
		{
			actualConfig.address = optarg;
			break;
		}
		case 'p':
		{
			actualConfig.port = static_cast<decltype(actualConfig.port)>(std::stoul(optarg));
			break;
		}
		case 'w':
		{
			actualConfig.waitConnection = std::stoul(optarg);
			break;
		}
		case 'r':
		{
			actualConfig.reconnectionAttempts = std::stoul(optarg);
			break;
		}
		case 'd':
		{
			std::cout
				<< "Default Settings  " << "\n"
				<< " Address: " << defaultSettings.address << "\n"
				<< " Port: " << defaultSettings.port << "\n"
				<< " Wait connection : " << defaultSettings.waitConnection << "\n"
				<< " Reconnection attempts : " << defaultSettings.reconnectionAttempts << "\n"
				<< std::endl;
			std::exit(EXIT_SUCCESS);
		}
		case 'h':
		{
			std::cout
				<< "Usage : ClientApplication [options] " << "\n"
				<< " -h, --help" << "\t\t\t" << "Display this help screen" << "\n"
				<< " -d, --default-config" << "\t\t" << "Display default configuration" << "\n"
				<< " -a, --address" << "\t\t\t" << "Set connection address" << "\n"
				<< " -p, --port" << "\t\t\t" << "Set connection port" << "\n"
				<< " -w, --wait-connect" << "\t\t\t" << "Set wait connections timeout" << "\n"
				<< " -r, --reconnects" << "\t\t\t" << "Ыet the number of reconnect attempts" << "\n"
				<< std::endl;
			std::exit(EXIT_SUCCESS);
		}
		default:
		{
			std::cout << "unrecognized option : " << option << "\n"
					  << "Try --help for more information" << "\n"
					  << std::endl;
		}
		}
	}

	return actualConfig;
}
