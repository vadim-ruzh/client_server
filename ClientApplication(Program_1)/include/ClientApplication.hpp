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
	/**
 	* @brief This class takes user input and sends it to the remote host.
 	*/
	class Application
	{
	 public:
		Application();

		Application(Application&&) = delete;
		Application(const Application&) = delete;
		Application operator=(Application&&) = delete;
		Application operator=(const Application&) = delete;

		/**
 		* @brief All services necessary for the application to work are started.
		* @details If an error occurs during the start of any of the services, the application will be stopped.
		* @remark If the application is already running, an exception is thrown.
	 	* @throw std::runtime_error
 		*/
		void Start(Configuration&& config) noexcept(false);

		/**
 		* @brief Send a signal to the signal processing service to stop the application.
		* @remark Trying to stop the application again will not be considered an error.
		* @note For more information about processing signals,see StartSignalProcessingService method.
 		*/
		void Stop() noexcept;
	 private:
		/**
		* @brief Setting up and launching a user input processing service.
	 	* @details The user input handler is being set.
		* @details In a separate thread, the processing of user input is started.
		*/
		void StartUserInputProcessingService();

		/**
		* @brief Setting up and launching a network traffic processing service.
	 	* @details Message handler is set.
		* @details Connecting to a remote host.
		* @remark Sending received messages is started in a separate thread.
		* @throw boost::system::error_code
		*/
		void StartNetworkTrafficProcessingService(const Configuration& config);

		/**
		* @brief Waiting for a signal to stop the program.
		* @details When a signal is received, all application services are stopped.
		* @note For more information on stop services, see the StopAllServices method.
		*/
		void StartSignalProcessingService() noexcept;

		/**
		* @brief Stops all running services.
	 	* @details Stop flag is being set
	 	* @details there is a stop of mInputProcessor and mNetworkProcessor and waiting for threads processing them.
		*/
		void StopAllServices() noexcept;

		std::atomic_bool mStop;
		ProcSignalGuard mSignalGuard;
		UserInputProcessor mInputProcessor;
		NetworkTrafficProcessor mNetworkProcessor;
		std::thread inputWorker;
		std::thread outputWorker;
	};
}//namespace client

#endif //CLIENTAPPLICATION_HPP_