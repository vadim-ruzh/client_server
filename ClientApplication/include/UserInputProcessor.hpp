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
	void ReadUserInput();

	boost::asio::io_context mContext;
	boost::asio::posix::stream_descriptor mStdInDescriptor;
	std::string line;
};

#endif //CLIENTANDSERVER_CLIENT_APP_INCLUDE_USERINPUTPROCESSOR_HPP_
