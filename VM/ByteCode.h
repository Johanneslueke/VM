//
// Created by johannes on 20.05.17.
//

#ifndef UTILITY_BYTECODE_H
#define UTILITY_BYTECODE_H
#include <memory>
#include <functional>

namespace vm
{

	class VM;

	enum ByteCodes : unsigned long long {
		NOP,
		IADD,
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

	struct Instruction {

		std::string mName;
		int         mNumericName;
		int         mOperandCount;
		std::function<void()>	mInstruction;

		Instruction(const std::string& OpCode, int Value, int operands);
		Instruction(const std::string& OpCode, int Value);
		Instruction(const std::string& OpCode, int Value, int operands, std::function<void()> lambda);
		Instruction(const std::string& OpCode, int Value, std::function<void()> lambda);

		virtual ~Instruction() = default;


		static void Init( VM*);
		
	};

	
	
	extern std::unique_ptr<Instruction> InstructionCode[]; 


}

#endif //UTILITY_BYTECODE_H