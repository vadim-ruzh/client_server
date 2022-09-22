#include "../include/TcpClient.hpp"
#include "../../../../message/include/Message.hpp"

TcpClient::TcpClient()
	: mReconnectionsAttempts(5), mWaitBeforeConnection(1)
{
}

TcpClient::~TcpClient()
{
	Disconnect();
}

void TcpClient::StartConnection(std::string_view address, std::string_view service)
{
	Resolve(address, service);

	Connect();
}

void TcpClient::Disconnect()
{
	mConnection.reset();
}

void TcpClient::Send(const void* data, size_t size)
{
	if (!mConnection.has_value())
	{
		throw boost::system::system_error(boost::asio::error::not_connected);
	}

	boost::system::error_code sendError = TrySend(data, size);
	if (sendError == boost::asio::error::connection_refused || sendError == boost::asio::error::connection_aborted)
	{
		std::cerr << "The connection was broken" << std::endl;
		Reconnect();

		sendError = TrySend(data, size);
		if (sendError)
		{
			std::cerr << "An error occurred while sending the Message : " << sendError.message() << std::endl;
		}
	}
	if (sendError)
	{
		std::cerr << "An error occurred while sending the Message : " << sendError.message() << std::endl;
	}
}

void TcpClient::Resolve(std::string_view address, std::string_view service)
{
	boost::asio::ip::tcp::resolver resolver(mContext);
	mEndpoints = resolver.resolve(address, service);
}

boost::system::error_code TcpClient::TryConnect()
{
	boost::system::error_code errorCode;

	for (size_t i = 0; i < mReconnectionsAttempts && mConnection.has_value(); ++i)
	{
		std::this_thread::sleep_for(mWaitBeforeConnection);
		boost::asio::connect(*mConnection, mEndpoints, errorCode);

		if (!errorCode)
		{
			std::cout << "The connection was successful" << std::endl;
			break;
		}
	}

	return errorCode;
}

void TcpClient::Connect()
{
	if (mConnection.has_value())
	{
		throw boost::system::system_error(boost::asio::error::already_connected);
	}

	mConnection = std::make_optional<boost::asio::ip::tcp::socket>(mContext);

	auto connectionError = TryConnect();
	if (connectionError)
	{
		mConnection.reset();
		throw boost::system::system_error(connectionError);
	}
}

void TcpClient::Reconnect()
{
	Disconnect();
	Connect();
}

boost::system::error_code TcpClient::TrySend(const void* data, size_t size)
{
	boost::system::error_code errorCode;
	boost::asio::write(*mConnection, boost::asio::buffer(data, size), errorCode);
	return errorCode;
}

void TcpClient::SendString(std::string_view string)
{
	Packet packet;

	packet.EncodePacket(string);

	Send(packet.Data(), packet.DataLength());
}

void TcpClient::SetWaitBeforeConnection(std::chrono::seconds seconds)
{
	mWaitBeforeConnection = seconds;
}

void TcpClient::SetReconnectionsAttempt(size_t attempts)
{
	mReconnectionsAttempts = attempts;
}


