//
// Created by johannes on 20.05.17.
//

#ifndef UTILITY_VM_H
#define UTILITY_VM_H
#include <vector>
#include <variant>

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



#include "Measurement.h"
namespace vm
{
	
	
	class VM
	{
	public:
		struct Type {
			enum types  { INT, POINTER, FLOAT, CHAR } mObjecttype;

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
		using TYPE = std::variant<double, bool, size_t>;
		using VariantMemory =  std::vector<TYPE>;

		typedef std::vector<Type>				  Memory;		
		typedef std::unique_ptr<char[]>			  HeapMemory;	
		

	private:
		friend		  Instruction;			//<-- Ugly but it works, to my surprise

		//Memory of the Machine
		//HeapMemory	  heap;	
		Memory        stack;
		Memory        globals;

			
		Memory        code;					//!<-- Copy of the Code to execute
		Statistics	  stats;				//!<-- Does Statistics about the execution of the Code


		//Rudimentary special purpose register 
		size_t                     instructionPointer = -1;
		size_t                     stackPointer = -1;
		size_t                     framePointer = -1;

		//Debug
		bool					   trace = true;

	protected:

		void	pushOntoStack(Type T);
		Type	popOffStack();
		size_t	popPointer();

		size_t  HeapSize() const { return Kilobytes(512); }

		std::string disassemble() const;
		std::string stackString() const;
		

	public:

		VM(const std::vector<Type>& code, int main);
		virtual ~VM();


		void cpu();
		void operator()()
		{
			cpu();
		}
	};
}



#endif //UTILITY_VM_H