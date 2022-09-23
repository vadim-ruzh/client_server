#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include <optional>
#include <thread>
#include <iostream>

#include <boost/asio.hpp>

class TcpClient
{
 public:
	TcpClient();
	~TcpClient();

	void SetReconnectionsAttempt(size_t attempts);

	void SetConnectionAwaitingTimeout(std::chrono::milliseconds ms);

	void StartConnection(std::string_view address, std::string_view service);
	void Disconnect();

	void SendString(std::string_view string);

	void Send(const void* data, size_t size);
 private:
	void Resolve(std::string_view address, std::string_view service);

	boost::system::error_code TryConnect();
	void Connect();
	void Reconnect();

	std::chrono::milliseconds mAwaitingTimeout;
	size_t mReconnectionsAttempts;
	boost::asio::io_context mContext;
	boost::asio::ip::tcp::resolver::results_type mEndpoints;
	std::optional<boost::asio::ip::tcp::socket> mConnection;
};

#endif //TCPCLIENT_HPP_
