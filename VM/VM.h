//
// Created by johannes on 20.05.17.
//

#ifndef UTILITY_VM_H
#define UTILITY_VM_H
#include <vector>

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
	public:
		Memory        globals;
		Memory        stack;
		Memory        code;

		size_t                     instructionPointer = -1;
		long                       stackPointer = -1;
		size_t                     framePointer = -1;

		bool                    trace = true;

		
		void	pushOntoStack(Type T);
		
		Type popOffStack();
		size_t popPointer();
		

	public:

		VM(const std::vector<Type>& code, int main);


		void cpu();
		void operator()()
		{
			cpu();
		}


		std::string disassemble() const;
		std::string stackString() const;


	};
}

#endif //UTILITY_VM_H