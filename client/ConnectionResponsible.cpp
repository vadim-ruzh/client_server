#include "ConnectionResponsible.hpp"

#include <thread>
#include <iostream>

ConnectionResponsible::ConnectionResponsible(
	boost::asio::io_context& ioContext,
	boost::asio::ip::tcp::resolver::results_type endpoints,
	size_t attemptsToReconnect) noexcept
	: mIoContext(ioContext),
	  mEndpoints(std::move(endpoints)),
	  mSocket(std::make_unique<boost::asio::ip::tcp::socket>(ioContext)), mReconnectionAttempts(attemptsToReconnect)
{
}

void ConnectionResponsible::Connect() noexcept(false)
{
	CloseIFOpen();

	auto errorCode = tryConnect();

	if (errorCode)
	{
		ReConnect();
	}
}

void ConnectionResponsible::ReConnect() noexcept(false)
{
	CloseIFOpen();

	boost::system::error_code errorCode = boost::asio::error::not_connected;

	for (size_t i = 1; i <= mReconnectionAttempts; ++i)
	{
		errorCode = tryReConnect();

		if (!errorCode)
		{
			return;
		}
	}

	throw boost::system::system_error(errorCode);
}

void ConnectionResponsible::Disconnect() noexcept(false)
{
	CloseIFOpen();
}

void ConnectionResponsible::Write(const uint8_t* data, size_t size) noexcept(false)
{
	auto errorCode = tryWrite(data, size);

	if (errorCode)
	{
		ReConnect();

		errorCode = tryWrite(data, size);

		if (errorCode)
		{
			std::cerr << "Reconnected but writing failed :" << errorCode.message() << std::endl;
		}
	}
}

void ConnectionResponsible::Read(uint8_t* data, size_t size) noexcept(false)
{
	auto errorCode = tryRead(data, size);

	if (errorCode)
	{
		ReConnect();

		errorCode = tryRead(data, size);

		if (errorCode)
		{
			std::cerr << "Reconnected but reading failed :" << errorCode.message() << std::endl;
		}
	}
}

void ConnectionResponsible::CloseIFOpen() noexcept(false)
{
	if (mSocket->is_open())
	{
		mSocket->close();
	}
}

boost::system::error_code ConnectionResponsible::tryReConnect() noexcept
{
	std::cerr << "Connection error, retrying after 15 seconds" << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(15));

	return tryConnect();
}

boost::system::error_code ConnectionResponsible::tryConnect() noexcept
{
	boost::system::error_code errorCode;

	boost::asio::connect(*mSocket, mEndpoints, errorCode);

	return errorCode;
}

boost::system::error_code ConnectionResponsible::tryRead(uint8_t* data, size_t size) noexcept
{
	boost::system::error_code errorCode;

	boost::asio::read(*mSocket, boost::asio::buffer(data, size), errorCode);

	return errorCode;
}

boost::system::error_code ConnectionResponsible::tryWrite(const uint8_t* data, size_t size) noexcept
{
	boost::system::error_code errorCode;

	boost::asio::write(*mSocket, boost::asio::buffer(data, size),errorCode);

	return errorCode;
}

