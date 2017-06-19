



#include "VM.h"
#include "ByteCode.h"



namespace vm
{
	std::unique_ptr<Instruction[]> InstructionCode = std::make_unique<Instruction[]>(MAXCODE);;

	Instruction::Instruction(const std::string& OpCode, int Value, int operands)
		: mName(OpCode), mNumericName(Value), mOperandCount(operands)
	{
		InstructionCode[Value] = (*this);
	}
	Instruction::Instruction(const std::string& OpCode, int Value)
		: mName(OpCode), mNumericName(Value), mOperandCount(0)
	{
		InstructionCode[Value] = (*this);
	}

	Instruction::Instruction(const std::string& OpCode, int Value, int operands, std::function<void()> lambda)
		: mName(OpCode), mNumericName(Value), mOperandCount(operands), mInstruction(lambda)
	{
		InstructionCode[Value] = (*this);
	}
	Instruction::Instruction(const std::string& OpCode, int Value, std::function<void()> lambda)
		: mName(OpCode), mNumericName(Value), mOperandCount(0), mInstruction(lambda)
	{
		InstructionCode[Value] = (*this);
	}


	void Instruction::Init( VM* Machine)
	{
		Instruction("NOP", 0, []() {});
		Instruction("IADD", IADD, [Machine]( ) {

			auto rhs = Machine->stack[Machine->stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
												

			Machine->stack.pop_back();
			Machine->stack.pop_back();
			Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mValue + rhs.mValue.mValue) } });
			Machine->stackPointer++;

		});
		Instruction("ISUB", ISUB, [Machine]( ) {

			auto rhs = Machine->stack[Machine->stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand


			Machine->stack.pop_back();
			Machine->stack.pop_back();
			Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mValue - rhs.mValue.mValue) } });
			Machine->stackPointer++;

		});
		Instruction("IMUL", IMUL, [Machine]( ) {

			auto rhs = Machine->stack[Machine->stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand


			Machine->stack.pop_back();
			Machine->stack.pop_back();
			Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mValue * rhs.mValue.mValue) } });
			Machine->stackPointer++;

		});
		Instruction("IEQ", IEQ, [Machine]( ) {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
				
				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{(double) (lhs.mValue.mValue == rhs.mValue.mValue) } });
				Machine->stackPointer++;

			}

		});
		Instruction("ILT", ILT, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
													

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (double)(lhs.mValue.mValue < rhs.mValue.mValue) } });
				Machine->stackPointer++;

			}
		});
		Instruction("IPUSH", IPUSH, 1, [Machine]() {
			{
				auto value = Machine->code[Machine->instructionPointer].mValue.mValue;
				Machine->instructionPointer++;
				Machine->stackPointer++;
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{ value } });
			}
		});

		Instruction("FADD", FADD, [Machine]() {
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand
	

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::FLOAT,{ (lhs.mValue.mValue + rhs.mValue.mValue) } });
				Machine->stackPointer++;
		});
		Instruction("FSUB", FSUB, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::FLOAT,{ (lhs.mValue.mValue - rhs.mValue.mValue) } });
				Machine->stackPointer++;

			}

		});
		Instruction("FMUL", FMUL, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::FLOAT,{ (lhs.mValue.mValue * rhs.mValue.mValue) } });
				Machine->stackPointer++;

			}

		});

		Instruction("FEQ", FEQ, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{ (double)(lhs.mValue.mValue == rhs.mValue.mValue) } });
				Machine->stackPointer++;

			}

		});
		Instruction("FLT", FLT, [Machine]() {

			{
				auto rhs = Machine->stack[Machine->stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine->stack[Machine->stackPointer--];   //Left-Hand-Side operand

				Machine->stack.pop_back();
				Machine->stack.pop_back();
				Machine->stack.push_back(VM::Type{ VM::Type::INT,{(double) (lhs.mValue.mValue < rhs.mValue.mValue) } });
				Machine->stackPointer++;

			}

		});

		Instruction("FPUSH", FPUSH, 1, [Machine]() {
			{
				auto value = Machine->code[Machine->instructionPointer].mValue.mValue;
				Machine->instructionPointer++;
				Machine->stackPointer++;
				Machine->stack.push_back(VM::Type{ VM::Type::FLOAT,{ value } });
			}
		});
		Instruction("Print", PRINT, [Machine]() {
			{
				std::cout << Machine->stack[Machine->stackPointer].mValue.mValue << "\n";
				Machine->stack.pop_back();
				Machine->stackPointer--;
			}
		});
		Instruction("HALT", HALT, [Machine]()
		{
			Machine->stats.doStats();;
			Machine->instructionPointer = Machine->code.size();

			return;
		});
		Instruction("POP", POP, [Machine]() {
			{
				Machine->stack.pop_back();
				Machine->stackPointer--;
			}

		});

		Instruction("GSTORE", GSTORE, 1, [Machine]() { //TODO(jojo): Add Type checking !!!!
			{
				auto addr = Machine->code[Machine->instructionPointer++];
				Machine->globals[addr.mValue.mPointer] = Machine->stack[Machine->stackPointer--];
				Machine->stack.pop_back();
			}

		});
		Instruction("GLOAD", GLOAD, 1, [Machine]() {
			{
				auto addr = Machine->code[Machine->instructionPointer++];
				auto val = Machine->globals[addr.mValue.mPointer];
				Machine->stack.push_back(val);
				Machine->stackPointer++;
			}
		});
		Instruction("STORE", STORE, 1, [Machine]() {
			{
				auto offset = Machine->code[Machine->instructionPointer++];
				Machine->stack[Machine->framePointer + offset.mValue.mPointer] = Machine->popOffStack();
			}
		});
		Instruction("LOAD", LOAD, 1, [Machine]() {
			{
				auto offset = Machine->code[Machine->instructionPointer++];
				if (offset.mObjecttype == VM::Type::INT)
					offset = VM::Type{ VM::Type::INT,offset.mValue.mValue };

				Machine->pushOntoStack(Machine->stack[Machine->framePointer + offset.mValue.mValue]);
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
				if (static_cast<bool>(Machine->popOffStack().mValue.mValue) == true) // Check if the condition to branch is TRUE and toss value away afterwards
					Machine->instructionPointer = addr.mValue.mPointer;

			}
		});
		Instruction("BRFALSE", BRFALSE, 1, [Machine]() {
			{
				auto addr = Machine->code[Machine->instructionPointer++];
				if (addr.mObjecttype != VM::Type::POINTER)
					throw std::runtime_error("INSTR: BRTRUE -> Expected Pointer rececived [ " + std::to_string(addr.mObjecttype) + " ]");
				if (static_cast<bool>(Machine->popOffStack().mValue.mValue) == false) // Check if the condition to branch is TRUE and toss value away afterwards
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

				for (int i = 0; i < nargs.mValue.mValue; i++) // pop all arguments of the stack
					Machine->popOffStack();

				Machine->pushOntoStack(retVal); // leave the return Value on the stack
			}
		});
	}


	


}