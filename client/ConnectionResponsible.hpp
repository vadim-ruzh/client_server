#ifndef CONNECTIONRESPONSIBLE_HPP_
#define CONNECTIONRESPONSIBLE_HPP_

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/core/noncopyable.hpp>

class ConnectionResponsible : boost::noncopyable
{
public:
	explicit ConnectionResponsible(boost::asio::io_context& ioContext,
		boost::asio::ip::tcp::resolver::results_type endpoints,
		size_t attemptsToReconnect = 3) noexcept;

	void Connect() noexcept(false);
	void Disconnect() noexcept(false);
	void ReConnect() noexcept(false);

	void Read(uint8_t* data, size_t size) noexcept(false);
	void Write(const uint8_t* data, size_t size) noexcept(false);
protected:
	void CloseIFOpen() noexcept(false);

	boost::system::error_code tryConnect() noexcept;
	boost::system::error_code tryReConnect() noexcept;

	boost::system::error_code tryWrite(const uint8_t* data, size_t size) noexcept;
	boost::system::error_code tryRead(uint8_t* data, size_t size) noexcept;
private:
	size_t mReconnectionAttempts;
	boost::asio::io_context& mIoContext;
	boost::asio::ip::tcp::resolver::results_type mEndpoints;
	std::unique_ptr<boost::asio::ip::tcp::socket> mSocket;
};

#endif //CONNECTIONRESPONSIBLE_HPP_
