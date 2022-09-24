#include "TcpClient.hpp"
#include "Packet.hpp"

TcpClient::TcpClient() noexcept
	: mReconnectionsCount(5), mAwaitingTimeout(50)
{
}

TcpClient::~TcpClient() noexcept
{
	StopConnection();
}

void TcpClient::StartConnection(std::string_view host, std::string_view service) noexcept(false)
{
	if (mConnection)
	{
		throw boost::system::system_error(boost::asio::error::already_connected);
	}

	Resolve(host, service);
	Connect();
}

void TcpClient::StopConnection() noexcept
{
	mConnection.reset();
}

void TcpClient::Send(const void* data, size_t size) noexcept(false)
{
	if (!mConnection)
	{
		throw boost::system::system_error(boost::asio::error::not_connected);
	}

	try
	{
		boost::asio::write(*mConnection, boost::asio::buffer(data, size));
	}
	catch (const boost::system::system_error& sendError)
	{
		std::cerr << "The connection was broken" << std::endl;
		Reconnect();
		boost::asio::write(*mConnection, boost::asio::buffer(data, size));
	}
}

void TcpClient::Resolve(std::string_view host, std::string_view service) noexcept(false)
{
	boost::asio::ip::tcp::resolver resolver(mContext);
	mEndpoints = resolver.resolve(host, service);
}

boost::system::error_code TcpClient::TryConnect() noexcept
{
	boost::system::error_code errorCode;

	for (size_t i = 0; i < mReconnectionsCount && mConnection; ++i)
	{
		std::this_thread::sleep_for(mAwaitingTimeout);
		boost::asio::connect(*mConnection, mEndpoints, errorCode);

		if (!errorCode)
		{
			std::cout << "The connection was successful" << std::endl;
			break;
		}
	}

	return errorCode;
}

void TcpClient::Connect() noexcept(false)
{
	mConnection = std::make_unique<boost::asio::ip::tcp::socket>(mContext);

	auto connectionError = TryConnect();
	if (connectionError)
	{
		mConnection.reset();
		throw boost::system::system_error(connectionError);
	}
}

void TcpClient::Reconnect() noexcept(false)
{
	StopConnection();
	Connect();
}

void TcpClient::SendPacket(std::string_view string) noexcept(false)
{
	Packet packet;

	packet.EncodePacket(string);

	Send(packet.Data(), packet.DataLength());
}

void TcpClient::SetAwaitingTimeout(std::chrono::milliseconds ms) noexcept
{
	mAwaitingTimeout = ms;
}

void TcpClient::SetReconnectionsCount(size_t count) noexcept
{
	mReconnectionsCount = count;
}



