#pragma once



namespace vm::frontend::message
{
	class Message;
	class Listner;

	class MessageProducer
	{
	public:

		virtual void addListner(const Listner& listner) = 0;

		virtual void removeListner(const Listner& listner) = 0;

		virtual void send(const Message& msg) = 0;
	};
}