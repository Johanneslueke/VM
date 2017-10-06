#pragma once
#include "Source.h"
#include <variant>


namespace vm::frontend
{
	class Token
	{
	private:
		//TokenType mType;
		std::string mText;
		std::variant<int> mValue;
		Source mSource;
		int LineNum;
		int position;

	public:

		Token(const Source& src);

		virtual void extract(){

			mText = std::to_string(currentChar());
			
		}

		char currentChar()
		{
			return mSource.currentChar();
		}

		char nextChar()
		{
			return mSource.nextChar();
		}

		char peekChar()
		{
			return mSource.peekChar();
		}

	};
}
