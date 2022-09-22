#ifndef CLIENTANDSERVER_CLIENT_APP_INCLUDE_NETWORKPROCESSOR_HPP_
#define CLIENTANDSERVER_CLIENT_APP_INCLUDE_NETWORKPROCESSOR_HPP_

#include <queue>
#include <iostream>
#include <mutex>
#include <string_view>
#include <boost/signals2.hpp>
#include <TcpClient.hpp>

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
	TcpClient client;
	std::queue<std::string> mBuffer;
	std::mutex mMutex;
	std::atomic_bool mStop;
	std::condition_variable mCv;
};


#endif //CLIENTANDSERVER_CLIENT_APP_INCLUDE_NETWORKPROCESSOR_HPP_
