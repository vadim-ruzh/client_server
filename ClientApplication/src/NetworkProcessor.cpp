#include "NetworkProcessor.hpp"

NetworkProcessorrr::NetworkProcessorrr() : mStop(false)
{

}


void NetworkProcessorrr::StartTcpClient(std::string_view address, std::string_view service)
{
	client.StartConnection(address,service);
}

void NetworkProcessorrr::SendMessage(std::string_view message)
{
	mMutex.lock();
	mBuffer.emplace(message);
	mMutex.unlock();
	mCv.notify_one();
}

void NetworkProcessorrr::RunProcessing()
{
	while (!mStop)
	{
		std::unique_lock<std::mutex> mBufferLock(mMutex);
		mCv.wait(mBufferLock, [this]()
		{ return !mBuffer.empty() || mStop; });

		if (!mBuffer.empty())
		{
			auto currentMessage = mBuffer.front();
			mBuffer.pop();
			mBufferLock.unlock();

			if (!onNewMessage.empty())
			{
				onNewMessage(currentMessage);
			}

			client.SendString(currentMessage);
		}
	}
}

void NetworkProcessorrr::StopProcessing()
{
	if (!mStop.exchange(true))
	{
		client.Disconnect();
		std::cerr << "The network processor has been stopped " << std::endl;
		mCv.notify_all();
	}
}
