#ifndef CLIENTANDSERVER_CLIENT_APP_INCLUDE_NETWORKPROCESSOR_HPP_
#define CLIENTANDSERVER_CLIENT_APP_INCLUDE_NETWORKPROCESSOR_HPP_

#include <queue>
#include <iostream>
#include <mutex>
#include <string_view>
#include <boost/signals2.hpp>
#include <TcpClient.hpp>

// TODO: better naming - NetworkTrafficProcessor
class NetworkProcessorrr
{
 public:
	NetworkProcessorrr();

	void StartTcpClient(std::string_view address,std::string_view service);

	void SendMessage(std::string_view message);

	void RunProcessing();

	void StopProcessing();

	boost::signals2::signal<void(std::string&)> onNewMessage;
 private:
	// TODO: better naming - mClient
	TcpClient client;
	std::queue<std::string> mBuffer;
	// CRITICAL: better naming - mutex of what? i.e. mBufferMutex or mProcessorMutex
	std::mutex mMutex;
	std::atomic_bool mStop;
	// CRITICAL: better naming - condition variable of what? i.e. mNewMessagesCv
	std::condition_variable mCv;
};


#endif //CLIENTANDSERVER_CLIENT_APP_INCLUDE_NETWORKPROCESSOR_HPP_
