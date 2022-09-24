#include "ClientApplication.hpp"
#include <iostream>

// TODO: better use anonimous namepsace
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

// TODO: too long line
ClientApplication::ClientApplication() : mStop(true), mSignalGuard({ SIGINT, SIGTERM, SIGQUIT, SIG_BLOCK })
{

}

void ClientApplication::StartUserInputProcessor()
{
	// CRITICAL: check already connected?
	mInputProcessor.onNewMessage.connect([this](std::string message)
	{
		// TODO: reduce nesting via invert if statement: 
		// if (std::none_of(message.begin(), message.end(), isdigit) 
		//		|| message.size() >= 64)
		// return;
	  if (std::all_of(message.begin(), message.end(), isdigit) && message.size() < 64)
	  {
		  std::sort(message.begin(), message.end(), std::less<>());

		  detail::replaceEvenValues(message, "KB");

		  mNetworkProcessor.SendMessage(message);
	  }
	});

	// CRITICAL: check already started?
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
	// CRITICAL: check already connected?
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

	// TODO: customize ip and port?
	mNetworkProcessor.StartTcpClient("127.0.0.1","8787");

	// CRITICAL: check already started?
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
	// TODO: reduce nesting via invert if statement: if(mStop.exchange(false)) { return; }
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
