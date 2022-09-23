#include "NetworkTrafficProcessor.hpp"

NetworkTrafficProcessor::NetworkTrafficProcessor() : mStop(true)
{

}

void NetworkTrafficProcessor::StartTcpClient(std::string_view address, std::string_view service,size_t reconnectionsAttempts,size_t connectionAwaitTimeout)
{
	if (!mStop.exchange(false))
	{
		return;
	}

	mClient.SetReconnectionsAttempt(reconnectionsAttempts);
	mClient.SetConnectionAwaitingTimeout(std::chrono::milliseconds(connectionAwaitTimeout));
	mClient.StartConnection(address, service);
}

void NetworkTrafficProcessor::SendMessage(std::string_view message)
{
	{
		std::lock_guard<std::mutex> bufferLock(mBufferMutex);
		mBuffer.emplace(message);
	}
	mNewMessageCv.notify_one();
}

void NetworkTrafficProcessor::RunProcessing()
{
	while (!mStop)
	{
		std::unique_lock<std::mutex> bufferLock(mBufferMutex);
		mNewMessageCv.wait(bufferLock, [this]()
		{ return !mBuffer.empty() || mStop; });

		if (!mBuffer.empty())
		{
			auto currentMessage = mBuffer.front();
			mBuffer.pop();
			bufferLock.unlock();

			onNewMessage(currentMessage);
			mClient.SendString(currentMessage);
		}
	}
}

void NetworkTrafficProcessor::StopProcessing()
{
	if (mStop.exchange(true))
	{
		return;
	}

	mClient.Disconnect();
	mNewMessageCv.notify_all();
	std::cerr << "The network processor has been stopped " << std::endl;
}
