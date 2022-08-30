#include "MyClient.hpp"

void MyClient::SendString(const std::string& string) noexcept(false)
{

	auto str = string;

	protocol::packet packet{};
	packet.pack(str.data(),str.size());

	//protocol::packet packet = mBuilder.CreatePacket(string);
	Send(packet);
}

