#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include <optional>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

/**
 * @brief The class establishes a tcp connection and sends data over it.
 */
class TcpClient : public boost::noncopyable
{
 public:
	TcpClient() noexcept;
	~TcpClient() noexcept;

	/**
	 * @brief Set the number of reconnects, in case of a failed connection.
	 */
	void SetReconnectionsCount(size_t count) noexcept;

	/**
	 * @brief Set awaiting timeout per connection attempt in milliseconds.
	 */
	void SetAwaitingTimeout(std::chrono::milliseconds ms) noexcept;

	/**
	 * @brief Connecting to an endpoint.
	 * @details Resolve endpoint and connection.
	 * @param host url or ip address of the host to which you want to connect.
	 * @param service The service or port to which to connect.
	 * @remark If the connection is already connected, an exception is thrown.
	 * @remark If the connection fails, an exception is thrown.
	 * @throw boost::system::system_error
	 * @note For more information on resolving,see the Resolve method.
	 * @note For more information on connecting, see the Connect method.
	 */
	void StartConnection(std::string_view host, std::string_view service) noexcept(false);

	/**
	 * @brief Closes the connection
	 * @remark Re-closing will not be considered an error
	 */
	void StopConnection() noexcept;

	/**
	 * @brief Sends prepackaged data.
	 * @details The data is packaged and send.
	 * @param message Message to be packaged.
	 * @throw boost::system::system_error
	 * @note For more information on sending, see the Send method.
	 */
	void SendPacket(std::string_view message) noexcept(false);

	/**
	 * @brief Sends raw data
	 * @details If there is an error sending data, the Reconnect method is called,
	 * 			if the reconnect was successful, a second attempt is made to send data.
	 * @param data Pointer to the beginning of the data to send.
	 * @param size Send data size.
	 * @remark If is not connected, an exception is thrown.
	 * @remark If the reconnect attempt was unsuccessful, an exception is thrown.
	 * @remark If the second attempt to send the data was not successful, an exception is thrown.
	 * @throw boost::system::system_error
	 */
	void Send(const void* data, size_t size) noexcept(false);
 private:
	/**
	 * @brief Trying to reconnect.
	 * @details Connection is disconnected and an attempt is made to re-establish the connection.
	 * @remarks If the connection was not successful, an exception is thrown.
	 * @throw boost::system::system_error
	 * @note For more information on connecting, see the Connect method.
	 */
	void Reconnect() noexcept(false);

	/**
	 * @brief Resolve endpoints.
	 * @details Resolve endpoints and set mEndpoints to the resulting value.
	 * @param host Url or ip address of the host to resolve.
	 * @param service The service or port to which to resolve.
	 * @remark If nothing was Resolved, an exception is thrown.
	 * @throw boost::system::system_error
	 */
	void Resolve(std::string_view host, std::string_view service) noexcept(false);

	/**
 	* @brief A socket is created and an attempt is made to establish a connection on it.
 	* @details If the connection was not successful, the socket is closed.
 	* @remark If the connection was not successful, an exception is thrown.
 	* @throw boost::system::system_error
	* @note For more information on connection attempt, see the TryConnect method.
 	*/
	void Connect() noexcept(false);

	/**
	 * @brief Attempts are being made to connect to the endpoint.
	 * @details Waiting for mAwaitingTimeout before each attempt.
	 * @details There are mReconnectionsCount attempts to connect.
	 * @param host Url or ip address of the host to resolve.
	 * @param service The service or port to which to resolve.
	 * @return result of connection attempts.
	 */
	boost::system::error_code TryConnect() noexcept;

	std::chrono::milliseconds mAwaitingTimeout;
	size_t mReconnectionsCount;
	boost::asio::io_context mContext;
	boost::asio::ip::tcp::resolver::results_type mEndpoints;
	std::unique_ptr<boost::asio::ip::tcp::socket> mConnection;
};

#endif //TCPCLIENT_HPP_
