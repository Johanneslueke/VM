#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <chrono>
#include "ByteCode.h"
#include "vm.h"


#include <stdio.h>
using namespace vm;
using namespace std::chrono;
using Type = vm::VM::Type;

///////////////////////////////////////////////////////////////////////////
///////////////////time measuring mechanism////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#define fw(what) std::forward<decltype(what)>(what)

/**
* @ class measure
* @ brief Class to measure the execution time of a callable
*/
template <
	typename TimeT = std::chrono::nanoseconds, class ClockT = std::chrono::high_resolution_clock
>
struct measure
{
	/**
	* @ fn    execution
	* @ brief Returns the quantity (count) of the elapsed time as TimeT units
	*/
	template<typename F, typename ...Args>
	static typename TimeT::rep execution(F&& func, Args&&... args)
	{
		auto start = ClockT::now();

		fw(func)(std::forward<Args>(args)...);

		auto duration = std::chrono::duration_cast<TimeT>(ClockT::now() - start);

		return duration.count();
	}

	/**
	* @ fn    duration
	* @ brief Returns the duration (in chrono's type system) of the elapsed time
	*/
	template<typename F, typename... Args>
	static TimeT duration(F&& func, Args&&... args)
	{
		auto start = ClockT::now();

		fw(func)(std::forward<Args>(args)...);

		return std::chrono::duration_cast<TimeT>(ClockT::now() - start);
	}
};





int main() {

	Type CodeLoop[] = {
								
	   // N = 10						ADDRESS
		INSTR(IPUSH, 10,INT),			// 0  Push Constant (10) Onto Stack 
		INSTR(GSTORE, 0,INT),			// 2  Store StackTop{Constant (10)} Globally var(N)								 
		INSTR(IPUSH, 0,INT),			// 4  Push Constant (0) Onto Stack
		INSTR(GSTORE, 1,INT),			// 6  Store StackTop{Constant (10)} Globally var(i)
										// do{
		INSTR(GLOAD, 1,INT),			// 15 Load i
		INSTR(IPUSH, 1,INT),			// 17 Push Constant (1) Onto Stack 
		SINSTR(IADD),					// 19 i = i+1
		INSTR(GSTORE, 1,INT),
										// }while( i < N)
		INSTR(GLOAD, 1,INT),			// 20
		INSTR(GLOAD, 0,INT)	,			// 22
		SINSTR(ILT),					// 24
		INSTR(BRTRUE, 8ull,POINTER),		// 25;
										
		SINSTR(HALT)					// 26
	};

	Type factorial[] = {
		//Instruction						// ADDRESS
		///Function factorial(var N)		// ARGS=1, LOCALS=0		
		INSTR(LOAD, -3,INT),				// [0]		Copy Given Argument N Into Current StackFrame
		INSTR(IPUSH, 2,INT),				// [2]		Push Constant (2) Onto Stack
		SINSTR(ILT),						// [4]		IF ((N < 2) == TRUE) Push TRUE ELSE Push FALSE Onto Stack
		INSTR(BRFALSE, 10ull,POINTER),		// [5]		Jump to Addr 10 if on Stack lays FALSE
		INSTR(IPUSH, 1,INT),				// [7]		Push Constant (1) Onto Stack
		SINSTR(RET),						// [9]		Return Constant (1)[7]
											// 
											//			RETURN N * factorial( N-1 )
		INSTR(LOAD, -3,INT),				// [10]		Copy Given Argument N Into Current StackFrame
		INSTR(LOAD, -3,INT),				// [12]		Copy Given Argument N Into Current StackFrame
		INSTR(IPUSH, 1,INT),				// [14]		Push Constant (1) Onto Stack
		SINSTR(ISUB),						// [16]		Substract Constant (1) of Value (N) 
		DINSTR(CALL, 0ull, 1,POINTER,INT),	// [17]		Call Function: factorial(var N)
		SINSTR(IMUL),						// [20]		Multiplicate Result of (factorial(var N))[17] times Value (N)[10]
		SINSTR(RET),						// [21]		Return Result[20]
		/// Function Main()					// ARGS=0, LOCALS=0
		//									 <-- MAIN METHOD!
		INSTR(IPUSH, 5,INT),				// [22]		Push Constant (5) Onto StacK
		DINSTR(CALL, 0ull, 1,POINTER,INT),	// [25]		Call Function: factorial(var N)
		SINSTR(PRINT),						// [26]		Print Stack Top
		SINSTR(HALT)						// [27]		Abort
	};



	VM::Memory CodeInstructions(std::begin(CodeLoop), std::end(CodeLoop));


	VM Machine(CodeInstructions, 0);

	try {
		///auto exe = &VM::cpu;
		//auto avg = (measure<std::chrono::milliseconds>::duration(Machine) );


		//std::cout << "Average Time: " << avg.count() << " ms\n";

		Machine.cpu();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}

	std::cin.get();
	return 0;
}