#include "UserInputProcessor.hpp"

UserInputProcessor::UserInputProcessor() : mStdInDescriptor(mContext, ::dup(STDIN_FILENO))
{
	ReadUserInput();
}

void UserInputProcessor::RunProcessing()
{
	mContext.run();
}

void UserInputProcessor::StopProcessing()
{
	mContext.stop();
}

void UserInputProcessor::ReadUserInput()
{
	boost::asio::async_read_until(mStdInDescriptor, boost::asio::dynamic_buffer(line), '\n',
		[this](const boost::system::error_code& errc, size_t sizeReceived)
		{
		  if (!errc)
		  {
			  if (sizeReceived)
			  {
				  onNewMessage(std::string(line.data(), sizeReceived - 1));
				  line.clear();
			  }

			  ReadUserInput();
		  }
		  else
		  {
			  throw boost::system::system_error(errc, errc.message());
		  }
		});
}
