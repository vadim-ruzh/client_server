#include "ClientSession.hpp"
#include "ClientSessionGroup.hpp"

#include <boost/asio/ip/tcp.hpp>

ClientSession::ClientSession(boost::asio::ip::tcp::socket&& sock, ClientSessionGroup& owner)
	: mSocket(std::move(sock)), mOwner(owner), mClientAddress(mSocket.remote_endpoint().address().to_string())
{
	mOwner.onConnect(mClientAddress);
}

ClientSession::~ClientSession()
{
	try
	{
		mOwner.onDisconnect(mClientAddress);
		mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	}
	catch (const boost::system::system_error& error)
	{
		mOwner.onError(mClientAddress, error.what());
	}
}

void ClientSession::Start()
{
	ReadHeader();
}

void ClientSession::ReadHeader()
{
	boost::asio::async_read(mSocket, boost::asio::buffer(packet.Header(), packet.HeaderLength()),
		std::bind(&ClientSession::HeaderHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void ClientSession::ReadBody()
{
	boost::asio::async_read(mSocket,boost::asio::buffer(packet.Body(),packet.BodyLength()),
		std::bind(&ClientSession::BodyHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void ClientSession::Stop()
{
	mOwner.RemoveSession(shared_from_this());
}

void ClientSession::BodyHandler(const boost::system::error_code& errorCode, size_t)
{
	if (!errorCode)
	{
		try
		{
			mOwner.onRequest(mClientAddress, packet.DecodeBody());
		}
		catch (const std::exception& ex)
		{
			mOwner.onError(mClientAddress, ex.what());
		}

		ReadHeader();
	}
	else
	{
		mOwner.onError(mClientAddress, errorCode.message());
		Stop();
	}
}

void ClientSession::HeaderHandler(const boost::system::error_code& errorCode, size_t)
{
	if (!errorCode)
	{
		try
		{
			packet.DecodeLength();
			ReadBody();
		}
		catch (const std::exception& ex)
		{
			mOwner.onError(mClientAddress, ex.what());
			ReadHeader();
		}
	}
	else
	{
		mOwner.onError(mClientAddress, errorCode.message());
		Stop();
	}
}

