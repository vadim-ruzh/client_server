#ifndef CLIENTANDSERVER_CLIENT_APP_INCLUDE_USERINPUTPROCESSOR_HPP_
#define CLIENTANDSERVER_CLIENT_APP_INCLUDE_USERINPUTPROCESSOR_HPP_

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

class UserInputProcessor
{
 public:
	UserInputProcessor();

	void RunProcessing();

	void StopProcessing();

	boost::signals2::signal<void(std::string)> onNewMessage;
 private:
	void PostAsyncTaskThatReadingUserInput();
	void ReadingUserInputHandler(const boost::system::error_code& errorCode ,size_t bytesReceived);

	boost::asio::io_context mContext;
	boost::asio::posix::stream_descriptor mStdInDescriptor;
	std::string line;
};

#endif //CLIENTANDSERVER_CLIENT_APP_INCLUDE_USERINPUTPROCESSOR_HPP_
