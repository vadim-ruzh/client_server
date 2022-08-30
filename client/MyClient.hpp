#ifndef MYCLIENT_HPP_
#define MYCLIENT_HPP_

#include "TcpClient.hpp"
#include"../protocol/packet.hpp"
//#include"../protocol/packet_manager.hpp"

class MyClient : public TcpClient<protocol::packet>
{
public:
	MyClient() = default;

	void SendString(const std::string& string) noexcept(false);
};

#endif //MYCLIENT_HPP_
