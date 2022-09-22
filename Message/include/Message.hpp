#ifndef SERVERAPPLICATION_MESSAGE_INCLUDE_MESSAGE_HPP_
#define SERVERAPPLICATION_MESSAGE_INCLUDE_MESSAGE_HPP_

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string_view>

class Packet
{
 public:
	Packet() : mBuffer(4), messageLength(0)
	{
	}

	uint8_t* Data();
	uint8_t* Header();
	uint8_t* Body();

	uint32_t DataLength() const;
	uint8_t HeaderLength();
	uint32_t BodyLength() const;

	void EncodePacket(std::string_view message);

	void DecodeLength();

	std::string DecodeBody();

	void Clear();
 private:
	uint8_t* Checksum();

	static constexpr uint8_t headerLength = 4;
	uint32_t messageLength;
	static constexpr uint8_t crcLength = 4;

	std::vector<uint8_t> mBuffer;
};

#endif //SERVERAPPLICATION_MESSAGE_INCLUDE_MESSAGE_HPP_
