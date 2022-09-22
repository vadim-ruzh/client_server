#include "../include/ServerApplication.hpp"

#include <thread>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>

using namespace boost::log::trivial;

ServerApplication::ServerApplication() :
	mSignalGuard({ SIGINT, SIGTERM, SIGQUIT, SIG_BLOCK }),
	mStop(true)
{}

void ServerApplication::Launch(ServerAppConfig&& config)
{
	if (mStop.exchange(false))
	{
		try
		{
			StartLogger(config.pathToLogFile);

			StartTcpServer(config.port, config.maxConnections);

			StartWorkersThread(config.workersCount);

			StartWaitStopSignal();
		}
		catch (const std::exception& exception)
		{
			BOOST_LOG_SEV(mLogger, fatal) << "The TcpServer application has been stopped : " << exception.what();;
			StopAllServices();
		}
	}
}

void ServerApplication::StartLogger(std::filesystem::path pathToLogFile)
{
	boost::log::add_file_log
		(
			boost::log::keywords::file_name = pathToLogFile,
			boost::log::keywords::format = "%LineID%. [%TimeStamp%] [%Severity%] : %Message%  "
		);

	boost::log::add_common_attributes();
}

void ServerApplication::StartTcpServer(uint16_t port, size_t maxConnections)
{
	mServer.SetOnConnectHandler([this](std::string_view ClientAddress)
	{
	  BOOST_LOG_SEV(mLogger, info) << "Connecting a TcpClient with an address :" << ClientAddress;
	});

	mServer.SetOnDisconnectHandler([this](std::string_view ClientAddress)
	{
	  BOOST_LOG_SEV(mLogger, info) << "Disconnecting a TcpClient with an address :" << ClientAddress;
	});

	mServer.SetOnErrorHandler([this](std::string_view ClientAddress, std::string_view errorMessage)
	{
	  BOOST_LOG_SEV(mLogger, error) << "The TcpClient with address " << ClientAddress
									<< " has an error :" << errorMessage;
	});

	mServer.SetOnRequestHandler([](std::string_view ClientAddress, std::string_view request)
	{
	  if (request.size() >= 2 && (std::stoi(request.data()) % 32 == 0))
	  {
		  std::cout << "Received data from a TcpClient with an address " << ClientAddress
					<< " : " << request << "\n";
	  }
	  else
	  {
		  std::cerr << " Data from the TcpClient with the address " << ClientAddress
					<< " is not consistent" << "\n";
	  }
	});

	mServer.StartAcceptor(port, maxConnections);

	BOOST_LOG_SEV(mLogger, info) << "TcpServer running on port : " << port;
}

void ServerApplication::StartWorkersThread(size_t workersCount)
{
	if (workersCount > 1)
	{
		mWork.emplace(boost::asio::io_context::work(mServer.GetContext()));
	}

	mWorkersGroup.reserve(workersCount);
	for (size_t i = 0; i < workersCount; ++i)
	{
		mWorkersGroup.emplace_back([this]()
			{
			  try
			  {
				  mServer.RunProccesing();
			  }
			  catch (const std::exception& exception)
			  {
				  BOOST_LOG_SEV(mLogger, fatal) << "An error occurred in the worker: " << exception.what();

				  Stop();
			  }
			}
		);
	}

	BOOST_LOG_SEV(mLogger, info) << workersCount << " workers launched";
}

void ServerApplication::StartWaitStopSignal()
{
	mSignalGuard.Wait([this](int signum)
	{
	  mStop = true;

	  BOOST_LOG_SEV(mLogger, fatal) << "The TcpServer application has been stopped with code " << signum;

	  StopAllServices();
	});
}

void ServerApplication::Stop()
{
	if (!mStop.exchange(true))
	{
		kill(getpid(), SIGINT);
	}
}

void ServerApplication::StopAllServices()
{
	mWork.reset();
	mServer.StopProccesing();

	for (std::thread& worker : mWorkersGroup)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}
}

