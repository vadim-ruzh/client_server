#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_

#include "../src/ClientSessionGroup.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>

 class TcpServer : public boost::noncopyable
{
 public:
	TcpServer();
	~TcpServer();

	void StartAcceptor(uint16_t port, size_t maxClientSessions);
	void StopAcceptor();

	void StartProcessingConnections();

	boost::asio::io_context& GetContext();

	void SetOnRequestHandler(const std::function<void(std::string_view clientIpAddress,std::string_view requestMessage)>& handler);
	void SetOnConnectHandler(const std::function<void(std::string_view clientIpAddress)>& handler);
	void SetOnDisconnectHandler(const std::function<void(std::string_view clientIpAddress)>& handler);
	void SetOnErrorHandler(const std::function<void(std::string_view clientIpAddress,std::string_view errorMessage)>& handler);
 private:
	void Accept();
	void WaitForAccept();

	boost::asio::io_context mIoContext;
	std::unique_ptr<boost::asio::ip::tcp::acceptor> mAcceptor;
	size_t mMaxClientSessions;
	ClientSessionGroup mClientSessions;
};

#endif //TCPSERVER_HPP_

