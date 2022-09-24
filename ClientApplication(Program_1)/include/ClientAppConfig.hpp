#ifndef CLIENTAPPCONFIG_HPP_
#define CLIENTAPPCONFIG_HPP_

#include <cstdio>
#include <cassert>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <filesystem>

namespace Client
{
	/**
 	* @brief The structure describes the configuration settings of the client application
 	*/
	struct Configuration
	{
		std::string host;
		std::string service;
		size_t connectionAwaitingTimeoutMs;
		size_t reconnectionAttempts;
	};

	/**
 	* @brief Gets configuration parameters from arguments
 	*/
	Configuration GetConfiguration(int argc, char* argv[], const Configuration& defaultConfig);

}//namespace Client

#endif //CLIENTAPPCONFIG_HPP_
