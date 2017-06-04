#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <chrono>
#include "ByteCode.h"
#include "vm.h"


#include <stdio.h>
using namespace vm;
using namespace std::chrono;
using Type = vm::VM::Type;

#include "Measurement.h"


long long test(long long n);




int main() {

	Type CodeLoop[] = {
								
	   // N = 10						ADDRESS
		INSTR(IPUSH, 10LL,INT),			// 0  Push Constant (10) Onto Stack 
		INSTR(GSTORE, 0LL,INT),			// 2  Store StackTop{Constant (10)} Globally var(N)								 
		INSTR(IPUSH, 0LL,INT),			// 4  Push Constant (0) Onto Stack
		INSTR(GSTORE, 1LL,INT),			// 6  Store StackTop{Constant (10)} Globally var(i)
										// do{
		INSTR(GLOAD, 1ll,INT),			// 15 Load i
		INSTR(IPUSH, 1ll,INT),			// 17 Push Constant (1) Onto Stack 
		SINSTR(IADD),					// 19 i = i+1
		INSTR(GSTORE, 1ll,INT),
										// }while( i < N)
		INSTR(GLOAD, 1ll,INT),			// 20
		INSTR(GLOAD, 0ll,INT)	,			// 22
		SINSTR(ILT),					// 24
		INSTR(BRTRUE, 8ull,POINTER),		// 25;
										
		SINSTR(HALT)					// 26
	};

	Type factorial[] = {
		//Instruction						// ADDRESS
		///Function factorial(var N)		// ARGS=1, LOCALS=0		
		INSTR(LOAD, -3ll,INT),				// [0]		Copy Given Argument N Into Current StackFrame
		INSTR(IPUSH, 2ll,INT),				// [2]		Push Constant (2) Onto Stack
		SINSTR(ILT),						// [4]		IF ((N < 2) == TRUE) Push TRUE ELSE Push FALSE Onto Stack
		INSTR(BRFALSE, 10ull,POINTER),		// [5]		Jump to Addr 10 if on Stack lays FALSE
		INSTR(IPUSH, 1ll,INT),				// [7]		Push Constant (1) Onto Stack
		SINSTR(RET),						// [9]		Return Constant (1)[7]
											// 
											//			RETURN N * factorial( N-1 )
		INSTR(LOAD, -3ll,INT),				// [10]		Copy Given Argument N Into Current StackFrame
		INSTR(LOAD, -3ll,INT),				// [12]		Copy Given Argument N Into Current StackFrame
		INSTR(IPUSH, 1ll,INT),				// [14]		Push Constant (1) Onto Stack
		SINSTR(ISUB),						// [16]		Substract Constant (1) of Value (N) 
		DINSTR(CALL, 0ull, 1ll,POINTER,INT),	// [17]		Call Function: factorial(var N)
		SINSTR(IMUL),						// [20]		Multiplicate Result of (factorial(var N))[17] times Value (N)[10]
		SINSTR(RET),						// [21]		Return Result[20]
		/// Function Main()					// ARGS=0, LOCALS=0
		//									 <-- MAIN METHOD!
		INSTR(IPUSH, 25ll,INT),				// [22]		Push Constant (5) Onto StacK
		DINSTR(CALL, 0ull, 1ll,POINTER,INT),	// [25]		Call Function: factorial(var N)
		//SINSTR(PRINT),						// [26]		Print Stack Top
		SINSTR(HALT)						// [27]		Abort
	};



	VM::Memory CodeInstructions(std::begin(factorial), std::end(factorial));


	VM Machine(CodeInstructions, 22);


	try {
		///auto exe = &VM::cpu;
		auto avg = (measure<std::chrono::nanoseconds>::duration(Machine) );

		auto avg2 = (measure<std::chrono::nanoseconds>::duration(test,25));



		std::cout << "Average Time: " << avg.count() * std::pow(10, -6) << " ms\n";
		std::cout << "Average Time: " << avg2.count()* std::pow(10, -6) << " ms\n";
		std::cout << "Delta Time: " << (avg - avg2).count() * std::pow(10,-6)<<"ms\n";

		//Machine.cpu();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}

	std::cin.get();
	return 0;
}

long long test(long long n)
{
	if (n < 2)
		return 1;
	return (n*test(n - 1));
}