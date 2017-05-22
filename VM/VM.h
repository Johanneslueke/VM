//
// Created by johannes on 20.05.17.
//

#ifndef UTILITY_VM_H
#define UTILITY_VM_H

#include <vector>
#include <valarray>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "ByteCode.h"
#define INSTR(CODE,VAL,TYPE) Type{vm::VM::Type::INT,	{CODE}	}, \
							 Type{vm::VM::Type::TYPE,	{VAL}	}
#define DINSTR(CODE,VAL1,VAL2,TYPE1,TYPE2) Type{vm::VM::Type::INT,		{CODE}	},\
										   Type{vm::VM::Type::TYPE1,	{VAL1}	},\
										   Type{vm::VM::Type::TYPE2,	{VAL2}	}

#define SINSTR(CODE) Type{vm::VM::Type::INT,{CODE}}
#define PINSTR(CODE) Type{vm::VM::Type::INT,{CODE}}

namespace vm
{
	class VM
	{
	public:
		struct Type {
			enum types { INT, POINTER, FLOAT, STRING, CHAR } mObjecttype;

			union Value {
				long  mInteger;
				size_t mPointer;
				char mCharacter;
				double mFloat;
				char* mString;
				Value() = default;
				Value(int  i) : mInteger(i) {};
				Value(size_t i) : mPointer(i) {};
				Value(char c) : mCharacter(c) {};
				Value(double f) : mFloat(f) {};
				Value(char* str) : mString(str) {};

			}mValue;

			Type() = default;
			Type(types t, Value val) : mObjecttype(t), mValue(val)
			{

			}
			Type(const Type& t) : mObjecttype(t.mObjecttype), mValue(t.mValue)
			{

			}

		};
	public:
		typedef std::vector<Type> Memory;
	private:
		Memory        globals;
		Memory        stack;
		Memory        code;

		int                     instructionPointer = -1;
		int                     stackPointer = -1;
		int                     framePointer = -1;

		bool                    trace = true;

		void					pushOntoStack(Type T)
		{
			stack.push_back(T);
			stackPointer++;
		}

		auto popOffStack()
		{
			auto tmp = stack[stackPointer--];
			stack.pop_back();
			return tmp;
		}

		size_t popPointer()
		{
			auto tmp =  popOffStack();
			if (tmp.mObjecttype != Type::POINTER)
				 throw std::runtime_error("Tried to access a pointer but received:" + std::to_string(tmp.mObjecttype) + "!");
			return tmp.mValue.mPointer;
		}

	public:

		VM(const std::vector<Type>& code, int main) : globals(1000)
		{
			this->code = code;
			this->instructionPointer = main;
		}


		void operator()()
		{
			cpu();
		}


