#pragma once


namespace vm::frontend::message
{
	class Message;

	class Listner
	{
	public:
		virtual void received(const Message& msg) = 0;


	};
}


