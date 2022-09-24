#ifndef MESSAGE_CLIENT_APP_INCLUDE_OPTIONS_HPP_
#define MESSAGE_CLIENT_APP_INCLUDE_OPTIONS_HPP_

#include <cstdio>
#include <cassert>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <filesystem>

struct ClientAppConfig
{
	// TODO: better naming - ipAddress
	std::string address;
	uint16_t port;
	// CRITICAL: better naming - connectionAwaitingTimeoutMs
	size_t waitConnection;
	size_t reconnectionAttempts;
};

ClientAppConfig GetClientAppConfig(int argc, char* argv[], const ClientAppConfig& defaultSettings);


#endif //MESSAGE_CLIENT_APP_INCLUDE_OPTIONS_HPP_