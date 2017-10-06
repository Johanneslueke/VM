#pragma once
#include "Source.h"
#include "Token.h"

namespace vm::frontend
{
	class Scanner
	{
		Source mSource;
		Token  mCurrentToken;

	public:
		Scanner(const Source& src);

		Token currentToken() const
		{
			return mCurrentToken;
		}

		Token nextToken()
		{
			mCurrentToken = extractToken();
			return mCurrentToken;
		}

		virtual Token extractToken() = 0 ;

		char currentChar()
		{
			return mSource.currentChar();
		}

		char nextChar()
		{
			return mSource.nextChar();
		}
	};
}


