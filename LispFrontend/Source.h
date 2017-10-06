#pragma once
#include <sstream>
#include <string>
#include "MessageProducer.h"
#include "MessageHandler.h"

namespace vm::frontend
{
	class Source : public message::MessageProducer
	{
	public:
		 char _EOL = '\n';
		 char _EOF = static_cast<char>(0);
	private:
		std::stringstream&			mReader;
		std::string					mLine;
		int							mLineNum;
		int							mCurrentPos;
		message::MessageHandler		mMsgHandler;

	public:

		Source(std::stringstream& BufferedReader);

		virtual void addListner(const message::Listner& listner) override
		{
			mMsgHandler.add(listner);
		}

		virtual void removeListner(const message::Listner& listner) override
		{
			mMsgHandler.remove(listner);
		}

		virtual void send(const message::Message& msg) override
		{
			mMsgHandler.send(msg);
		}

		virtual char currentChar()
		{
			if(mCurrentPos == -2)
			{
				readLine();
				return nextChar();
			}else if(mLine.empty())
			{
				return _EOF;
			}else if(mCurrentPos > mLine.length())
			{
				readLine();
				return nextChar();
			}else
			{
				return mLine[mCurrentPos];
			}
		}

		virtual char nextChar()
		{
			mCurrentPos++;
			return currentChar();
		}

		virtual char peekChar()
		{
			currentChar();
			if (mLine.empty())
				return _EOF;

			auto nextpos = mCurrentPos + 1;
			return nextpos < mLine.length() ? mLine[nextpos] : _EOL;
		}

		virtual void readLine()
		{
			auto rdLine = [this]() -> std::string
			{
				char buf[256];
				this->mReader.getline(buf, 256);
				return { buf };
			};
			mLine = rdLine();
			//Get rid of everything which is not a line
			mLine.erase(std::begin(mLine)+mLine.find('\n'), std::end(mLine));
			mCurrentPos = -1;
			if (!mLine.empty())
				mLineNum++;
		}

		void close()
		{
			
		}


	};
}
