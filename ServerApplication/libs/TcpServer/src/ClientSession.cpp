#include "ClientSession.hpp"
#include "СlientSessionGroup.hpp"

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
	boost::asio::async_read(mSocket, boost::asio::buffer(message.Header(), message.HeaderLength()),
		std::bind(&ClientSession::MessageLengthHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void ClientSession::ReadBody()
{
	boost::asio::async_read(mSocket,boost::asio::buffer(message.Body(),message.BodyLength()),
		std::bind(&ClientSession::ReadBodyHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void ClientSession::Stop()
{
	mOwner.RemoveSession(shared_from_this());
}

void ClientSession::ReadBodyHandler(boost::system::error_code errc, size_t)
{
	if (!errc)
	{
		try
		{
			mOwner.onRequest(mClientAddress, message.DecodeBody());
		}
		catch (const std::exception& ex)
		{
			mOwner.onError(mClientAddress, ex.what());
		}

		ReadHeader();
	}
	else
	{
		mOwner.onError(mClientAddress, errc.message());
		Stop();
	}
}

void ClientSession::MessageLengthHandler(boost::system::error_code errc, size_t)
{
	if (!errc)
	{
		try
		{
			message.DecodeLength();
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
		mOwner.onError(mClientAddress, errc.message());
		Stop();
	}
}

