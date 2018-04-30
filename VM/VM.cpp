

#include <valarray>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "ByteCode.h"


#include "Variant_helpers.h"

#include "VM.h"

#include <optional>

namespace vm
{
	VM::VM(const std::vector<Type>& code, int main) : globals(10)
	{
		this->code = code;
		this->instructionPointer = main;

		Instruction::Init(this);

	}

	VM::~VM() {

		
		
	}

	size_t VM::fetch() {
		return code[instructionPointer++].mValue.mValue;// fetch OpCode
		//instructionPointer++;
	}

	void VM::validate_opcode(size_t opcode) {
		if (opcode < 0)
			throw std::runtime_error("Opcode Mismatch No Negative instructions please ");
		else if (opcode > MAXCODE)
			throw std::runtime_error("Opcode not supported: "
				+ std::to_string(opcode)
				+ "! at Instruction Pointer("
				+ std::to_string(instructionPointer - 1) + ").");
		else if (opcode == 0)
			throw std::runtime_error("Opcode 0 Does not exist yet!\n ");
	}

	auto VM::decode(size_t opcode) {
		return InstructionCode[static_cast<size_t>(opcode)].mOperandCount;
	}

	void VM::execute(size_t  opcode) {
		if (trace) {
			std::cout << std::left
				<< std::setw(40)
				<< disassemble()
				<< std::right
				<< stackString() << "\n";

			stats.AddMeasurement({ InstructionCode[static_cast<size_t>(opcode)].mName,
				(measure<std::chrono::nanoseconds>::
					duration(
						InstructionCode[static_cast<size_t>(opcode)].mInstruction
					)).count()
				});

		}
		else {
			stats.AddMeasurement({ InstructionCode[static_cast<size_t>(opcode)].mName,
				(measure<std::chrono::nanoseconds>::
					duration(
						InstructionCode[static_cast<size_t>(opcode)].mInstruction
					)).count()
				});
		}
	}

	void VM::cpu()
	{
		if (trace)
			std::cout << std::left << std::setw(44) << "Instruction & Results:" << std::right << "Stack Memory:\n";

		while (instructionPointer < code.size())
		{
			try {
				auto opcode = fetch();
				validate_opcode(opcode);
				auto arg = decode(opcode);
				execute(opcode);
				
			}
			catch (std::out_of_range& e)
			{
				std::cerr << e.what();
			}
			catch (std::exception& e)
			{
				std::cerr << e.what();
			}
			catch (...)
			{
				std::cerr << "Totally lost... do not know what happend but i'm dead for now!!!\n";
			}
			
				
		}

	}


	std::string VM::disassemble() const {
		std::stringstream buffer;
		auto opcode = static_cast<size_t>(code[instructionPointer-1].mValue.mValue);
		if (opcode != 0 && opcode < MAXCODE) //Print Code section
		{
			buffer << std::showbase << std::setbase(10)
				<< std::setw(4 * 2) << std::left << instructionPointer-1 << std::dec << ": \t"
				<< std::setw(4 * 2) << std::left << InstructionCode[static_cast<size_t>(opcode)].mName;

			if (InstructionCode[static_cast<size_t>(opcode)].mOperandCount > 0) {
				auto nargs = InstructionCode[static_cast<size_t>(opcode)].mOperandCount;
				std::vector<std::string> operands;
				for (auto i = instructionPointer ; i <= instructionPointer + nargs-1; i++)
				{
					switch (code[i].mObjecttype)
					{
					
					case Type::POINTER:
						operands.push_back(std::to_string(code[i].mValue.mPointer));
						break;
					default:
						operands.push_back(std::to_string(code[i].mValue.mValue));
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
		if (stack.size() != 0)
		{
			for (size_t i = 0; i <= stackPointer; ++i) {
				switch (stack[i].mObjecttype)
				{
				case Type::INT:
					buffer << " " << (long long)stack[i].mValue.mValue;
					break;
				case Type::FLOAT:
					buffer << " " << stack[i].mValue.mValue;
					break;
				}

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