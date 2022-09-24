#ifndef NETWORTRAFFICKPROCESSOR_HPP_
#define NETWORTRAFFICKPROCESSOR_HPP_

#include <queue>
#include <iostream>
#include <mutex>
#include <string_view>
#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>
#include "TcpClient.hpp"



/**
* @brief The class establishes a connection and sends data to the remote host.
*/
 class NetworkTrafficProcessor : public boost::noncopyable
{
 public:
	NetworkTrafficProcessor();
	~NetworkTrafficProcessor();

	/**
	* @brief Сonnection to a remote host.
	* @remark If the connection has not been established, an boost::system::sytem_error exception is thrown.
	* @remark If already connected, an std::runtime_error exception will be thrown.
	* @throw boost::sytem::system_error
	* @throw std::runtime_error
	*/
	void StartConnection(std::string_view host, std::string_view service,size_t reconnectionsAttempts,size_t connectionAwaitTimeout);
	
	/**
	* @brief The connection is closed and a shutdown signal is sent to all threads sending data on this connection.
	*/
	void StopConnection();

	/**
	* @brief Queue data to be send.
	*/
 	void PostToSend(std::string_view message);

	/**
	* @brief Start a loop for sending messages that are in the queue.
	* @remarks If is not connection, an exception will be thrown.
	* @throw std::runtime_error 
	*/
 	void StartSending();

	/**
	* @brief Message handler before sending to remote host.
	*/
	boost::signals2::signal<void(std::string&)> onNewMessage;
 private:
	std::atomic_bool mStop;
	TcpClient mClient;
	std::queue<std::string> mBuffer;
	std::mutex mBufferMutex;
	std::condition_variable mNewMessageCv;
};


#endif //NETWORTRAFFICKPROCESSOR_HPP_
