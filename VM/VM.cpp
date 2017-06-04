

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

		InstructionCode[0] = nullptr;
		Instruction("IADD", IADD, [](VM& Machine) {

			auto rhs = Machine.stack[Machine.stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine.stack[Machine.stackPointer--];   //Left-Hand-Side operand

																//Implicit typecasting Float -> Integer
			if (rhs.mObjecttype == VM::Type::FLOAT)
				rhs = VM::Type{ VM::Type::INT,(long long)rhs.mValue.mFloat };
			if (lhs.mObjecttype == VM::Type::FLOAT)
				lhs = VM::Type{ Type::INT,(long long)rhs.mValue.mFloat };

			Machine.stack.pop_back();
			Machine.stack.pop_back();
			Machine.stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mInteger + rhs.mValue.mInteger) } });
			Machine.stackPointer++;

		});
		Instruction("ISUB", ISUB,[](VM& Machine) {

			auto rhs = Machine.stack[Machine.stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine.stack[Machine.stackPointer--];   //Left-Hand-Side operand

																//Implicit typecasting Float -> Integer
			if (rhs.mObjecttype == VM::Type::FLOAT)
				rhs = VM::Type{ VM::Type::INT,(long long)rhs.mValue.mFloat };
			if (lhs.mObjecttype == VM::Type::FLOAT)
				lhs = VM::Type{ Type::INT,(long long)rhs.mValue.mFloat };

			Machine.stack.pop_back();
			Machine.stack.pop_back();
			Machine.stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mInteger - rhs.mValue.mInteger) } });
			Machine.stackPointer++;

		});
		Instruction("IMUL", IMUL,[](VM& Machine) {

			auto rhs = Machine.stack[Machine.stackPointer--];	//Right-Hand-Side operand
			auto lhs = Machine.stack[Machine.stackPointer--];   //Left-Hand-Side operand

																//Implicit typecasting Float -> Integer
			if (rhs.mObjecttype == VM::Type::FLOAT)
				rhs = VM::Type{ VM::Type::INT,(long long)rhs.mValue.mFloat };
			if (lhs.mObjecttype == VM::Type::FLOAT)
				lhs = VM::Type{ Type::INT,(long long)rhs.mValue.mFloat };

			Machine.stack.pop_back();
			Machine.stack.pop_back();
			Machine.stack.push_back(VM::Type{ VM::Type::INT,{ (lhs.mValue.mInteger * rhs.mValue.mInteger) } });
			Machine.stackPointer++;

		});
		Instruction("IEQ", IEQ, [](VM& Machine) {

			{
				auto rhs = Machine.stack[Machine.stackPointer--]; //Right-Hand-Side operand
				auto lhs = Machine.stack[Machine.stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == Type::FLOAT)
					rhs = Type{ Type::INT,(long long)rhs.mValue.mFloat };
				if (lhs.mObjecttype == Type::FLOAT)
					lhs = Type{ Type::INT,(long long)rhs.mValue.mFloat };

				Machine.stack.pop_back();
				Machine.stack.pop_back();
				Machine.stack.push_back(Type{ Type::INT,{ (long long)(lhs.mValue.mInteger == rhs.mValue.mInteger) } });
				Machine.stackPointer++;
			
			}
		
		});
		Instruction("ILT", ILT, [this](VM& ) {

			{
				auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
				auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == Type::FLOAT)
					rhs = Type{ Type::INT,(long long)rhs.mValue.mFloat };
				if (lhs.mObjecttype == Type::FLOAT)
					lhs = Type{ Type::INT,(long long)rhs.mValue.mFloat };

				stack.pop_back();
				stack.pop_back();
				stack.push_back(Type{ Type::INT,{ (long long)(lhs.mValue.mInteger < rhs.mValue.mInteger) } });
				stackPointer++;
				
			}
		});
		Instruction("IPUSH", IPUSH, 1, [this](VM&) {
			{
				auto value = this->code[instructionPointer].mValue.mInteger;
				instructionPointer++;
				stackPointer++;
				stack.push_back(Type{ Type::INT,{ value } });
			}
		});

		Instruction("FADD", FADD, [this](VM&) {

			{
				auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
				auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == Type::INT)
					rhs = Type{ Type::FLOAT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == Type::INT)
					lhs = Type{ Type::FLOAT,(float)rhs.mValue.mInteger };

				stack.pop_back();
				stack.pop_back();
				stack.push_back(Type{ Type::FLOAT,{ (lhs.mValue.mFloat + rhs.mValue.mFloat) } });
				stackPointer++;
				
			}
		
		});
		Instruction("FSUB", FSUB, [this](VM&) {

			{
				auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
				auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == Type::INT)
					rhs = Type{ Type::FLOAT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == Type::INT)
					lhs = Type{ Type::FLOAT,(float)rhs.mValue.mInteger };

				stack.pop_back();
				stack.pop_back();
				stack.push_back(Type{ Type::FLOAT,{ (lhs.mValue.mFloat - rhs.mValue.mFloat) } });
				stackPointer++;

			}

		});
		Instruction("FMUL", FMUL, [this](VM&) {

			{
				auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
				auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == Type::INT)
					rhs = Type{ Type::FLOAT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == Type::INT)
					lhs = Type{ Type::FLOAT,(float)rhs.mValue.mInteger };

				stack.pop_back();
				stack.pop_back();
				stack.push_back(Type{ Type::FLOAT,{ (lhs.mValue.mFloat * rhs.mValue.mFloat) } });
				stackPointer++;

			}

		});
		Instruction("FEQ", FEQ, [this](VM&) {
		
			{
				auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
				auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == Type::INT)
					rhs = Type{ Type::INT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == Type::INT)
					lhs = Type{ Type::INT,(float)rhs.mValue.mInteger };

				stack.pop_back();
				stack.pop_back();
				stack.push_back(Type{ Type::INT,{ (long long)(lhs.mValue.mInteger == rhs.mValue.mInteger) } });
				stackPointer++;
				
			}
		
		});
		Instruction("FLT", FLT, [this](VM&) {

			{
				auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
				auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
													//Implicit typecasting
				if (rhs.mObjecttype == Type::INT)
					rhs = Type{ Type::INT,(float)rhs.mValue.mInteger };
				if (lhs.mObjecttype == Type::INT)
					lhs = Type{ Type::INT,(float)rhs.mValue.mInteger };

				stack.pop_back();
				stack.pop_back();
				stack.push_back(Type{ Type::INT,{ (long long)(lhs.mValue.mInteger < rhs.mValue.mInteger) } });
				stackPointer++;
				
			}

		});

		Instruction("FPUSH", FPUSH, 1, [this](VM&) {
			{
				auto value = this->code[instructionPointer].mValue.mFloat;
				instructionPointer++;
				stackPointer++;
				stack.push_back(Type{ Type::FLOAT,{ value } });
			}
		});
		Instruction("Print", PRINT, [this](VM&) {
			{
				std::cout << stack[stackPointer].mValue.mInteger << "\n";
				stack.pop_back();
				stackPointer--;
			}
		});
		Instruction("HALT", HALT,[this](VM&)
		{
			this->stats.doStats();;
			this->~VM();
			return;
		});
		Instruction("POP", POP,  [this](VM&) {
		    {
				stack.pop_back();
				stackPointer--;
			}
		
		});

		Instruction("GSTORE", GSTORE, 1, [this](VM&) {
			{
				auto addr = this->code[instructionPointer++];
				globals[addr.mValue.mInteger] = stack[stackPointer--];
				stack.pop_back();
			}
		
		});
		Instruction("GLOAD", GLOAD, 1, [this](VM&) {
			{
				auto addr =this-> code[instructionPointer++];
				auto val = globals[addr.mValue.mInteger];
				stack.push_back(val);
				stackPointer++;
			}
		});
		Instruction("STORE", STORE, 1, [this](VM&) {
			{
				auto offset = this->code[instructionPointer++];
				stack[framePointer + offset.mValue.mInteger] = popOffStack();
			}
		});
		Instruction("LOAD", LOAD, 1, [this](VM&) {
			{
				auto offset = this->code[instructionPointer++];
				if (offset.mObjecttype == Type::FLOAT)
					offset = Type{ Type::FLOAT,(long long)offset.mValue.mFloat };

				pushOntoStack(stack[framePointer + offset.mValue.mInteger]);
			}
		});

		Instruction("BRANCH", BRANCH, 1, [this](VM&) {
			{
				auto addr = this->code[instructionPointer++];
				if (addr.mObjecttype != Type::POINTER)
					throw std::runtime_error("INSTR: BRANCH -> Expected Pointer rececived [" + std::to_string(addr.mObjecttype) + "]");
				instructionPointer = addr.mValue.mPointer;
				
			}
		});
		Instruction("BRTRUE", BRTRUE, 1,[this](VM&) {
			{
				auto addr = this->code[instructionPointer++];
				if (addr.mObjecttype != Type::POINTER)
					throw std::runtime_error("INSTR: BRTRUE -> Expected Pointer rececived [ " + std::to_string(addr.mObjecttype) + " ]");
				if (static_cast<bool>(popOffStack().mValue.mInteger) == true) // Check if the condition to branch is TRUE and toss value away afterwards
					instructionPointer = addr.mValue.mPointer;
				
			}
		});
		Instruction("BRFALSE", BRFALSE, 1, [this](VM&) {
			{
				auto addr = this->code[instructionPointer++];
				if (addr.mObjecttype != Type::POINTER)
					throw std::runtime_error("INSTR: BRTRUE -> Expected Pointer rececived [ " + std::to_string(addr.mObjecttype) + " ]");
				if (static_cast<bool>(popOffStack().mValue.mInteger) == false) // Check if the condition to branch is TRUE and toss value away afterwards
					instructionPointer = addr.mValue.mPointer;

			}
		});
		Instruction("CALL", CALL, 2, [this](VM&) {
			{
				auto func = this->code[instructionPointer++];
				auto nargs = this->code[instructionPointer++];

				if (func.mObjecttype != Type::POINTER)
					throw std::runtime_error("INSTR: CALL -> operand is not an Pointer to an Object/Function!!!");

				pushOntoStack(nargs);										// Save Number of arguments
				pushOntoStack(Type{ Type::POINTER,{ (size_t)framePointer } });		// Save Current FramePointer
				pushOntoStack(Type{ Type::POINTER,{ (size_t)instructionPointer } });// Save return adress 


				framePointer = stackPointer;
				instructionPointer = func.mValue.mPointer;	// Set InstructionPointer to the functions address
			}
		});
		Instruction("RET", RET, [this](VM&) {
			{
				auto retVal = popOffStack();		// return value of the function
				stackPointer = static_cast<long>(framePointer);		//// jump over locals to fp which points at ret addr

				instructionPointer = popPointer();  // pop return address, jump to it
				framePointer = popPointer();	// restore framepointer

				auto nargs = popOffStack();			//number of args to throw out
				if (nargs.mObjecttype != Type::INT)
					throw std::runtime_error("INSTR: RET -> Number of arguments not a Number!!!");

				for (int i = 0; i < nargs.mValue.mInteger; i++) // pop all arguments of the stack
					popOffStack();

				pushOntoStack(retVal); // leave the return Value on the stack
			}
		});
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
				auto avg = (measure<std::chrono::nanoseconds>::duration(InstructionCode[opcode]->mInstruction, *this));
				
				stats.AddMeasurement({ InstructionCode[opcode]->mName, avg.count() });
			}
			else {
				stats.AddMeasurement({ InstructionCode[opcode]->mName,  (measure<std::chrono::nanoseconds>::duration(InstructionCode[opcode]->mInstruction, *this)).count() });
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

	void	VM::pushOntoStack(Type T)
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