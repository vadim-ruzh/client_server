#include "NetworkTrafficProcessor.hpp"

NetworkTrafficProcessor::NetworkTrafficProcessor() : mStop(true)
{

}

NetworkTrafficProcessor::~NetworkTrafficProcessor()
{
	StopConnection();
}

void NetworkTrafficProcessor::StartConnection(
	std::string_view host,
	std::string_view service,
	size_t reconnectionsAttempts,
	size_t connectionAwaitTimeout)
{
	if (!mStop.exchange(false))
	{
		throw std::runtime_error("Network Traffic Processor already started");
	}

	mClient.SetReconnectionsCount(reconnectionsAttempts);
	mClient.SetAwaitingTimeout(std::chrono::milliseconds(connectionAwaitTimeout));
	mClient.StartConnection(host, service);
}

void NetworkTrafficProcessor::PostToSend(std::string_view message)
{
	{
		std::lock_guard<std::mutex> bufferLock(mBufferMutex);
		mBuffer.emplace(message);
	}
	mNewMessageCv.notify_one();
}

void NetworkTrafficProcessor::StartSending()
{
	if (!mStop)
	{
		throw std::runtime_error("Network Traffic Processor not started");
	}

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
			mClient.SendPacket(currentMessage);
		}
	}
}

void NetworkTrafficProcessor::StopConnection()
{
	if (mStop.exchange(true))
	{
		return;
	}

	mClient.StopConnection();
	mNewMessageCv.notify_all();
	std::cerr << "The network processor has been stopped " << std::endl;
}

