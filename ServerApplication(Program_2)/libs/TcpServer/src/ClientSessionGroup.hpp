#ifndef CLIENTSESSIONGROUP_HPP_
#define CLIENTSESSIONGROUP_HPP_

#include <shared_mutex>
#include <unordered_set>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include "ClientSession.hpp"

class ClientSessionGroup
{
 public:
	void StartNewSession(boost::asio::ip::tcp::socket socket);
	void AddSession(ClientSessionPtr sessionPtr);
	void RemoveSession(ClientSessionPtr sessionPtr);
	void RemoveAllSessions();

	size_t Size();

	boost::signals2::signal<void(std::string_view  ClientAddress)> onConnect;
	boost::signals2::signal<void(std::string_view  ClientAddress)> onDisconnect;
	boost::signals2::signal<void(std::string_view  ClientAddress,std::string_view message)> onRequest;
	boost::signals2::signal<void(std::string_view  ClientAddress, std::string_view ErrorMessage)> onError;
 private:
	std::shared_mutex mMutex;
	std::unordered_set<ClientSessionPtr> mClientSessions;
};

#endif //CLIENTSESSIONGROUP_HPP_
