#include "ServerAppConfig.hpp"

namespace Server
{
	Configuration GetConfiguration(int argc, char* argv[], Configuration&& defaultSettings)
	{
		Configuration resultConfig = defaultSettings;

		static struct option possibleOptions[] =
			{
				{ "port", required_argument, nullptr, 'p' },
				{ "workers", required_argument, nullptr, 'w' },
				{ "max-conn", required_argument, nullptr, 'm' },
				{ "log-path", required_argument, nullptr, 'l' },
				{ "default-config", no_argument, nullptr, 'd' },
				{ "help", no_argument, nullptr, 'h' }
			};

		int optIndex;
		int selectedOption;
		while ((selectedOption = getopt_long(argc, argv, "p:w:m:l:dh", possibleOptions, &optIndex)) != EOF)
		{
			switch (selectedOption)
			{
			case 'p':
			{
				resultConfig.port = static_cast<decltype(resultConfig.port)>(std::stoul(optarg));
				break;
			}
			case 'w':
			{
				resultConfig.workersCount = std::stoul(optarg);
				assert(("The number of workers must not be less than 1", resultConfig.workersCount < 1));
				break;
			}
			case 'm':
			{
				resultConfig.maxConnections = std::stoul(optarg);
				assert(("The maximum number of connections should not be less than 1", resultConfig.maxConnections < 1));
				break;
			}
			case 'l':
			{
				resultConfig.pathToLogFile = optarg;
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
					<< "Usage : Application [options] " << "\n"
					<< " -h, --help" << "\t\t\t" << "Display this help screen" << "\n"
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
				std::cout << "unrecognized option : " << selectedOption << "\n"
						  << "Try --help for more information" << "\n"
						  << std::endl;
			}
			}
		}

		return resultConfig;
	}
}//namespace Server
