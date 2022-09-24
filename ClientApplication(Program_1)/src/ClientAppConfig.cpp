#include "ClientAppConfig.hpp"

namespace Client
{
	Configuration GetConfiguration(int argc, char* argv[], const Configuration& defaultConfig)
	{
		Configuration resultConfig = defaultConfig;

		static struct option possibleOptions[] =
			{
				{ "host", required_argument, nullptr, 'H' },
				{ "service", required_argument, nullptr, 's' },
				{ "wait-connect", required_argument, nullptr, 'w' },
				{ "reconnects", required_argument, nullptr, 'r' },
				{ "default-config", no_argument, nullptr, 'd' },
				{ "help", no_argument, nullptr, 'h' }
			};

		int optIndex;
		int selectedOption;
		while ((selectedOption = getopt_long(argc, argv, "a:s:w:r:dh", possibleOptions, &optIndex)) != EOF)
		{
			switch (selectedOption)
			{
			case 'H':
			{
				resultConfig.host = optarg;
				break;
			}
			case 's':
			{
				resultConfig.service = optarg;
				break;
			}
			case 'w':
			{
				resultConfig.connectionAwaitingTimeoutMs = std::stoul(optarg);
				break;
			}
			case 'r':
			{
				resultConfig.reconnectionAttempts = std::stoul(optarg);
				break;
			}
			case 'd':
			{
				std::cout
					<< "Default Settings  " << "\n"
					<< " Host: " << defaultConfig.host << "\n"
					<< " Service: " << defaultConfig.service << "\n"
					<< " Connection awaiting timeout : " << defaultConfig.connectionAwaitingTimeoutMs << "ms" << "\n"
					<< " Reconnection attempts : " << defaultConfig.reconnectionAttempts << "\n"
					<< std::endl;
				std::exit(EXIT_SUCCESS);
			}
			case 'h':
			{
				std::cout
					<< "Usage : ClientApplication(Program_1) [options] " << "\n"
					<< " -h, --help" << "\t\t\t" << "Display this help screen" << "\n"
					<< " -d, --default-config" << "\t\t" << "Display default configuration" << "\n"
					<< " -h, --Host" << "\t\t\t" << "Connection url or ip address" << "\n"
					<< " -s, --service" << "\t\t\t" << "Connection service or port" << "\n"
					<< " -w, --wait-connect" << "\t\t\t" << "Connection await timeout in ms" << "\n"
					<< " -r, --reconnects" << "\t\t\t" << "Number of reconnection attempts" << "\n"
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
}

