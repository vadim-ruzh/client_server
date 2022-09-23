#ifndef CLIENTAPPLICATION_HPP_
#define CLIENTAPPLICATION_HPP_

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include "TcpClient.hpp"
#include "ClientAppConfig.hpp"
#include "ProcSignalGuard.hpp"
#include "UserInputProcessor.hpp"
#include "NetworkTrafficProcessor.hpp"

namespace Client
{
	class Application
	{
	 public:
		Application();
		Application(Application&&) = delete;
		Application(const Application&) = delete;
		Application operator=(Application&&) = delete;
		Application operator=(const Application&) = delete;

		void Start(Configuration&& config);
		void Stop();
	 private:
		void StartUserInputProcessing();
		void StartNetworkTrafficProcessing(const Configuration& config);
		void StartAwaitingOfStopSignal();
		void StopAllServices();

		std::atomic_bool mStop;
		ProcSignalGuard mSignalGuard;
		UserInputProcessor mInputProcessor;
		NetworkTrafficProcessor mNetworkProcessor;
		std::thread inputWorker;
		std::thread outputWorker;
	};
}//namespace client

#endif //CLIENTAPPLICATION_HPP_