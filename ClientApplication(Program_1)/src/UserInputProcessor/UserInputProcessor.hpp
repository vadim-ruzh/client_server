#ifndef USERINPUTPROCESSOR_HPP_
#define USERINPUTPROCESSOR_HPP_

#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>

 class UserInputProcessor : public boost::noncopyable
{
 public:
	UserInputProcessor();
	~UserInputProcessor();

	/**
	* @brief Start receiving user input.
	*/
	void StartReadingInput();

	/**
	* @brief Stop receiving data from the user.
	*/
	void StopReadingInput();

	/**
	* @brief Data handler received from the user
	*/
	boost::signals2::signal<void(std::string)> onNewMessage;
 private:

	/**
	* @brief Setting a task in context to receive data from the user
	*/
	void PostAsyncTaskThatReadingUserInput();

	/**
	* @brief Processing data received from the user.
	* @remark If an error occurs while receiving user input, an exception is thrown.
	* @throw boost::sytem::system_error
	*/
	void ReadingUserInputHandler(const boost::system::error_code& errorCode ,size_t bytesReceived);

	boost::asio::io_context mContext;
	boost::asio::posix::stream_descriptor mStdInDescriptor;
	std::string line;
};

#endif //USERINPUTPROCESSOR_HPP_
