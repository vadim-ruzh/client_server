#ifndef PROGRAMMOPTIONS_HPP_
#define PROGRAMMOPTIONS_HPP_

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
#endif //PROGRAMMOPTIONS_HPP_
