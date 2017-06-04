

#include <valarray>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "ByteCode.h"


#include "VM.h"



namespace vm
{
	VM::VM(const std::vector<Type>& code, int main) : globals(1000)
	{
		this->code = code;
		this->instructionPointer = main;

		Instruction::Init(this);
	}

	void VM::cpu()
	{
		if (trace)
			std::cout << std::left << std::setw(44) << "Instruction & Results:" << std::right << "Stack Memory:\n";
		while (instructionPointer <= code.size())
		{
			auto opcode = code[instructionPointer].mValue.mInteger; // fetch OpCode
			instructionPointer++;		

			if (opcode < 0)
				throw std::runtime_error("Opcode Mismatch No Negative instructions please ");
			else if(opcode > MAXCODE)
				throw std::runtime_error("Opcode not supported: "
					+ std::to_string(opcode)
					+ "! at Instruction Pointer("
					+ std::to_string(instructionPointer - 1) + ").");
			

			if (trace) {
				std::cout << std::left << std::setw(40) << disassemble() << std::right
					<< stackString() << "\n";
				auto avg = (measure<std::chrono::nanoseconds>::duration(InstructionCode[opcode]->mInstruction));
				
				stats.AddMeasurement({ InstructionCode[opcode]->mName, avg.count() });
			}
			else {
				stats.AddMeasurement({ InstructionCode[opcode]->mName,  (measure<std::chrono::nanoseconds>::duration(InstructionCode[opcode]->mInstruction)).count() });
			}	
		}
	}


	std::string VM::disassemble() const {
		std::stringstream buffer;
		int opcode = code[instructionPointer-1].mValue.mInteger;
		if (opcode != 0 && opcode < MAXCODE) //Print Code section
		{
			buffer << std::showbase << std::setbase(10)
				<< std::setw(4 * 2) << std::left << instructionPointer << std::dec << ": \t"
				<< std::setw(4 * 2) << std::left << InstructionCode[opcode]->mName;

			if (InstructionCode[opcode]->mOperandCount > 0) {
				auto nargs = InstructionCode[opcode]->mOperandCount;
				std::vector<std::string> operands;
				for (auto i = instructionPointer + 1; i <= instructionPointer + nargs; i++)
				{
					switch (code[i].mObjecttype)
					{
					case Type::INT:
						operands.push_back(std::to_string(code[i].mValue.mInteger));
						break;
					case Type::POINTER:
						operands.push_back(std::to_string(code[i].mValue.mPointer));
						break;
					case Type::FLOAT:
						operands.push_back(std::to_string(code[i].mValue.mFloat));
						break;
					case Type::STRING:
						operands.push_back({ code[i].mValue.mString });
						break;
					}
				}

				for (auto item = 0; item < operands.size(); item++)
				{
					if (item > 0)
						buffer << std::left << ", ";

					buffer << std::setw(2) << std::left << " " << operands[item];
				}

			}
		}
		return buffer.str();
	}

	std::string VM::stackString() const {
		std::stringstream buffer;
		buffer << "\tstack[";
		for (int i = 0; i <= stackPointer; i++) {
			switch (stack[i].mObjecttype)
			{
			case Type::INT:
				buffer << " " << stack[i].mValue.mInteger;
				break;
			case Type::FLOAT:
				buffer << " " << stack[i].mValue.mFloat;
				break;
			case Type::STRING:
				buffer << " " << stack[i].mValue.mString;
				break;
			}

		}
		buffer << " ]";
		return buffer.str();
	}

	void VM::pushOntoStack(Type T)
	{
		stack.push_back(T);
		stackPointer++;
	}
	
	auto VM::popOffStack() -> VM::Type
	{
		auto tmp = stack[stackPointer--];
		stack.pop_back();
		return tmp;
	}

	size_t VM::popPointer()
	{
		auto tmp = popOffStack();
		if (tmp.mObjecttype != Type::POINTER)
			throw std::runtime_error("Tried to access a pointer but received:" + std::to_string(tmp.mObjecttype) + "!");
		return tmp.mValue.mPointer;
	}
}