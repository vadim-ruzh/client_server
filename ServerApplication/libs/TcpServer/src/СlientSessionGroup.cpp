#include "СlientSessionGroup.hpp"

#include <memory>

void ClientSessionGroup::AddSession(ClientSessionPtr sessionPtr)
{
	std::unique_lock<std::shared_mutex> lock(mMutex);
	mClientSessions.insert(sessionPtr);
	lock.unlock();
}

void ClientSessionGroup::RemoveSession(ClientSessionPtr sessionPtr)
{
	std::unique_lock<std::shared_mutex> lock(mMutex);
	mClientSessions.erase(sessionPtr);
	lock.unlock();
}

void ClientSessionGroup::RemoveAllSessions()
{
	std::unique_lock<std::shared_mutex> lock(mMutex);
	mClientSessions.erase(mClientSessions.begin(), mClientSessions.end());
	lock.unlock();
}

size_t ClientSessionGroup::Size()
{
	std::shared_lock<std::shared_mutex> lock(mMutex);
	size_t size = mClientSessions.size();
	lock.unlock();

	return size;
}

void ClientSessionGroup::StartNewSession(boost::asio::ip::tcp::socket socket)
{
	ClientSessionPtr session = std::make_shared<ClientSession>(std::move(socket), *this);

	AddSession(session);

	session->Start();
}
