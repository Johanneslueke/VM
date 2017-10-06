#pragma once
#include <vector>
#include "MessageListner.h"
#include "Message.h"


namespace vm::frontend::message
{
	class Message;

	class MessageHandler
	{
	private:
		Message							mCurrentMessage;
		std::vector<Listner>		mlistner;
	public:
		MessageHandler() = default;

		void add(const Listner& listner)
		{
			mlistner.push_back(listner);
		}

		void remove(const Listner& listner)
		{
			mlistner.erase(std::find(std::begin(mlistner), std::end(mlistner), listner));
		}

		void send(const Message& msg)
		{
			mCurrentMessage = msg;
			notify();
		}

		void notify()
		{
			for (auto& listner : mlistner)
				listner.received(mCurrentMessage);
		}

	};
}

