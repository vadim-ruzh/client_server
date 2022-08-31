#include "ClientApplication.hpp"
#include <iostream>

ClientApplication::ClientApplication(): mWork(false)
{

}

void ClientApplication::Start(const std::string& address, const std::string& service)
{
	if (!mWork.exchange(true))
	{
		std::thread input([this]()
		{ startUserInput(); });
		std::thread output([this, &address, &service]()
		{ startNetworkOutput(address, service); });

		input.join();
		output.join();
	}
}

void ClientApplication::Stop()
{
	if (mWork.exchange(false))
	{
		mCv.notify_one(); //notify the output stream that the work is completed
		std::cerr << "Application stopped" << std::endl;
		std::exit(EXIT_SUCCESS);
	}
}

void ClientApplication::startNetworkOutput(const std::string& address, const std::string& service)
{
	try
	{
		startNetworkOutputLoop(address, service);
	}
	catch (const boost::exception& systemError)
	{
		std::string str;
		std::cerr << "the output to the network was interrupted: :"  << std::endl;
		Stop();
	}
}

void ClientApplication::startNetworkOutputLoop(const std::string& address, const std::string& service)
{
	MyClient myClient;
	myClient.ConnectTo(address, service);

	while (mWork)
	{
		std::unique_lock<std::mutex> mBufferLock(mMutex);
		mCv.wait(mBufferLock, [this]()
		{ return !mBuffer.empty() || !mWork; });

		if (!mBuffer.empty())
		{
			auto currentMessage = mBuffer.front();
			mBuffer.pop();
			mBufferLock.unlock();

			myClient.SendString(currentMessage);
		}
	}

}

void ClientApplication::startUserInput()
{
	try
	{
		userInputLoop();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "user input was interrupted: " << ex.what() << std::endl;
		Stop();
	}
}

void ClientApplication::userInputLoop()
{
	while (mWork)
	{
		std::string line;
		std::getline(std::cin, line);

		std::unique_lock<std::mutex> mBufferLock(mMutex);
		mBuffer.push(line);
		mBufferLock.unlock();

		mCv.notify_one();
	}
}
