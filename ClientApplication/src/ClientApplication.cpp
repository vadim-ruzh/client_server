#include "ClientApplication.hpp"
#include <iostream>

namespace detail
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
}

ClientApplication::ClientApplication() : mStop(true), mSignalGuard({ SIGINT, SIGTERM, SIGQUIT, SIG_BLOCK })
{

}

void ClientApplication::StartUserInputProcessor()
{
	mInputProcessor.onNewMessage.connect([this](std::string message)
	{
	  if (std::all_of(message.begin(), message.end(), isdigit) && message.size() < 64)
	  {
		  std::sort(message.begin(), message.end(), std::less<>());

		  detail::replaceEvenValues(message, "KB");

		  mNetworkProcessor.SendMessage(message);
	  }
	});

	input = std::thread([this]()
	{
	  try
	  {
		  mInputProcessor.RunProcessing();
	  }
	  catch (const std::exception& ex)
	  {
		  std::cerr << "Input processor stopped with error: " << ex.what() << "\n";
		  Stop();
	  }
	});
}

void ClientApplication::StartNetworkProcessor()
{
	mNetworkProcessor.onNewMessage.connect([](std::string& message)
	{
	  std::cout << message << std::endl;

	  int summ = 0;
	  for (const char ch : message)
	  {
		  if (isdigit(ch))
		  {
			  summ += (ch - '0');
		  }
	  }

	  message = std::to_string(summ);
	});

	mNetworkProcessor.StartTcpClient("127.0.0.1","8787");

	output = std::thread([this]()
	{
	  try
	  {
		  mNetworkProcessor.RunProcessing();
	  }
	  catch (const std::exception& ex)
	  {
		  std::cerr << "Input processor stopped with error: " << ex.what() << "\n";
		  Stop();
	  }
	});
}

void ClientApplication::StartWaitStopSignal()
{
	mSignalGuard.Wait([this](int signum)
	{
	  mStop = true;

	  StopAllServices();

	  std::cerr << "Client application stopped with code: " << signum << "\n";
	});
}

void ClientApplication::StopAllServices()
{
	mInputProcessor.StopProcessing();
	if (input.joinable())
	{
		input.join();
	}

	mNetworkProcessor.StopProcessing();
	if (output.joinable())
	{
		output.join();
	}
}

void ClientApplication::Launch(ClientAppConfig&& config)
{
	if (mStop.exchange(false))
	{
		try
		{
			StartUserInputProcessor();

			StartNetworkProcessor();

			StartWaitStopSignal();
		}
		catch (const std::exception& exception)
		{
			std::cerr << "Client application stopped with error: " << exception.what() << std::endl;
			StopAllServices();
		}
	}
}

void ClientApplication::Stop()
{
	if (!mStop.exchange(true))
	{
		kill(getpid(), SIGINT);
	}
}
