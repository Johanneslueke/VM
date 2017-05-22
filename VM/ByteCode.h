//
// Created by johannes on 20.05.17.
//

#ifndef UTILITY_BYTECODE_H
#define UTILITY_BYTECODE_H

namespace vm
{

	class Instruction {
	public:
		std::string mName;
		int         mNumericName;
		int         mOperandCount;
	public:
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
		IADD = 1, //Integer
		ISUB,
		IMUL,
		ILT, //Integer Less than
		IEQ, //Integer EQUAL
		IPUSH,  //Floating Point
		FADD,
		FSUB,
		FMUL,
		FPUSH,
		PRINT,
		HALT,
		POP,
		GSTORE, //Stores Value Globally
		GLOAD,
		STORE, //Stores Value Locally
		LOAD,
		BRANCH, // unconditional branch
		BRTRUE, // conditional branches
		BRFALSE,
		CALL,  //Function call
		RET,

		MAXCODE
	};
	Instruction* InstructionCode[] = {
		nullptr,
		new Instruction("IADD",IADD),
		new Instruction("ISUB",ISUB),
		new Instruction("IMUL",IMUL),
		new Instruction("IEQ",IEQ),
		new Instruction("ILT",ILT),
		new Instruction("IPUSH",IPUSH,1),

		new Instruction("FADD",FADD),
		new Instruction("FSUB",FSUB),
		new Instruction("FMUL",FMUL),
		new Instruction("FPUSH",FPUSH,1),

		new Instruction("Print",PRINT),
		new Instruction("HALT",HALT),
		new Instruction("POP",POP),

		new Instruction("GSTORE",GSTORE,1),
		new Instruction("GLOAD",GLOAD,1),
		new Instruction("STORE",STORE,1),
		new Instruction("LOAD",LOAD,1),

		new Instruction("BRANCH",BRANCH,1),
		new Instruction("BRTRUE",BRTRUE,1),
		new Instruction("BRFALSE",BRFALSE,1),
		new Instruction("CALL",CALL,2),
		new Instruction("RET",RET),



	};


}

#endif //UTILITY_BYTECODE_H