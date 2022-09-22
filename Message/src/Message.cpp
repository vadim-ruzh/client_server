#include "../include/Message.hpp"

#include <boost/crc.hpp>

namespace detail
{
	uint32_t GetCrc32(const void* data, size_t size)
	{
		boost::crc_32_type result;
		result.process_bytes(data, size);
		return result.checksum();
	}
}

uint8_t* Packet::Checksum()
{
	return Body() + messageLength;
}

std::string Packet::DecodeBody()
{
	uint32_t receivedChecksum = *(reinterpret_cast<uint32_t*>(Checksum()));
	uint32_t actual_crc = detail::GetCrc32(Body(), messageLength);

	if (receivedChecksum != actual_crc)
	{
		throw std::runtime_error("checksum is not correct");
	}

	std::string str(reinterpret_cast<const char*>(Body()), messageLength);
	return str;
}
void Packet::DecodeLength()
{
	messageLength = *(reinterpret_cast<uint32_t*>(Header()));

	mBuffer.resize(headerLength + messageLength + crcLength);
}
void Packet::Clear()
{
	mBuffer.clear();
	messageLength = 0;
}
void Packet::EncodePacket(std::string_view message)
{
	messageLength = static_cast<uint32_t>(message.size());
	mBuffer.resize(headerLength + messageLength + crcLength);

	//encode packet length
	std::memcpy(mBuffer.data(), &messageLength, headerLength);

	//encode Message
	std::memcpy(Body(), message.data(), message.size());

	//encode crc
	auto crc = detail::GetCrc32(message.data(), message.size());
	std::memcpy(Checksum(), &crc, crcLength);
}
uint32_t Packet::BodyLength() const
{
	return messageLength + crcLength;
}
 uint8_t Packet::HeaderLength()
{
	return headerLength;
}
uint32_t Packet::DataLength() const
{
	return mBuffer.size();
}
uint8_t* Packet::Body()
{
	return Header() + headerLength;
}
uint8_t* Packet::Header()
{
	return mBuffer.data();
}
uint8_t* Packet::Data()
{
	return mBuffer.data();
}
