#ifndef PROGRAMMOPTIONS_HPP_
#define PROGRAMMOPTIONS_HPP_

#include <cstdio>
#include <cassert>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <filesystem>

struct ServerAppConfig
{
	uint16_t port;
	size_t workersCount;
	size_t maxConnections;
	std::string pathToLogFile;
};

ServerAppConfig GetServerAppConfig(int argc, char** argv, const ServerAppConfig& defaultSettings);


#endif //PROGRAMMOPTIONS_HPP_
