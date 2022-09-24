#ifndef CLIENTSESSION_HPP_
#define CLIENTSESSION_HPP_

#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include "Packet.hpp"

class ClientSessionGroup;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
 public:
	ClientSession(boost::asio::ip::tcp::socket&& sock, ClientSessionGroup& owner);
	~ClientSession();

	void Start();
	void Stop();
 public:
	void ReadHeader();
	void ReadBody();

	void HeaderHandler(const boost::system::error_code& errorCode, size_t);
	void BodyHandler(const boost::system::error_code& errorCode, size_t);

	boost::asio::ip::tcp::socket mSocket;
	ClientSessionGroup& mOwner;
	Packet packet;
	std::string mClientAddress;
};

using ClientSessionPtr = std::shared_ptr<ClientSession>;


#endif //CLIENTSESSION_HPP_