#ifndef SERVERAPPCONFIG_HPP_
#define SERVERAPPCONFIG_HPP_

#include <cstdio>
#include <cassert>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <filesystem>

namespace Server
{
	struct Configuration
	{
		uint16_t port;
		size_t workersCount;
		size_t maxConnections;
		std::string pathToLogFile;
	};

	Configuration GetConfiguration(int argc, char* argv[],Configuration&& defaultConfiguration);
}
#endif //SERVERAPPCONFIG_HPP_
