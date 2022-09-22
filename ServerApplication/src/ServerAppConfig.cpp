#include "../include/ServerAppConfig.hpp"

ServerAppConfig GetServerAppConfig(int argc, char** argv,const ServerAppConfig& defaultSettings)
{
	ServerAppConfig actualConfig = defaultSettings;

	static struct option longOptions[] =
		{
			{ "port", required_argument, nullptr, 'p' },
			{ "workers", required_argument, nullptr, 'w' },
			{ "max-conn", required_argument, nullptr, 'm' },
			{ "log-path", required_argument, nullptr, 'l' },
			{ "default-config", no_argument, nullptr, 'd' },
			{ "help", no_argument, nullptr, 'h' }
		};

	int optIndex;
	int option;
	while ((option = getopt_long(argc, argv, "p:w:m:l:dh", longOptions, &optIndex)) != EOF)
	{
		switch (option)
		{
		case 'p':
		{
			actualConfig.port = static_cast<decltype(actualConfig.port)>(std::stoul(optarg));
			break;
		}
		case 'w':
		{
			actualConfig.workersCount = std::stoul(optarg);
			assert(("The number of workers must not be less than 1",actualConfig.workersCount <= 0));
			break;
		}
		case 'm':
		{
			actualConfig.maxConnections = std::stoul(optarg);
			assert(("The maximum number of connections should not be less than 1",actualConfig.maxConnections <= 0));
			break;
		}
		case 'l':
		{
			actualConfig.pathToLogFile = optarg;
			break;
		}
		case 'd':
		{
			std::cout
				<< "Default Settings  " << "\n"
				<< " Port: " << defaultSettings.port << "\n"
				<< " Workers count : " << defaultSettings.workersCount << "\n"
				<< " Max connections : " << defaultSettings.maxConnections << "\n"
				<< " Log path : " << defaultSettings.pathToLogFile << "\n"
				<< std::endl;
			std::exit(EXIT_SUCCESS);
		}
		case 'h':
		{
			std::cout
				<< "Usage : ServerApplication [options] " << "\n"
				<< " -h, --help" << "\t\t\t" <<"Display this help screen" << "\n"
				<< " -d, --default-config" << "\t\t" << "Display default configuration" << "\n"
				<< " -p, --port" << "\t\t\t" << "Set port to accept connections" << "\n"
				<< " -w, --workers" << "\t\t\t" << "Set workers thread count" << "\n"
				<< " -m, --max-conn" << "\t\t\t" << "Set max connections count" << "\n"
				<< " -l, --log-path" << "\t\t\t" << "Set path to log file" << "\n"
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
