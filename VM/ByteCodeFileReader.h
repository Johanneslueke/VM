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
				mFileContent.put(ByteCodeFile.get()); //TODO: Could be done better!
		}

		void parseHumanReadable()
		{
			if (mIsHumanReadable)
			{
				VM::Memory  code;
				std::string buffer;
				while (!mFileContent.eof())
				{
					char ch = mFileContent.get();
					if (ch != ' ' || ch != '\n')
					{
						buffer += ch;
					}
					else
					{
						for (auto& i : InstructionCode)
						{
							if (i.mName == buffer)
							{
								code.emplace_back({ VM::Type::INT,{ static_cast<double>(i.mNumericName) } });
							}
						}
					}
				}
			}
			*this();
		}


		VM::Memory operator()() // MEh don'T like that--- does not look neet. there must be a different way
		{
			std::string token;
			VM::Memory  code;

			while (!mFileContent.eof())
			{
				char byte = static_cast<char>(mFileContent.get());

				switch (byte)
				{
				case IPUSH:
				case GSTORE:
				case GLOAD:
				case LOAD:
				case STORE:
				case BRANCH:
				case BRTRUE:
				case BRFALSE:
					code.emplace_back({ VM::Type::INT,{ static_cast<double>(byte) } });
					byte = static_cast<char>(mFileContent.get()); // get the argument for the INSTRUCTION
					code.emplace_back({ VM::Type::INT,{ static_cast<double>(byte) } });
					break;
				case FPUSH:
					code.emplace_back({ VM::Type::INT,{ static_cast<double>(byte) } });
					byte = static_cast<char>(mFileContent.get()); // get the argument for the INSTRUCTION
					code.emplace_back({ VM::Type::FLOAT,{ static_cast<double>(byte) } }); //argument differs in type compared to the rest
					break;
				case CALL:
					code.emplace_back({ VM::Type::INT,{ static_cast<double>(byte) } });
					byte = static_cast<char>(mFileContent.get()); // get the first argument for the INSTRUCTION
					code.emplace_back({ VM::Type::INT,{ static_cast<double>(byte) } });
					byte = static_cast<char>(mFileContent.get()); // get the second argument for the INSTRUCTION
					code.emplace_back({ VM::Type::INT,{ static_cast<double>(byte) } });

					break;
				default:
					code.emplace_back({ VM::Type::INT,{ static_cast<double>(byte) } });
					break;
				}

			}
		}

	};
}