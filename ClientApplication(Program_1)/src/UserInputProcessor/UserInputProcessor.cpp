#include "UserInputProcessor.hpp"

UserInputProcessor::UserInputProcessor() : mStdInDescriptor(mContext, ::dup(STDIN_FILENO))
{
	PostAsyncTaskThatReadingUserInput();
}

UserInputProcessor::~UserInputProcessor()
{
	StopReadingInput();
}


void UserInputProcessor::StartReadingInput()
{
	mContext.run();
}

void UserInputProcessor::StopReadingInput()
{
	mContext.stop();
}

void UserInputProcessor::PostAsyncTaskThatReadingUserInput()
{
	boost::asio::async_read_until(mStdInDescriptor, boost::asio::dynamic_buffer(line), '\n',
		std::bind(&UserInputProcessor::ReadingUserInputHandler,this,std::placeholders::_1,std::placeholders::_2));
}

void UserInputProcessor::ReadingUserInputHandler(const boost::system::error_code& errorCode, size_t bytesReceived)
{
	if (!errorCode)
	{
		if (bytesReceived)
		{
			onNewMessage(std::string(line.data(), bytesReceived - 1));
			line.clear();
		}

		PostAsyncTaskThatReadingUserInput();
	}
	else
	{
		throw boost::system::system_error(errorCode, errorCode.message());
	}
}
