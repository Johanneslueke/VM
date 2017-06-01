#include "VM.h"




#include "ByteCode.h"



namespace vm
{
	Instruction::Instruction(const std::string& OpCode, int Value, int operands)
		: mName(OpCode), mNumericName(Value), mOperandCount(operands)
	{

	}
	Instruction::Instruction(const std::string& OpCode, int Value)
		: mName(OpCode), mNumericName(Value), mOperandCount(0)
	{

	}

	Instruction::Instruction(const std::string& OpCode, int Value, int operands, std::function<void(VM&)> lambda)
		: mName(OpCode), mNumericName(Value), mOperandCount(operands), mInstruction(lambda)
	{

	}
	Instruction::Instruction(const std::string& OpCode, int Value, std::function<void(VM&)> lambda)
		: mName(OpCode), mNumericName(Value), mOperandCount(0), mInstruction(lambda)
	{

	}



	


}