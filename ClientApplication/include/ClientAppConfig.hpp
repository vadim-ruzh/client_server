#ifndef MESSAGE_CLIENT_APP_INCLUDE_OPTIONS_HPP_
#define MESSAGE_CLIENT_APP_INCLUDE_OPTIONS_HPP_

#include <cstdio>
#include <cassert>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <filesystem>

namespace Client
{
	struct Configuration
	{
		std::string ipAddress;
		std::string service;
		size_t connectionAwaitingTimeoutMs;
		size_t reconnectionAttempts;
	};

	Configuration GetConfiguration(int argc, char* argv[], const Configuration& defaultConfig);

}//namespace Client
#endif //MESSAGE_CLIENT_APP_INCLUDE_OPTIONS_HPP_
