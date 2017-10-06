#pragma once
#include <variant>


namespace vm::frontend::message
{
	class Message
	{
	public:
		using MessageType = std::variant<int>;
	private:
		std::variant<int> mType;
	public:

		Message(MessageType type) : mType{type}
		{}

	};
}
