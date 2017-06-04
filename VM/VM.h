//
// Created by johannes on 20.05.17.
//

#ifndef UTILITY_VM_H
#define UTILITY_VM_H
#include <vector>

#define INSTR(CODE,VAL,TYPE) Type{vm::VM::Type::INT,	{(double)CODE}	}, \
							 Type{vm::VM::Type::TYPE,	{VAL}				}


#define DINSTR(CODE,VAL1,VAL2,TYPE1,TYPE2) Type{vm::VM::Type::INT,		{(double)CODE}	},\
										   Type{vm::VM::Type::TYPE1,	{VAL1}	},\
										   Type{vm::VM::Type::TYPE2,	{VAL2}	}

#define SINSTR(CODE) Type{vm::VM::Type::INT,{(double)CODE}}
#define PINSTR(CODE) Type{vm::VM::Type::INT,{(double)CODE}}

#define Bytes(Value)     (Value)
#define Kilobytes(Value) (Bytes(Value)*1024ULL)
#define Megabytes(Value) (Kilobytes(Value)*1024ULL)
#define Gigabytes(Value) (Megabytes(Value)*1024ULL)
#define Terabytes(Value) (Gigabytes(Value)*1024ULL)

template<typename ArrayType>
struct deleter {
	void operator()(ArrayType* array) {
		if (array)
		{
			delete[] array;
			array = nullptr;
		}
	}
};


#include "Measurement.h"
namespace vm
{
	class VM
	{
	public:
		struct Type {
			enum types  { INT, POINTER, FLOAT, STRING, CHAR } mObjecttype;

			union Value {
				double	mValue;
				size_t	mPointer;

				Value() = default;
				Value(double  i) : mValue(i) {};
				Value(size_t i) : mPointer(i) {};

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
		typedef std::unique_ptr<char[]>			  HeapMemory;

		

	private:
		friend Instruction;			//<-- Ugly but it works, to my surprise
		Memory        globals;
		Memory        stack;
		Memory        code;
		HeapMemory	  heap;
		Statistics	  stats;

		size_t                     instructionPointer = -1;
		long                       stackPointer = -1;
		size_t                     framePointer = -1;

		bool					   trace = false;

		
		void	pushOntoStack(Type T);
		Type	popOffStack();
		size_t	popPointer();

		size_t  HeapSize() const { return Kilobytes(512); }
		

	public:

		VM(const std::vector<Type>& code, int main);
		virtual ~VM();


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