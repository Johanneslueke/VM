#pragma once
#include "Scanner.h"
#include "Token.h"
#include "MessageProducer.h"


namespace vm::frontend
{
	class Parser : public message::MessageProducer
	{
	private:
		Scanner& scanner;
		message::MessageHandler		mMsgHandler;

	public:

		Parser(Scanner& scanner);

		virtual void parse() = 0;
		virtual int getErrorCount() = 0;


		void addListner(const message::Listner& listner) override
		{
			mMsgHandler.add(listner);
		}

		void removeListner(const message::Listner& listner) override
		{
			mMsgHandler.remove(listner);
		}
		
		void send(const message::Message& msg) override
		{
			mMsgHandler.send(msg);
		}


		virtual Token& currentToken()
		{
			return scanner.currentToken();
		}

		virtual Token& nextToken()
		{
			return scanner.nextToken();
		}


	};
}