		void cpu()
		{
			if (trace)
				std::cout << std::left << std::setw(44) << "Instruction & Results:" << std::right << "Stack Memory:\n";
			while (instructionPointer <= code.size())
			{
				auto opcode = code[instructionPointer].mValue.mInteger; // fetch OpCode
				if (trace) {
					std::cout << std::left << std::setw(40) << disassemble() << std::right
						<< stackString() << "\n";
				}


				instructionPointer++;

				switch (opcode) {
				case IPUSH: {
					auto value = code[instructionPointer].mValue.mInteger;
					instructionPointer++;
					stackPointer++;
					stack.push_back(Type{ Type::INT,{ value } });
					break;
				}
				case FPUSH: {
					auto value = code[instructionPointer].mValue.mFloat;
					instructionPointer++;
					stackPointer++;
					stack.push_back(Type{ Type::FLOAT,{ value } });
					break;
				}
				case PRINT: {
					std::cout << stack[stackPointer].mValue.mInteger << "\n";
					stack.pop_back();
					stackPointer--;
					break;
				}
				case IEQ: {
					auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
					auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
														//Implicit typecasting
					if (rhs.mObjecttype == Type::FLOAT)
						rhs = Type{ Type::INT,(long )rhs.mValue.mFloat };
					if (lhs.mObjecttype == Type::FLOAT)
						lhs = Type{ Type::INT,(long )rhs.mValue.mFloat };

					stack.pop_back();
					stack.pop_back();
					stack.push_back(Type{ Type::INT,{ (long )(lhs.mValue.mInteger == rhs.mValue.mInteger) } });
					stackPointer++;
					break;
				}
				case ILT: {
					auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
					auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
														//Implicit typecasting
					if (rhs.mObjecttype == Type::FLOAT)
						rhs = Type{ Type::INT,(long )rhs.mValue.mFloat };
					if (lhs.mObjecttype == Type::FLOAT)
						lhs = Type{ Type::INT,(long )rhs.mValue.mFloat };

					stack.pop_back();
					stack.pop_back();
					stack.push_back(Type{ Type::INT,{ (long )(lhs.mValue.mInteger < rhs.mValue.mInteger) } });
					stackPointer++;
					break;
				}
				case IADD: {
					auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
					auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
														//Implicit typecasting
					if (rhs.mObjecttype == Type::FLOAT)
						rhs = Type{ Type::INT,(long )rhs.mValue.mFloat };
					if (lhs.mObjecttype == Type::FLOAT)
						lhs = Type{ Type::INT,(long )rhs.mValue.mFloat };

					stack.pop_back();
					stack.pop_back();
					stack.push_back(Type{ Type::INT,{ (lhs.mValue.mInteger + rhs.mValue.mInteger) } });
					stackPointer++;
					break;
				}
				case ISUB: {
					auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
					auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
														//Implicit typecasting
					if (rhs.mObjecttype == Type::FLOAT)
						rhs = Type{ Type::INT,(long )rhs.mValue.mFloat };
					if (lhs.mObjecttype == Type::FLOAT)
						lhs = Type{ Type::INT,(long )rhs.mValue.mFloat };

					stack.pop_back();
					stack.pop_back();
					stack.push_back(Type{ Type::INT,{ (lhs.mValue.mInteger - rhs.mValue.mInteger) } });
					stackPointer++;
					break;
				}
				case IMUL: {
					auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
					auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
														//Implicit typecasting
					if (rhs.mObjecttype == Type::FLOAT)
						rhs = Type{ Type::INT,(long )rhs.mValue.mFloat };
					if (lhs.mObjecttype == Type::FLOAT)
						lhs = Type{ Type::INT,(long )rhs.mValue.mFloat };

					stack.pop_back();
					stack.pop_back();
					stack.push_back(Type{ Type::INT,{ (lhs.mValue.mInteger*rhs.mValue.mInteger) } });
					stackPointer++;
					break;
				}
				case FADD: {
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
					break;
				}
				case FSUB: {
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
					break;
				}
				case FMUL: {
					auto rhs = stack[stackPointer--]; //Right-Hand-Side operand
					auto lhs = stack[stackPointer--];   //Left-Hand-Side operand
														//Implicit typecasting
					if (rhs.mObjecttype == Type::INT)
						rhs = Type{ Type::FLOAT,(float)rhs.mValue.mInteger };
					if (lhs.mObjecttype == Type::INT)
						lhs = Type{ Type::FLOAT,(float)rhs.mValue.mInteger };

					stack.pop_back();
					stack.pop_back();
					stack.push_back(Type{ Type::FLOAT,{ (lhs.mValue.mFloat*rhs.mValue.mFloat) } });
					stackPointer++;
					break;
				}
				case POP:
					stack.pop_back();
					stackPointer--;
					break;
				case GSTORE: {
					auto addr = code[instructionPointer++];
					globals[addr.mValue.mInteger] = stack[stackPointer--];
					stack.pop_back();
					break;
				}
				case GLOAD: {
					auto addr = code[instructionPointer++];
					auto val = globals[addr.mValue.mInteger];
					stack.push_back(val);
					stackPointer++;
					break;
				}
				case CALL: {
					auto func = code[instructionPointer++];
					auto nargs = code[instructionPointer++];

					if (func.mObjecttype != Type::POINTER)
						throw std::runtime_error("INSTR: CALL -> operand is not an Pointer to an Object/Function!!!");

					pushOntoStack(nargs);										// Save Number of arguments
					pushOntoStack(Type{ Type::POINTER,{ (size_t)framePointer } });		// Save Current FramePointer
					pushOntoStack(Type{ Type::POINTER,{ (size_t)instructionPointer } });// Save return adress 


					framePointer = stackPointer;
					instructionPointer = func.mValue.mPointer;	// Set InstructionPointer to the functions address


					break;
				}
				case RET: {
					auto retVal = popOffStack();		// return value of the function
					stackPointer = framePointer;		//// jump over locals to fp which points at ret addr

					instructionPointer = popPointer();  // pop return address, jump to it
					framePointer	   = popPointer();	// restore framepointer

					auto nargs = popOffStack();			//number of args to throw out
					if (nargs.mObjecttype != Type::INT)
						 throw std::runtime_error("INSTR: RET -> Number of arguments not a Number!!!");

					for (int i = 0; i < nargs.mValue.mInteger;i++) // pop all arguments of the stack
						popOffStack();

					pushOntoStack(retVal); // leave the return Value on the stack
						

					break;
				}
				case BRANCH: {
					auto addr = code[instructionPointer++];
					if (addr.mObjecttype != Type::POINTER)
						throw std::runtime_error("INSTR: BRANCH -> Expected Pointer rececived [" + std::to_string(addr.mObjecttype) + "]");
					instructionPointer = addr.mValue.mPointer;
					break;
				}
				case BRTRUE: {
					auto addr = code[instructionPointer++];
					if (addr.mObjecttype != Type::POINTER)
						 throw std::runtime_error("INSTR: BRTRUE -> Expected Pointer rececived [ " + std::to_string(addr.mObjecttype) + " ]");
					if(popOffStack().mValue.mInteger == true) 
						instructionPointer = addr.mValue.mPointer;
					break;
				}
				case BRFALSE: {
					auto addr = code[instructionPointer++];
					if (addr.mObjecttype != Type::POINTER)
						throw std::runtime_error("INSTR: BRFALSE -> Expected Pointer rececived [" + std::to_string(addr.mObjecttype) + "]");
					if (popOffStack().mValue.mInteger == false)
						instructionPointer = addr.mValue.mPointer;
					break;
				}
				case LOAD: {
					auto offset = code[instructionPointer++];
					pushOntoStack(stack[framePointer + offset.mValue.mInteger]);
					break;
				}
				case STORE: {
					auto offset = code[instructionPointer++];
					stack[framePointer + offset.mValue.mInteger] = popOffStack();
					break;
				}
				case HALT:
					return;
				default: {
					if (opcode < MAXCODE)
						if (opcode < 0)
							throw std::runtime_error("Opcode Mismatch");
						throw std::runtime_error("Opcode not implemented yet: "
							+ InstructionCode[opcode]->mName
							+ "! at Instruction Pointer("
							+ std::to_string(instructionPointer - 1) + ").");
					throw std::runtime_error("Opcode not supported: "
						+ std::to_string(opcode)
						+ "! at Instruction Pointer("
						+ std::to_string(instructionPointer - 1) + ").");
				}
				}
			}
		}

		std::string disassemble() const {
			std::stringstream buffer;
			int opcode = code[instructionPointer].mValue.mInteger;
			if (opcode != 0 && opcode < MAXCODE) //Print Code section
			{
				buffer << std::showbase << std::setbase(16)
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

		std::string stackString() const {
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
	};

}

#endif //UTILITY_VM_H