#ifndef SERVERAPPLICATION_SERVERAPPLICATION_HPP
#define SERVERAPPLICATION_SERVERAPPLICATION_HPP

#include "TcpServer.hpp"

#include <filesystem>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "ProcSignalGuard.hpp"
#include "ServerAppConfig.hpp"

class ServerApplication
{
 public:
	ServerApplication();

	ServerApplication(ServerApplication&&) = delete;
	ServerApplication(const ServerApplication&) = delete;
	ServerApplication operator=(ServerApplication&&) = delete;
	ServerApplication operator=(const ServerApplication&) = delete;

	void Launch(ServerAppConfig&& config);

	void Stop();
 private:
	void StopAllServices();

	void StartWorkersThread(size_t workersCount);

	static void StartLogger(std::filesystem::path pathToLogFile);

	void StartTcpServer(uint16_t port, size_t maxConnections);

	void StartWaitStopSignal();

	std::atomic_bool mStop;
	ProcSignalGuard mSignalGuard;
	TcpServer mServer;
	std::optional<boost::asio::io_context::work> mWork;
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> mLogger;
	std::vector<std::thread> mWorkersGroup;
};

#endif //SERVERAPPLICATION_SERVERAPPLICATION_HPP
