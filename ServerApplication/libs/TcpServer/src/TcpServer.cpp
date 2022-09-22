#include "TcpServer.hpp"

#include <cstddef>
#include <thread>

TcpServer::TcpServer() : mIoContext(), mMaxClientSessions(1)
{

}

void TcpServer::StartAcceptor(uint16_t port, size_t maxClientSessions)
{
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

void TcpServer::RunProccesing()
{
	mIoContext.run();
}

void TcpServer::StopProccesing()
{
	mClientSessions.RemoveAllSessions();
	mIoContext.stop();
}

boost::asio::io_context& TcpServer::GetContext()
{
	return mIoContext;
}

void TcpServer::SetOnDisconnectHandler(std::function<void(std::string_view)>&& handler)
{
	mClientSessions.onDisconnect.connect(handler);
}

void TcpServer::SetOnErrorHandler(std::function<void(std::string_view, std::string_view)>&& handler)
{
	mClientSessions.onError.connect(handler);
}

void TcpServer::SetOnConnectHandler(std::function<void(std::string_view)>&& handler)
{
	mClientSessions.onConnect.connect(handler);
}

void TcpServer::SetOnRequestHandler(std::function<void(std::string_view, std::string_view)>&& handler)
{
	mClientSessions.onRequest.connect(handler);
}



