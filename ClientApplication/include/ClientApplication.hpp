#ifndef CLIENTAPPLICATION_HPP_
#define CLIENTAPPLICATION_HPP_

#include <string>
#include <boost/asio.hpp>
#include "TcpClient.hpp"
#include "ClientAppConfig.hpp"
#include "ProcSignalGuard.hpp"
#include <iostream>
#include <boost/signals2.hpp>
#include <UserInputProcessor.hpp>
#include <NetworkProcessor.hpp>

class ClientApplication
{
 public:
	ClientApplication();

	void Launch(ClientAppConfig&& config);

	void Stop();
 private:
	void StartUserInputProcessor();
	void StartNetworkProcessor();
	void StartWaitStopSignal();

	void StopAllServices();

	std::atomic_bool mStop;
	ProcSignalGuard mSignalGuard;
	UserInputProcessor mInputProcessor;
	NetworkProcessorrr mNetworkProcessor;
	std::thread input;
	std::thread output;
};

#endif //CLIENTAPPLICATION_HPP_