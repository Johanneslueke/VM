



#include "VM.h"
#include "ByteCode.h"



namespace vm
{
	std::unique_ptr<Instruction> InstructionCode[MAXCODE-1];

	Instruction::Instruction(const std::string& OpCode, int Value, int operands)
		: mName(OpCode), mNumericName(Value), mOperandCount(operands)
	{
		InstructionCode[Value] = std::make_unique< Instruction>(*this);
	}
	Instruction::Instruction(const std::string& OpCode, int Value)
		: mName(OpCode), mNumericName(Value), mOperandCount(0)
	{
		InstructionCode[Value] = std::make_unique< Instruction>(*this);
	}

	Instruction::Instruction(const std::string& OpCode, int Value, int operands, std::function<void(VM&)> lambda)
		: mName(OpCode), mNumericName(Value), mOperandCount(operands), mInstruction(lambda)
	{
		InstructionCode[Value] = std::make_unique< Instruction>(*this);
	}
	Instruction::Instruction(const std::string& OpCode, int Value, std::function<void(VM&)> lambda)
		: mName(OpCode), mNumericName(Value), mOperandCount(0), mInstruction(lambda)
	{
		InstructionCode[Value] = std::make_unique< Instruction>(*this);
	}



	


}