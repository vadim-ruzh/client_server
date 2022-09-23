#ifndef NETWORTRAFFICKPROCESSOR_HPP_
#define NETWORTRAFFICKPROCESSOR_HPP_

#include <queue>
#include <iostream>
#include <mutex>
#include <string_view>
#include <boost/signals2.hpp>
#include "TcpClient.hpp"

class NetworkTrafficProcessor
{
 public:
	NetworkTrafficProcessor();

	void StartTcpClient(std::string_view address, std::string_view service,size_t reconnectionsAttempts,size_t connectionAwaitTimeout);

	void SendMessage(std::string_view message);

	void RunProcessing();

	void StopProcessing();

	boost::signals2::signal<void(std::string&)> onNewMessage;
 private:
	std::atomic_bool mStop;
	TcpClient mClient;
	std::queue<std::string> mBuffer;
	std::mutex mBufferMutex;
	std::condition_variable mNewMessageCv;
};


#endif //NETWORTRAFFICKPROCESSOR_HPP_
