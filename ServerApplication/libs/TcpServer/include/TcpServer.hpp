#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_

#include "../src/СlientSessionGroup.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>

class TcpServer
{
 public:
	TcpServer();

	~TcpServer()
	{
		StopProccesing();
	}

	void StartAcceptor(uint16_t port, size_t maxClientSessions);

	void RunProccesing();
	void StopProccesing();

	boost::asio::io_context& GetContext();

	void SetOnRequestHandler(std::function<void(std::string_view clientIpAddress,
		std::string_view requestMessage)>&& handler);
	void SetOnConnectHandler(std::function<void(std::string_view clientIpAddress)>&& handler);
	void SetOnDisconnectHandler(std::function<void(std::string_view clientIpAddress)>&& handler);
	void SetOnErrorHandler(std::function<void(std::string_view clientIpAddress,
		std::string_view errorMessage)>&& handler);
 private:
	void Accept();
	void WaitForAccept();

	boost::asio::io_context mIoContext;
	std::unique_ptr<boost::asio::ip::tcp::acceptor> mAcceptor;
	size_t mMaxClientSessions;
	ClientSessionGroup mClientSessions;
};

#endif //TCPSERVER_HPP_

