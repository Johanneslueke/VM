#pragma once

#include <regex>
#include <string>
#include <sstream>

#include <istream>

#include "ByteCode.h"
#include "VM.h"

namespace vm
{
	bool isNumber(std::string token)
	{
		return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
	}

	class ByteCodeFileReader
	{
	private:
		std::stringstream			mFileContent;
		bool						mIsHumanReadable;
	public:

		ByteCodeFileReader(const std::string& file,bool ishumanreadable = true): mIsHumanReadable(ishumanreadable) {

			std::istream	ByteCodeFile(file.c_str());
			while (!ByteCodeFile.eof())
				mFileContent.put(ByteCodeFile.get());
		}


		VM::Memory operator()()
		{
			std::string token;
			VM::Memory  code;

			while (!mFileContent.eof())
			{
				mFileContent >> token;

				if (mIsHumanReadable)
				{
					if (isNumber(token))
					{

					}
					else 
						for (auto i : InstructionCode)
							if(i->mName == token)
								code.emplace_back(
									VM::Type{ 
										VM::Type::INT,
										{ 
											std::atoi(token.c_str()
										}
							});
				}
				else
				{
					switch (std::atoi(token.c_str()))
					{
					default:
						break;
					}
				}

			}
		}

	};
}