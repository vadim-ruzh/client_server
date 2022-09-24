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
	Application::Application() :mStop(true),
		mSignalGuard({ SIGINT, SIGTERM, SIGQUIT, SIG_BLOCK })
	{

	}

	void Application::StartUserInputProcessingService()
	{
		mInputProcessor.onNewMessage.connect([this](std::string message)
		{
		  if (std::all_of(message.begin(), message.end(), isdigit) && message.size() < 64)
		  {
			  std::sort(message.begin(), message.end(), std::less<>());

			  replaceEvenValues(message, "KB");

			  mNetworkProcessor.PostToSend(message);
		  }
		});

		inputWorker = std::thread([this]()
		{
		  try
		  {
			  mInputProcessor.StartReadingInput();
		  }
		  catch (const std::exception& exception)
		  {
			  std::cerr << "Input processor stopped with error: " << exception.what() << "\n";
			  Stop();
		  }
		});
	}

	void Application::StartNetworkTrafficProcessingService(const Configuration& config)
	{
		mNetworkProcessor.onNewMessage.connect([](std::string& message)
		{
		  std::cout << message << std::endl;

		  message = std::to_string(SumOfAllNumbersInString(message));
		});

		mNetworkProcessor.StartConnection(
			config.host, config.service,
			config.reconnectionAttempts, config.connectionAwaitingTimeoutMs);

		outputWorker = std::thread([this]()
		{
		  try
		  {
			  mNetworkProcessor.StartSending();
		  }
		  catch (const std::exception& exception)
		  {
			  std::cerr << "Input processor stopped with error: " << exception.what() << "\n";
			  Stop();
		  }
		});
	}

	void Application::StartSignalProcessingService() noexcept
	{
		mSignalGuard.Wait([this](int signum)
		{
		  StopAllServices();

		  std::cerr << "Client application stopped with code: " << signum << "\n";
		});
	}

	void Application::StopAllServices() noexcept
	{
		mStop = true;

		mInputProcessor.StopReadingInput();
		if (inputWorker.joinable())
		{
			inputWorker.join();
		}

		mNetworkProcessor.StopConnection();
		if (outputWorker.joinable())
		{
			outputWorker.join();
		}
	}

	void Application::Start(Configuration&& config) noexcept(false)
	{
		if (!mStop.exchange(false))
		{
			throw std::runtime_error("Client application already started");
		}

		try
		{
			StartUserInputProcessingService();
			StartNetworkTrafficProcessingService(config);
			StartSignalProcessingService();
		}
		catch (const std::exception& exception)
		{
			std::cerr << "Client application stopped with error: " << exception.what() << std::endl;
			StopAllServices();
		}

	}

	void Application::Stop() noexcept
	{
		if (!mStop.exchange(true))
		{
			kill(getpid(), SIGINT);
		}
	}
}
