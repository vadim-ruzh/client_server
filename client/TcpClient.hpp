#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include "ConnectionResponsible.hpp"

template<class PacketType>
class TcpClient
{
public:
	TcpClient() noexcept;

	void ConnectTo(const std::string& address, const std::string& service) noexcept(false);
	void Disconnect() noexcept(false);

	void Send(const PacketType& packet) noexcept(false);
	void Receive(PacketType& packet) noexcept(false);
private:
	boost::asio::io_context mContext;
	boost::asio::ip::tcp::resolver mResolver;
	std::unique_ptr<ConnectionResponsible> mConnectionHolder;
};



template<class PacketType>
TcpClient<PacketType>::TcpClient() noexcept : mResolver(mContext)
{

}

template<class PacketType>
void TcpClient<PacketType>::ConnectTo(const std::string& address, const std::string& service) noexcept(false)
{
	auto endpoints = mResolver.resolve(address, service);

	mConnectionHolder = std::make_unique<ConnectionResponsible>(mContext, endpoints);

	mConnectionHolder->Connect();
}

template<class PacketType>
void TcpClient<PacketType>::Disconnect() noexcept(false)
{
	mConnectionHolder->Disconnect();
}

template<class PacketType>
void TcpClient<PacketType>::Send(const PacketType& packet) noexcept(false)
{
	mConnectionHolder->Write(packet.data, packet.size);
}

template<class PacketType>
void TcpClient<PacketType>::Receive(PacketType& packet) noexcept(false)
{
	mConnectionHolder->Read(packet.data, packet.size);
}

#endif //TCPCLIENT_HPP_
