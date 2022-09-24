#include "TcpServer.hpp"

#include <cstddef>
#include <chrono>
#include <thread>

TcpServer::TcpServer() : mIoContext(), mMaxClientSessions(1)
{

}

void TcpServer::StartAcceptor(uint16_t port, size_t maxClientSessions)
{
	if (mAcceptor)
	{
		throw std::runtime_error("Acceptor already started");
	}

	mMaxClientSessions = maxClientSessions;
	mAcceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(mIoContext,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));

	Accept();
}

void TcpServer::Accept()
{
	mAcceptor->async_accept(boost::asio::make_strand(mIoContext),
		[this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
		{
		  if (!ec)
		  {
			  mClientSessions.StartNewSession(std::move(socket));
		  }

		  mClientSessions.Size() < mMaxClientSessions ? Accept() : WaitForAccept();
		});
}

void TcpServer::WaitForAccept()
{
	mIoContext.post([this]()
	{
	  std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });

	  mClientSessions.Size() < mMaxClientSessions ? Accept() : WaitForAccept();
	});
}

void TcpServer::StartProcessingConnections()
{
	if (!mAcceptor)
	{
		throw std::runtime_error("Acceptor not started");
	}

	mIoContext.run();
}

void TcpServer::StopAcceptor()
{
	if (!mAcceptor)
	{
		return;
	}

	mClientSessions.RemoveAllSessions();
	mIoContext.stop();
	mAcceptor.reset();
}

boost::asio::io_context& TcpServer::GetContext()
{
	return mIoContext;
}

void TcpServer::SetOnDisconnectHandler(const std::function<void(std::string_view)>& handler)
{
	mClientSessions.onDisconnect.connect(handler);
}

void TcpServer::SetOnErrorHandler(const std::function<void(std::string_view, std::string_view)>& handler)
{
	mClientSessions.onError.connect(handler);
}

void TcpServer::SetOnConnectHandler(const std::function<void(std::string_view)>& handler)
{
	mClientSessions.onConnect.connect(handler);
}

void TcpServer::SetOnRequestHandler(const std::function<void(std::string_view, std::string_view)>& handler)
{
	mClientSessions.onRequest.connect(handler);
}

TcpServer::~TcpServer()
{
	StopAcceptor();
}



