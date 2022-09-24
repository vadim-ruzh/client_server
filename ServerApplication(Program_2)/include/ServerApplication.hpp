#ifndef SERVERAPPLICATION_HPP_
#define SERVERAPPLICATION_HPP_

#include <filesystem>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include "TcpServer.hpp"
#include "ProcSignalGuard.hpp"
#include "ServerAppConfig.hpp"

namespace Server
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
		void StartLoggingService(std::filesystem::path pathToLogFile);
		void StartNetworkProcessingService(uint16_t port, size_t maxConnections);
		void StartWorkers(size_t workersCount);
		void StartSignalProcessingService();
		void StopAllServices();

		std::atomic_bool mStop;
		ProcSignalGuard mSignalGuard;
		TcpServer mServer;
		std::optional<boost::asio::io_context::work> mWork;
		boost::log::sources::severity_logger<boost::log::trivial::severity_level> mLogger;
		std::vector<std::thread> mWorkersGroup;
	};
}//namespace Server
#endif //SERVERAPPLICATION_HPP_
