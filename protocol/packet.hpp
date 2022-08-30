#ifndef PACKET_HPP_
#define PACKET_HPP_

#include <inttypes.h>
#include <cstddef>
#include <string>

enum class ResultCode : uint32_t
{
	S_OK,
	E_FAIL,
};

enum class ArgTypes : uint32_t
{
	Array,
	Int16,
	UInt16,
	Char16,
	Char32,
	Char8,
	ResultCode,
};

enum class Commands : uint16_t
{
	ProcessSting = 0,
	Result,
};

namespace protocol
{

#pragma pack(push, 1)

	struct header
	{
		uint8_t version[4];
		uint32_t length;
		uint16_t packet_id; // 0,
		uint8_t isRequest; // 0,
		uint16_t crc;
	};

	struct arrayValue // "Network byte order" is Big Endian
	{ // {1, 2, 3}
		ArgTypes /*commandArgument*/ elementsType; // Uint16
		uint16_t elementsCount; // 3

		uint8_t* elements; // { 0x00,0x01, 0x00,0x02, 0x00,0x03 }
	};

	struct commandArgument
	{ // int16_t ; std::string("OAOa") : 1-2*5 ; std::wstring("OAOa") : 2-4*5
		ArgTypes argType; // Int ; String
		uint8_t* value; // **** **** **** **** ; arrayValue { Char, 5, [0x012,...., ]} ; { Char, 5, [0x012,...., ]}
	};

	struct body
	{
		Commands /*Commands*/ command;// command/result
		uint16_t argsCount; // 1
		uint8_t* /*commandArgument*/ args; // ResultCode
	};

	struct packet
	{
		//static constexpr std::size_t version_length = 4;

		//header header;
		//body body;

		void pack(char* ptr, size_t lenght)
		{
			size = lenght;
			data = reinterpret_cast<uint8_t*>(ptr);
		}

		size_t size;
		uint8_t* data;
	};

#pragma pack(pop)
}

#endif //PACKET_HPP_
