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

namespace Server
{

	Application::Application() :
		mSignalGuard({ SIGINT, SIGTERM, SIGQUIT, SIG_BLOCK }),
		mStop(true)
	{
	}

	void Application::Start(Configuration&& config)
	{
		if (!mStop.exchange(false))
		{
			throw std::runtime_error("Server application already started");
		}

		try
		{
			StartLoggingService(config.pathToLogFile);
			StartNetworkProcessingService(config.port, config.maxConnections);
			StartWorkers(config.workersCount);
			StartSignalProcessingService();
		}
		catch (const std::exception& exception)
		{
			BOOST_LOG_SEV(mLogger, fatal) << "The TcpServer application has been stopped : " << exception.what();;
			StopAllServices();
		}

	}

	void Application::StartLoggingService(std::filesystem::path pathToLogFile)
	{
		boost::log::add_file_log
			(
				boost::log::keywords::file_name = pathToLogFile,
				boost::log::keywords::format = "%LineID%. [%TimeStamp%] [%Severity%] : %Packet%  "
			);

		boost::log::add_common_attributes();
	}

	void Application::StartNetworkProcessingService(uint16_t port, size_t maxConnections)
	{
		mServer.SetOnConnectHandler([this](std::string_view ClientAddress)
		{
		  BOOST_LOG_SEV(mLogger, info) << "Connecting a TcpClient with an ipAddress :" << ClientAddress;
		});

		mServer.SetOnDisconnectHandler([this](std::string_view ClientAddress)
		{
		  BOOST_LOG_SEV(mLogger, info) << "Disconnecting a TcpClient with an ipAddress :" << ClientAddress;
		});

		mServer.SetOnErrorHandler([this](std::string_view ClientAddress, std::string_view errorMessage)
		{
		  BOOST_LOG_SEV(mLogger, error) << "The TcpClient with ipAddress " << ClientAddress
										<< " has an error :" << errorMessage;
		});

		mServer.SetOnRequestHandler([](std::string_view ClientAddress, std::string_view message)
		{
		  if (message.size() >= 2 && (std::stoi(message.data()) % 32 == 0))
		  {
			  std::cout << "Received message from a TcpClient with an ipAddress " << ClientAddress
						<< " : " << message << "\n";
		  }
		  else
		  {
			  std::cerr << "Message from the TcpClient with the ipAddress " << ClientAddress
						<< " is not consistent" << "\n";
		  }
		});

		mServer.StartAcceptor(port, maxConnections);

		BOOST_LOG_SEV(mLogger, info) << "TcpServer running on service : " << port;
	}

	void Application::StartWorkers(size_t workersCount)
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
				  mServer.StartProcessingConnections();
			  }
			  catch (const std::exception& exception)
			  {
				  BOOST_LOG_SEV(mLogger, fatal) << "An error occurred in the worker: " << exception.what();
				  Stop();
			  }
			});
		}

		BOOST_LOG_SEV(mLogger, info) << workersCount << " workers launched";
	}

	void Application::StartSignalProcessingService()
	{
		mSignalGuard.Wait([this](int signum)
		{
			BOOST_LOG_SEV(mLogger, fatal) << "The TcpServer application has been stopped with code " << signum;

			StopAllServices();
		});
	}

	void Application::Stop()
	{
		if (!mStop.exchange(true))
		{
			kill(getpid(), SIGINT);
		}
	}

	void Application::StopAllServices()
	{
		mStop = true;
		mWork.reset();
		mServer.StopAcceptor();

		for (std::thread& worker : mWorkersGroup)
		{
			if (worker.joinable())
			{
				worker.join();
			}
		}
	}
}//namespace Server
