//
// Created by johannes on 20.05.17.
//

#ifndef UTILITY_BYTECODE_H
#define UTILITY_BYTECODE_H
#include <memory>
namespace vm
{

	struct Instruction {

		std::string mName;
		int         mNumericName;
		int         mOperandCount;

		Instruction(const std::string& OpCode, int Value, int operands)
			: mName(OpCode), mNumericName(Value), mOperandCount(operands)
		{

		}
		Instruction(const std::string& OpCode, int Value)
			: mName(OpCode), mNumericName(Value), mOperandCount(0)
		{

		}
	};

	enum ByteCodes {
		IADD = 1,	
		ISUB,
		IMUL,
		ILT,		//Integer Less than
		IEQ,		//Integer EQUAL
		IPUSH,		//Integer
		FADD,
		FSUB,
		FMUL,
		FLT,		//Float Less than
		FEQ,		//Float EQUAL
		FPUSH,		//Floating Point
		PRINT,
		HALT,
		POP,
		GSTORE,		//Stores Value Globally
		GLOAD,
		STORE,		//Stores Value Locally
		LOAD,
		BRANCH,		// unconditional branch
		BRTRUE,		// conditional branches
		BRFALSE,
		CALL,		//Function call
		RET,		//Returns to the Point after the Call

		MAXCODE
	};
	
	std::unique_ptr<Instruction> InstructionCode[] = {
		nullptr,
		std::make_unique< Instruction>("IADD",IADD),
		std::make_unique< Instruction>("ISUB",ISUB),
		std::make_unique< Instruction>("IMUL",IMUL),
		std::make_unique< Instruction>("IEQ",IEQ),
		std::make_unique< Instruction>("ILT",ILT),
		std::make_unique< Instruction>("IPUSH",IPUSH,1),

		std::make_unique< Instruction>("FADD",FADD),
		std::make_unique< Instruction>("FSUB",FSUB),
		std::make_unique< Instruction>("FMUL",FMUL),
		std::make_unique< Instruction>("FEQ",FEQ),
		std::make_unique< Instruction>("FLT",FLT),
		std::make_unique< Instruction>("FPUSH",FPUSH,1),

		std::make_unique< Instruction>("Print",PRINT),
		std::make_unique< Instruction>("HALT",HALT),
		std::make_unique< Instruction>("POP",POP),

		std::make_unique< Instruction>("GSTORE",GSTORE,1),
		std::make_unique< Instruction>("GLOAD",GLOAD,1),
		std::make_unique< Instruction>("STORE",STORE,1),
		std::make_unique< Instruction>("LOAD",LOAD,1),

		std::make_unique< Instruction>("BRANCH",BRANCH,1),
		std::make_unique< Instruction>("BRTRUE",BRTRUE,1),
		std::make_unique< Instruction>("BRFALSE",BRFALSE,1),
		std::make_unique< Instruction>("CALL",CALL,2),
		std::make_unique< Instruction>("RET",RET),



	};


}

#endif //UTILITY_BYTECODE_H