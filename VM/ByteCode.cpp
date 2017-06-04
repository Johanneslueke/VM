



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

	Instruction::Instruction(const std::string& OpCode, int Value, int operands, std::function<void()> lambda)
		: mName(OpCode), mNumericName(Value), mOperandCount(operands), mInstruction(lambda)
	{
		InstructionCode[Value] = std::make_unique< Instruction>(*this);
	}
	Instruction::Instruction(const std::string& OpCode, int Value, std::function<void()> lambda)
		: mName(OpCode), mNumericName(Value), mOperandCount(0), mInstruction(lambda)
	{
		InstructionCode[Value] = std::make_unique< Instruction>(*this);
	}








	void Instruction::Init( VM* Machine)
	{
		InstructionCode[0] = nullptr;
		Instruction("IADD", IADD, [Machine]( ) {

			auto rhs = Machine->stack[Machine->stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand

																//Implicit typecasting Float -> Integer
			if (rhs.mObjecttype == VM::Type::FLOAT)
				rhs = VM::Type{ VM::Type::INT,(long long)rhs.mValue.mFloat };
			if (lhs.mObjecttype == VM::Type::FLOAT)
				lhs = VM::Type{ VM::Type::INT,(long long)lhs.mValue.mFloat };

			Machine->stack.pop_back();
			Machine->stack.pop_back();
			Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mInteger + rhs.mValue.mInteger) } });
			Machine->stackPointer++;

		});
		Instruction("ISUB", ISUB, [Machine]( ) {

			auto rhs = Machine->stack[Machine->stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand

																//Implicit typecasting Float -> Integer
			if (rhs.mObjecttype == VM::Type::FLOAT)
				rhs = VM::Type{ VM::Type::INT,(long long)rhs.mValue.mFloat };
			if (lhs.mObjecttype == VM::Type::FLOAT)
				lhs = VM::Type{ VM::Type::INT,(long long)lhs.mValue.mFloat };

			Machine->stack.pop_back();
			Machine->stack.pop_back();
			Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mInteger - rhs.mValue.mInteger) } });
			Machine->stackPointer++;

		});
		Instruction("IMUL", IMUL, [Machine]( ) {

			auto rhs = Machine->stack[Machine->stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand

																//Implicit typecasting Float -> Integer
			if (rhs.mObjecttype == VM::Type::FLOAT)
				rhs = VM::Type{ VM::Type::INT,(long long)rhs.mValue.mFloat };
			if (lhs.mObjecttype == VM::Type::FLOAT)
				lhs = VM::Type{ VM::Type::INT,(long long)lhs.mValue.mFloat };

			Machine->stack.pop_back();
			Machine->stack.pop_back();
			Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mInteger * rhs.mValue.mInteger) } });
			Machine->stackPointer++;

		});
		Instruction("IEQ", IEQ, [Machine]( ) {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
																	//Implicit typecasting
				if (rhs.mObjecttype == VM::Type::FLOAT)
					rhs = VM::Type{ VM::Type::INT,(long long)rhs.mValue.mFloat };
				if (lhs.mObjecttype == VM::Type::FLOAT)
					lhs = VM::Type{ VM::Type::INT,(long long)lhs.mValue.mFloat };

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (long long)(lhs.mValue.mInteger == rhs.mValue.mInteger) } });
				Machine->stackPointer++;

			}

		});
		Instruction("ILT", ILT, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == VM::Type::FLOAT)
					rhs = VM::Type{ VM::Type::INT,(long long)rhs.mValue.mFloat };
				if (lhs.mObjecttype == VM::Type::FLOAT)
					lhs = VM::Type{ VM::Type::INT,(long long)lhs.mValue.mFloat };

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (long long)(lhs.mValue.mInteger < rhs.mValue.mInteger) } });
				Machine->stackPointer++;

			}
		});
		Instruction("IPUSH", IPUSH, 1, [Machine]() {
			{
				auto value = Machine->code[Machine->instructionPointer].mValue.mInteger;
				Machine->instructionPointer++;
				Machine->stackPointer++;
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{ value } });
			}
		});

		Instruction("FADD", FADD, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == VM::Type::INT)
					rhs = VM::Type{ VM::Type::FLOAT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == VM::Type::INT)
					lhs = VM::Type{ VM::Type::FLOAT,(float)lhs.mValue.mInteger };

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::FLOAT,{ (lhs.mValue.mFloat + rhs.mValue.mFloat) } });
				Machine->stackPointer++;

			}

		});
		Instruction("FSUB", FSUB, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == VM::Type::INT)
					rhs = VM::Type{ VM::Type::FLOAT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == VM::Type::INT)
					lhs = VM::Type{ VM::Type::FLOAT,(float)lhs.mValue.mInteger };

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::FLOAT,{ (lhs.mValue.mFloat - rhs.mValue.mFloat) } });
				Machine->stackPointer++;

			}

		});
		Instruction("FMUL", FMUL, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == VM::Type::INT)
					rhs = VM::Type{ VM::Type::FLOAT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == VM::Type::INT)
					lhs = VM::Type{ VM::Type::FLOAT,(float)lhs.mValue.mInteger };

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::FLOAT,{ (lhs.mValue.mFloat * rhs.mValue.mFloat) } });
				Machine->stackPointer++;

			}

		});

		Instruction("FEQ", FEQ, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == VM::Type::INT)
					rhs = VM::Type{ VM::Type::INT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == VM::Type::INT)
					lhs = VM::Type{ VM::Type::INT,(float)lhs.mValue.mInteger };

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (long long)(lhs.mValue.mInteger == rhs.mValue.mInteger) } });
				Machine->stackPointer++;

			}

		});
		Instruction("FLT", FLT, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == VM::Type::INT)
					rhs = VM::Type{ VM::Type::INT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == VM::Type::INT)
					lhs = VM::Type{ VM::Type::INT,(float)lhs.mValue.mInteger };

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (long long)(lhs.mValue.mInteger < rhs.mValue.mInteger) } });
				Machine->stackPointer++;

			}

		});

		Instruction("FPUSH", FPUSH, 1, [Machine]() {
			{
				auto value = Machine->code[Machine->instructionPointer].mValue.mFloat;
				Machine->instructionPointer++;
				Machine->stackPointer++;
				Machine->stack.push_back(VM::Type{ VM::Type::FLOAT,{ value } });
			}
		});
		Instruction("Print", PRINT, [Machine]() {
			{
				std::cout << Machine->stack[Machine->stackPointer].mValue.mInteger << "\n";
				Machine->stack.pop_back();
				Machine->stackPointer--;
			}
		});
		Instruction("HALT", HALT, [Machine]()
		{
			Machine->stats.doStats();;
			Machine->~VM();
			return;
		});
		Instruction("POP", POP, [Machine]() {
			{
				Machine->stack.pop_back();
				Machine->stackPointer--;
			}

		});

		Instruction("GSTORE", GSTORE, 1, [Machine]() {
			{
				auto addr = Machine->code[Machine->instructionPointer++];
				Machine->globals[addr.mValue.mInteger] = Machine->stack[Machine->stackPointer--];
				Machine->stack.pop_back();
			}

		});
		Instruction("GLOAD", GLOAD, 1, [Machine]() {
			{
				auto addr = Machine->code[Machine->instructionPointer++];
				auto val = Machine->globals[addr.mValue.mInteger];
				Machine->stack.push_back(val);
				Machine->stackPointer++;
			}
		});
		Instruction("STORE", STORE, 1, [Machine]() {
			{
				auto offset = Machine->code[Machine->instructionPointer++];
				Machine->stack[Machine->framePointer + offset.mValue.mInteger] = Machine->popOffStack();
			}
		});
		Instruction("LOAD", LOAD, 1, [Machine]() {
			{
				auto offset = Machine->code[Machine->instructionPointer++];
				if (offset.mObjecttype == VM::Type::FLOAT)
					offset = VM::Type{ VM::Type::FLOAT,(long long)offset.mValue.mFloat };

				Machine->pushOntoStack(Machine->stack[Machine->framePointer + offset.mValue.mInteger]);
			}
		});

		Instruction("BRANCH", BRANCH, 1, [Machine]() {
			{
				auto addr = Machine->code[Machine->instructionPointer++];
				if (addr.mObjecttype != VM::Type::POINTER)
					throw std::runtime_error("INSTR: BRANCH -> Expected Pointer rececived [" + std::to_string(addr.mObjecttype) + "]");
				Machine->instructionPointer = addr.mValue.mPointer;

			}
		});
		Instruction("BRTRUE", BRTRUE, 1, [Machine]() {
			{
				auto addr = Machine->code[Machine->instructionPointer++];
				if (addr.mObjecttype != VM::Type::POINTER)
					throw std::runtime_error("INSTR: BRTRUE -> Expected Pointer rececived [ " + std::to_string(addr.mObjecttype) + " ]");
				if (static_cast<bool>(Machine->popOffStack().mValue.mInteger) == true) // Check if the condition to branch is TRUE and toss value away afterwards
					Machine->instructionPointer = addr.mValue.mPointer;

			}
		});
		Instruction("BRFALSE", BRFALSE, 1, [Machine]() {
			{
				auto addr = Machine->code[Machine->instructionPointer++];
				if (addr.mObjecttype != VM::Type::POINTER)
					throw std::runtime_error("INSTR: BRTRUE -> Expected Pointer rececived [ " + std::to_string(addr.mObjecttype) + " ]");
				if (static_cast<bool>(Machine->popOffStack().mValue.mInteger) == false) // Check if the condition to branch is TRUE and toss value away afterwards
					Machine->instructionPointer = addr.mValue.mPointer;

			}
		});
		Instruction("CALL", CALL, 2, [Machine]() {
			{
				auto func = Machine->code[Machine->instructionPointer++];
				auto nargs = Machine->code[Machine->instructionPointer++];

				if (func.mObjecttype != VM::Type::POINTER)
					throw std::runtime_error("INSTR: CALL -> operand is not an Pointer to an Object/Function!!!");

				Machine->pushOntoStack(nargs);										// Save Number of arguments
				Machine->pushOntoStack(VM::Type{ VM::Type::POINTER,{ (size_t)Machine->framePointer } });		// Save Current FramePointer
				Machine->pushOntoStack(VM::Type{ VM::Type::POINTER,{ (size_t)Machine->instructionPointer } });// Save return adress 


				Machine->framePointer = Machine->stackPointer;
				Machine->instructionPointer = func.mValue.mPointer;	// Set InstructionPointer to the functions address
			}
		});
		Instruction("RET", RET, [Machine]() {
			{
				auto retVal = Machine->popOffStack();		// return value of the function
				Machine->stackPointer = static_cast<long>(Machine->framePointer);		//// jump over locals to fp which points at ret addr

				Machine->instructionPointer = Machine->popPointer();  // pop return address, jump to it
				Machine->framePointer = Machine->popPointer();			// restore framepointer

				auto nargs = Machine->popOffStack();			//number of args to throw out

				if (nargs.mObjecttype != VM::Type::INT)
					throw std::runtime_error("INSTR: RET -> Number of arguments not a Number!!!");

				for (int i = 0; i < nargs.mValue.mInteger; i++) // pop all arguments of the stack
					Machine->popOffStack();

				Machine->pushOntoStack(retVal); // leave the return Value on the stack
			}
		});
	}


	


}