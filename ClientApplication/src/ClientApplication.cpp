#include "ClientApplication.hpp"
#include <iostream>

namespace
{
	void replaceEvenValues(std::string& str, const char* newValue)
	{
		std::string tmp;

		for (const char ch : str)
		{
			if (ch % 2 == 0)
				tmp.append(newValue);
			else
				tmp.push_back(ch);
		}

		str = std::move(tmp);
	}

	int32_t SumOfAllNumbersInString(std::string_view str)
	{
		int32_t summ = 0;
		for (const char ch : str)
		{
			if (isdigit(ch))
			{
				summ += (ch - '0');
			}
		}
		return summ;
	}

}

namespace Client
{

	Application::Application() :
		mStop(true),
		mSignalGuard({ SIGINT, SIGTERM, SIGQUIT, SIG_BLOCK })
	{

	}

	void Application::StartUserInputProcessing()
	{
		mInputProcessor.onNewMessage.connect([this](std::string message)
		{
		  if (std::all_of(message.begin(), message.end(), isdigit) && message.size() < 64)
		  {
			  std::sort(message.begin(), message.end(), std::less<>());

			  replaceEvenValues(message, "KB");

			  mNetworkProcessor.SendMessage(message);
		  }
		});

		inputWorker = std::thread([this]()
		{
		  try
		  {
			  mInputProcessor.RunProcessing();
		  }
		  catch (const std::exception& exception)
		  {
			  std::cerr << "Input processor stopped with error: " << exception.what() << "\n";
			  Stop();
		  }
		});
	}

	void Application::StartNetworkTrafficProcessing(const Configuration& config)
	{
		mNetworkProcessor.onNewMessage.connect([](std::string& message)
		{
		  std::cout << message << std::endl;

		  message = std::to_string(SumOfAllNumbersInString(message));
		});

		mNetworkProcessor.StartTcpClient(
			config.ipAddress, config.service,
			config.reconnectionAttempts, config.connectionAwaitingTimeoutMs);

		outputWorker = std::thread([this]()
		{
		  try
		  {
			  mNetworkProcessor.RunProcessing();
		  }
		  catch (const std::exception& exception)
		  {
			  std::cerr << "Input processor stopped with error: " << exception.what() << "\n";
			  Stop();
		  }
		});
	}

	void Application::StartAwaitingOfStopSignal()
	{
		mSignalGuard.Wait([this](int signum)
		{
		  mStop = true;

		  StopAllServices();

		  std::cerr << "Client application stopped with code: " << signum << "\n";
		});
	}

	void Application::StopAllServices()
	{
		mInputProcessor.StopProcessing();
		if (inputWorker.joinable())
		{
			inputWorker.join();
		}

		mNetworkProcessor.StopProcessing();
		if (outputWorker.joinable())
		{
			outputWorker.join();
		}
	}

	void Application::Start(Configuration&& config)
	{
		if (!mStop.exchange(false))
		{
			return;
		}

		try
		{
			StartUserInputProcessing();

			StartNetworkTrafficProcessing(config);

			StartAwaitingOfStopSignal();
		}
		catch (const std::exception& exception)
		{
			std::cerr << "Client application stopped with error: " << exception.what() << std::endl;
			StopAllServices();
		}

	}

	void Application::Stop()
	{
		if (!mStop.exchange(true))
		{
			kill(getpid(), SIGINT);
		}
	}
}
