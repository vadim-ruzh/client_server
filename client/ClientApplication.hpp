#ifndef CLIENTAPPLICATION_HPP_
#define CLIENTAPPLICATION_HPP_

#include <queue>
#include <string>
#include <boost/asio.hpp>
#include "MyClient.hpp"

class ClientApplication
{
public:
	ClientApplication();

	void Start(const std::string& address, const std::string& service);
	void Stop();

private:
	void startNetworkOutput(const std::string& address, const std::string& service);
	void startNetworkOutputLoop(const std::string& address, const std::string& service);

	void startUserInput();
	void userInputLoop();

	std::queue<std::string> mBuffer;
	std::mutex mMutex;
	std::condition_variable mCv;
	std::atomic_bool mWork;
};

#endif //CLIENTAPPLICATION_HPP_
